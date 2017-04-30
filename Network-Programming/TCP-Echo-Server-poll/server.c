#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdbool.h>
#include <poll.h>
#include <limits.h>

#define MAX_OPEN    256
#define BACKLOG     10
#define SERVICE     "echo"
#define BUF_SIZE    4096
#define INFTIM      -1 

/**
   for log error message and exit the program
**/
void linuxError( char *msg ) {
    fprintf( stderr, "%s: %s\n", msg, strerror( errno ) );
    exit( EXIT_FAILURE );
}

/**
   Creates a SOCK_STREAM socket bound to the IP address on the TCP port specified by service.
   This function is designed for use by TCP servers.
**/
int setupServerListen( const char *service, int backlog, socklen_t *addrlen )
{
    struct addrinfo hints;
    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    struct addrinfo *servinfo;
    int status = getaddrinfo( NULL, service, &hints, &servinfo );
    if( status != 0 ) {
        return -1;
    }

    int sockfd;
    struct addrinfo *ptr;
    for( ptr = servinfo; ptr != NULL; ptr = ptr->ai_next )
    {
        sockfd = socket( ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol );
        if( sockfd == -1 ) {
            continue;
        }

        int turn_on = 1;
        if( setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &turn_on, sizeof( turn_on ) ) == -1 ) {
            close( sockfd );
            freeaddrinfo( servinfo );
            return -1;
        }
        // if bind() success, then just return
        if( bind( sockfd, ptr->ai_addr, ptr->ai_addrlen ) == 0 ) {
            break;
        }
        // if bind() falied then close sockfd and try next address
        close( sockfd );
    }
    
    if( ptr == NULL ) {
        freeaddrinfo( servinfo );
        return -1;
    }

    if( listen( sockfd, backlog ) == -1 ) {
        freeaddrinfo( servinfo );
        return -1;
    }

    if( addrlen != NULL ) {
        *addrlen = ptr->ai_addrlen;
    }

    freeaddrinfo( servinfo );
    return sockfd;
}

/**
   print the client's infomation 
 **/
void printClientMessage( int clientfd, const struct sockaddr *clientAddr, socklen_t addrSize )
{
    char clientHost[NI_MAXHOST];
    char clientService[NI_MAXSERV];

    int ret = getnameinfo( clientAddr, addrSize, clientHost, sizeof( clientHost ),
                           clientService, sizeof( clientService ), 0 );
    if( ret == 0 )
    {
        printf( "Accepted connection on descriptor %d ( host=%s, port=%s )\n",
                clientfd, clientHost, clientService );
    }
    else
    {
        printf( "Accept connection but can't get client's information\n" );
    }    
}

int main()
{
    // get socket dedcriptor for server
    int serverfd = setupServerListen( SERVICE, BACKLOG, NULL );
    if( serverfd == -1 )
    {
        linuxError( "could not create server socket" );
    }

    struct pollfd client[MAX_OPEN];
    client[0].fd = serverfd;
    client[0].events = POLLRDNORM;

    for( int i = 1; i < MAX_OPEN; ++i )
    {
	client[i].fd = -1;
    }

    int maxi = 0;

    // buffer for receive data
    char buf[BUF_SIZE];

    while( true )
    {
	int nready = poll( client, maxi + 1, INFTIM );
	if( nready == -1 )
	{
	    linuxError( "poll" );
	}
	
	if( client[0].revents & POLLRDNORM )
	{
	    struct sockaddr clientAddr;
	    socklen_t addrSize = sizeof( clientAddr );
	    
	    int sockfd = accept( serverfd, &clientAddr, &addrSize );
	    if( sockfd == -1 )
	    {
		linuxError( "accept" );
	    }
	    
	    printClientMessage( sockfd, &clientAddr, addrSize );

	    int i;
	    for( i = 1; i < MAX_OPEN; ++i )
	    {
		if( client[i].fd < 0 )
		{
		    client[i].fd = sockfd;
		    break;
		}
	    }

	    if( i == MAX_OPEN )
	    {
		fprintf( stderr, "too many clients" );
		exit( 1 );
	    }
	    client[i].events = POLLRDNORM;
	    if( i > maxi )
	    {
		maxi = i;
	    }

	    if( --nready <= 0 )
	    {
		continue;
	    }
	}

	for( int i = 1; i <= maxi; ++i )
	{
	    if( client[i].fd < 0 )
	    {
		continue;
	    }
	    int sockfd = client[i].fd;
	    if( client[i].revents & ( POLLRDNORM | POLLERR ) )
	    {
		ssize_t n = read( sockfd, buf, BUF_SIZE );
		if( n < 0 )
		{
		    if( errno == ECONNRESET )
		    {
			close( sockfd );
			client[i].fd = -1;
		    }
		    else
		    {
			linuxError( "read" );			
		    }
		}
		else if( n == 0 )
		{
		    close( sockfd );
		    client[i].fd = -1;
		}
		else
		{
		    if( write( sockfd, buf, n ) < 0 )
		    {
			linuxError( "write" );
		    }
		}
		// no more readable descriptors
		if( --nready <= 0 )
		{
		    break;
		}
	    }

	}
    }
    // close the server's socket descriptor 
    close( serverfd );
    
    return 0;
}
