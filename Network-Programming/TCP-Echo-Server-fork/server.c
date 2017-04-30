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

#define SERVICE  "echo"
#define BACKLOG  10
#define BUF_SIZE 4096

typedef void ( *sighandler_t )( int );

/**
   for log error message and exit the program
**/
void unixError( char *msg ) {
    fprintf( stderr, "%s: %s\n", msg, strerror( errno ) );
    exit( EXIT_FAILURE );
}

/**
   SIGCHLD handler to reap dead child processes
**/
void sigchldHandler( int sig ) {
    int savedErrno = errno;
    
    while( waitpid( -1, NULL, WNOHANG ) > 0 ) {
        continue;
    }
    errno = savedErrno;
}

/**
   more safe than signal()
**/
sighandler_t signalWrapper( int signum, sighandler_t handler ) {
    struct sigaction action, old_action;
 
    action.sa_handler = handler;
    sigemptyset( &action.sa_mask );           /* Block sigs of type being handled */
    action.sa_flags = SA_RESTART;             /* Restart syscalls if possible */
    if( sigaction( signum, &action, &old_action ) < 0 ) {
        unixError( "Signal() error" );
    }
    return ( old_action.sa_handler );
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

void handleClientRequest( int clientfd ) {
    char buf[BUF_SIZE];

    rio_t rio;
    rio_readinitb( &rio, clientfd );

    ssize_t numRead;
    while( ( numRead = rio_readlineb( &rio, buf, BUF_SIZE ) ) > 0 ) {
        printf( "server received %d bytes\n", numRead );
        if( rio_writen( clientfd, buf, numRead ) == -1 ) {
            unixError( "rio_writen() failed" );
        }
    }

    if( numRead == -1 ) {
        unixError( "rio_readlineb() failed" );
    }
}

int main(int argc, char *argv[])
{
    signalWrapper( SIGCHLD, sigchldHandler );

    int sockfd = setupServerListen( SERVICE, BACKLOG, NULL );
    if( sockfd == -1 ) {
        unixError( "could not create server socket" );
    }
    
    while( true ) {
        int clientfd = accept( sockfd, NULL, NULL );
        if( clientfd == -1 ) {
            unixError( "accept() failed" );
        }

        switch( fork() ) {
        case -1:
            fprintf( stderr, "%s: %s\n", "can't create child", strerror( errno ) );
            close( clientfd );
            break;
        case 0:
            close( sockfd );
            handleClientRequest( clientfd );
            close( clientfd );
            exit( EXIT_SUCCESS );
        default:
            close( clientfd );
            break;
        }
    }
    
    return 0;
}
