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
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "rio.h"

#define SERVICE "echo"
#define BUF_SIZE 4096

/**
   for log error message and exit the program
**/
void unixError( char *msg ) {
    fprintf( stderr, "%s: %s\n", msg, strerror( errno ) );
    exit( EXIT_FAILURE );
}


int setupClientConnect( const char *host, const char *service ) {
    struct addrinfo hints;
    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
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

        if( connect( sockfd, ptr->ai_addr, ptr->ai_addrlen ) != -1 ) {
            break;
        }

        // if bind() falied then close sockfd and try next address
        close( sockfd );
    }

    freeaddrinfo( servinfo );
    return ( ptr == NULL ? -1 : sockfd );
}

char *Fgets( char *ptr, int n, FILE *stream ) 
{
    char *rptr;

    if( ( ( rptr = fgets( ptr, n, stream ) ) == NULL ) && ferror( stream ) ) {
        fprintf( stderr, "%s\n", "Fgets() failed" );
        exit( EXIT_FAILURE );
    }

    return rptr;
}

int main(int argc, char *argv[])
{
    if( argc != 2 ) {
        fprintf( stderr, "usage: %s <host>\n", argv[0] );
        exit( 0 );
    }

    const char *host = argv[1];

    int clientfd = setupClientConnect( host, SERVICE );
    if( clientfd == -1 ) {
        unixError( "could not connect to server socket" );
    }

    char buf[BUF_SIZE];
    rio_t rio;
    rio_readinitb( &rio, clientfd );

    while( Fgets( buf, BUF_SIZE, stdin ) != NULL ) {
        rio_writen( clientfd, buf, strlen( buf ) );
        rio_readlineb( &rio, buf, BUF_SIZE );
        
        if( fputs( buf, stdout ) == EOF ) {
            unixError( "Fputs() error" );	
        }
    }

    close( clientfd );
    
    return 0;
}
