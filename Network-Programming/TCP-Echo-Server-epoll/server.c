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

#define BACKLOG     10
#define MAXEVENTS   64
#define SERVICE     "echo"

/**
   for log error message and exit the program
**/
void linuxError( char *msg ) {
    fprintf( stderr, "%s: %s\n", msg, strerror( errno ) );
    exit( EXIT_FAILURE );
}

/**
   make the socket for non-blocking
 **/
int makeSocketNonblock( int sockfd )
{
    int flags = fcntl( sockfd, F_GETFL, 0 );
    if( flags == -1 )
    {
        perror( "fcntl" );
        return -1;
    }
    
    flags |= O_NONBLOCK;
    int status = fcntl( sockfd, F_SETFL, flags );
    if( status == -1 )
    {
        perror( "fcntl" );
        return -1;
    }
    
    return 0;
}

/**
   Creates a SOCK_STREAM socket bound to the IP address on the TCP port specified by service.
   This function is designed for use by TCP servers.
**/
int setupServerListen( const char *service, int backlog, socklen_t *addrlen ) {
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
    for( ptr = servinfo; ptr != NULL; ptr = ptr->ai_next ) {
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
   write n bytes data from buf to clientfd
   return -1 on error
**/
ssize_t writeAllNonblock( int clientfd, void *buf, size_t n )
{
    size_t nleft = n;
    char *bufp = buf;
    
    while( nleft > 0 )
    {
        ssize_t nwritten = write( clientfd, bufp, nleft );
        if( nwritten == -1 )
        {
            if( errno == EAGAIN || errno == EWOULDBLOCK )
            {
                break;
            }
            else
            {
                return -1;
            }
        }
	
        nleft -= nwritten;
        bufp += nwritten;
    }
    
    return n;
}

/**
   read the content and send the same content back to the client
   return 0 on success or -1 on error
 **/
int echoClient( int clientfd )
{
    char buf[1024];

    /**
       call read() in loop:
       We must read whatever data is available completely,
       as we are running in edge-triggered mode
       and won't get a notification again for the same data.
     **/
    while( true )
    {
        ssize_t count = read( clientfd, buf, sizeof( buf ) );
        if( count == -1 )
        {
	     
	    // if errno == EAGAIN or EWOULDBLOCK that means we have read all data.
            if( errno == EAGAIN || errno == EWOULDBLOCK )
            {
                break;
            }
            else
            {
                return -1;
            }
        }
	// End of file. The remote client has closed the connection. 
        if( count == 0 )
	{
            return -1;
        }
	// write all the data back to the remote client
        if( writeAllNonblock( clientfd, buf, count ) == -1 )
        {
            return -1;
        }
    }
    return 0;
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
    int sockfd = setupServerListen( SERVICE, BACKLOG, NULL );
    if( sockfd == -1 )
    {
        linuxError( "could not create server socket" );
    }

    if( makeSocketNonblock( sockfd ) == -1 )
    {
        linuxError( "could not make socket non-block" );
    }

    int epfd = epoll_create1( 0 );
    if( epfd == -1 )
    {
        linuxError( "epoll_create1()" );
    }

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = sockfd;
    
    if( epoll_ctl( epfd, EPOLL_CTL_ADD, sockfd, &event ) == -1 )
    {
        linuxError( "epoll_ctl()" );
    }
    
    struct epoll_event *events = calloc( MAXEVENTS, sizeof( event ) );

    while( true )
    {
        int readyNum = epoll_wait( epfd, events, MAXEVENTS, -1 );
        if( readyNum == -1 )
        {
            if( errno == EINTR )
            {
                continue;
            }
            else
            {
                linuxError( "epoll_wait()" );
            }
        }
	
        for( int i = 0; i < readyNum; ++i )
        {
            if( events[i].data.fd == sockfd )
            {
		/**
		   We have a notification on the listening socket, which
		   means one or more incoming connections.
		**/
                while( true )
                {
                    struct sockaddr clientAddr;
                    socklen_t addrSize = sizeof( clientAddr );

                    int clientfd = accept( sockfd, &clientAddr, &addrSize );
                    if( clientfd == -1 )
                    {
                        if( errno == EAGAIN || errno == EWOULDBLOCK )
                        {
                            break;
                        }
                        else
                        {
                            linuxError( "accept()" );
                        }
                    }

                    printClientMessage( clientfd, &clientAddr, addrSize );
		    
                    if( makeSocketNonblock( clientfd ) == -1 )
		    {
                        linuxError( "could not set socket non-block" );
                    }

                    event.events = EPOLLIN | EPOLLET;
                    event.data.fd = clientfd;
		    
                    if( epoll_ctl( epfd, EPOLL_CTL_ADD, clientfd, &event ) == -1 )
		    {
                        linuxError( "epoll_ctl()" );		    
                    }
                }
            }
            else if( events[i].events & EPOLLIN )
            {
		/**
		   if connection closed by remote client or some error,
		   then we just close the socket descriptor.
		 **/
                if( echoClient( events[i].data.fd ) == -1 )
                {
                    printf( "Closed connection on descriptor %d\n", events[i].data.fd );

		    /**
		       Closing the descriptor will make epoll remove it
		       from the set of descriptors which are monitored. **/
                    close( events[i].data.fd );
                }
            }
	    else if( events[i].events & ( EPOLLHUP | EPOLLERR ) )
	    {
		// an error has occured on so we close the connection.
                fprintf( stderr, "epoll error\n" );
                close( events[i].data.fd );		
	    }
        }
    }
    
    free( events );
    close( sockfd );
    
    return 0;
}
