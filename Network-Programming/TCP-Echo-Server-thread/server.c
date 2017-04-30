#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#define SERVICE  "echo"
#define BACKLOG  10
#define BUF_SIZE 4096

/**
   for log error message and exit the program
**/
void unixError( char *msg ) {
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
    if( status != 0 )
    {
        return -1;
    }

    int sockfd;
    struct addrinfo *ptr;
    for( ptr = servinfo; ptr != NULL; ptr = ptr->ai_next )
    {
        sockfd = socket( ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol );
        if( sockfd == -1 )
	{
            continue;
        }

        int turn_on = 1;
        if( setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &turn_on, sizeof( turn_on ) ) == -1 )
	{
            close( sockfd );
            freeaddrinfo( servinfo );
            return -1;
        }

        // if bind() success, then just return
        if( bind( sockfd, ptr->ai_addr, ptr->ai_addrlen ) == 0 )
	{
            break;
        }
        // if bind() falied then close sockfd and try next address
        close( sockfd );
    }

    if( ptr == NULL )
    {
        freeaddrinfo( servinfo );
        return -1;
    }

    if( listen( sockfd, backlog ) == -1 )
    {
        freeaddrinfo( servinfo );
        return -1;
    }

    if( addrlen != NULL )
    {
        *addrlen = ptr->ai_addrlen;
    }

    freeaddrinfo( servinfo );
    return sockfd;
}

void handleClientRequest( int clientfd )
{
    char buf[BUF_SIZE];

    ssize_t numRead;
    
    while( ( numRead = read( clientfd, buf, BUF_SIZE ) ) > 0 )
    {
	if( write( clientfd, buf, numRead ) == -1 )
	{
            unixError( "writen" );
        }
    }
    
    if( numRead == -1 ) {
        unixError( "read" );
    }
}

void *thread( void *vargp )
{
    int clientfd = *( (int *)vargp );
    pthread_detach( pthread_self() );
    free( vargp );
    
    handleClientRequest( clientfd );
    close( clientfd );
    return NULL;
}

int main(int argc, char *argv[])
{
    int sockfd = setupServerListen( SERVICE, BACKLOG, NULL );
    if( sockfd == -1 )
    {
        unixError( "could not create server socket" );
    }
    
    pthread_t tid;
    int *clientfdp;

    while( true )
    {
	clientfdp = malloc( sizeof(int) );
	if( clientfdp == NULL )
	{
	    unixError( "malloc" );
	}
	
        *clientfdp = accept( sockfd, NULL, NULL );
        if( *clientfdp == -1 )
	{
            unixError( "accept() failed" );
        }
	
	int err = pthread_create( &tid, NULL, thread, clientfdp );
	if( err != 0 )
	{
	    fprintf( stderr, "%s: %s\n", "pthread_failed()", strerror( errno ) );
	    close( *clientfdp );        // give up this connection
	    free( clientfdp );
	    continue;                   // try next client
	}
    }
    
    return 0;
}
