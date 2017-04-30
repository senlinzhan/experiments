#include "rio.h"

ssize_t rio_readn( int fd, void *buf, size_t n )
{
    size_t nleft = n;
    char *bufp = buf;

    while( nleft > 0 )
    {
        ssize_t nread = read( fd, bufp, nleft );

        // EOF
        if( nread == 0 ) {
            break;
        }
	
        if( nread == -1 )
        {
            if( errno == EINTR ) {     // Interrupted by signal handler 
                continue;
            } else {                   // if error just return
                return -1;
            }
        } 

        nleft -= nread;
        bufp += nread;
    }
    return n - nleft;
}

ssize_t rio_writen( int fd, void *buf, size_t n )
{
    size_t nleft = n;
    char *bufp = buf;

    while( nleft > 0 )
    {
        ssize_t nwritten = write( fd, bufp, nleft );
        if( nwritten == -1 )
        {
            if( errno == EINTR ) {
                continue;
            } else {
                return -1;
            }
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    
    return n;
}


void rio_readinitb( rio_t *rp, int fd ) {
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read( rio_t *rp, char *usrbuf, size_t n ) {
    while( rp->rio_cnt <= 0 ) {
        rp->rio_cnt = read( rp->rio_fd, rp->rio_buf, sizeof( rp->rio_buf ) );
        if( rp->rio_cnt < 0 ) {
            if( errno != EINTR ) {
                return -1;
            }
        } else if( rp->rio_cnt == 0 ) {
            return 0;
        } else {
            rp->rio_bufptr = rp->rio_buf;
        }
    }

    int cnt = n;
    if( rp->rio_cnt < n ) {
        cnt = rp->rio_cnt;
    }

    memcpy( usrbuf, rp->rio_bufptr, cnt );
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

ssize_t rio_readnb( rio_t *rp, void *buf, size_t n )
{
    size_t nleft = n;
    char *bufp = buf;

    while( nleft > 0 )
    {
        ssize_t nread = rio_read( rp, bufp, nleft );

        // EOF
        if( nread == 0 ) {
            break;
        }
	
        if( nread == -1 ) {
            if( errno == EINTR ) {     // Interrupted by signal handler 
                continue;
            } else {                   // if error just return
                return -1;
            }
        } 

        nleft -= nread;
        bufp += nread;
    }
    return n - nleft;
}

ssize_t rio_readlineb( rio_t *rp, void *usrbuf, size_t maxlen ) {
    int n, rc;
    char c, *bufp = usrbuf;
    
    for( n = 1; n < maxlen; ++n ) {
        rc = rio_read( rp, &c, 1 );
        if( rc == 1 ) {
            *bufp++ = c;
            if( c == '\n' ) {
                break;
            }
        } else if( rc == 0 ) {
            if( n == 1 ) {
                return 0;       // EOF, no data read
            } else {
                break;          // EOF, some data was read
            }
        } else {
            return -1;          // ERROR
        }
    }

    *bufp = 0;
    return n;
}
