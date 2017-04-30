#include "cstring.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static int expand_capacity(String *s, int capacity_at_least);
static int max(int first, int second);

String *String_create(const char *str)
{
    static const int INIT_CAPACITY = 10;
    
    String *s = (String *)malloc(sizeof(String));
    if (s == NULL)
    {
	return NULL;
    }
    
    int size = (str == NULL) ? 0 : strlen(str);
    int capacity = (size <= INIT_CAPACITY) ? INIT_CAPACITY : size;

    s->data = (char *)malloc(capacity + 1);
    if (s->data == NULL)
    {
	free(s);
	return NULL;
    }

    s->size = size; 
    s->capacity = capacity;

    memcpy(s->data, str, size); 
    s->data[size] = '\0';

    return s;
}

void String_destroy(String *s)
{
    if (s != NULL)
    {
	if( s->data != NULL)
	{
	    free(s->data);
	}
	free(s);
    }
}


int String_append(String *s, const char *str)
{
    if (str == NULL)
    {
	return 0;
    }
    
    int size = s->size + strlen(str);
    if(size > s->capacity)
    {
	if (expand_capacity(s, size) == -1)
	{
	    return -1;
	}
    }

    strncat(s->data, str, strlen(str));
    s->size = size;

    return 0;
}

int max(int first, int second)
{
    return (first > second) ? first : second;
}

int expand_capacity(String *s, int capacity_at_least)
{
    int capacity = max(s->capacity * 2, capacity_at_least);
    
    char *data = (char *)malloc(capacity + 1);    
    if (data == NULL)
    {
	return -1;
    }

    memcpy(data, s->data, s->size);
    data[s->size] = '\0';
    free(s->data);
    
    s->data = data;
    s->capacity = capacity;
	
    return 0;
}


int String_size(const String *s)
{
    return (s != NULL) ? s->size : 0;
}

int String_capacity(const String *s)
{
    return (s != NULL) ? s->capacity : 0;
}

char *String_find(const String *s, char c)
{
    if (s == NULL || s->data == NULL)
    {
	return NULL;
    }

    for (char *ptr = s->data; *ptr != '\0'; ++ptr)
    {
	if (*ptr == c)
	{
	    return ptr;
	}   
    }
  
    return NULL;
}

char *String_at(const String *s, int n)
{
    if (s == NULL || n < 0 || n >= s->size)
    {
	return NULL;
    }

    return s->data + n;
}


String *String_copy(const String *s)
{
    if (s == NULL)
    {
	errno = 0;
	return NULL;
    }

    String *other = (String *)malloc(sizeof(String));
    if (other == NULL)
    {
	return NULL;
    }
    other->size = s->size;
    other->capacity = s->capacity;
    
    other->data = (char *)malloc(other->capacity + 1);
    if (other->data == NULL)
    {
	free(other);
	return NULL;
    }

    memcpy(other->data, s->data, s->size);
    other->data[other->size] = '\0';
    return other;
}


String *String_merge(const String *first, const String *second)
{
    if (first == NULL && second == NULL)
    {
	errno = 0;
	return NULL;
    }
    else if (first != NULL && second != NULL)
    {
	String *s = String_copy(first);
	if (s == NULL)
	{
	    return NULL;
	}
	int status = String_append(s, second->data);
	if (status == -1)
	{
	    String_destroy(s);
	    return NULL;
	}
	return s;
    }
    
    const String *s = (first != NULL) ? first : second;
    return String_copy(s);
}

String *String_substring(const String *s, int first, int last)
{
    if (s == NULL || first > last || first < 0 || last > s->size)
    {
	errno = 0;
	return NULL;
    }
    
    int size = last - first;
    char *str = (char *)malloc(size + 1);
    if (str == NULL)
    {
	return NULL;
    }
    
    memcpy(str, s->data + first, size);
    str[size] = '\0';

    String *substring = String_create(str);
    
    free(str);
    return substring;
}


String *readline(FILE *fp)
{ 
    static const int BUF_SIZE = 1024;
    
    String *line = NULL;
    char buf[BUF_SIZE];
    bool has_next = true;
    
    while(has_next)
    {
	char *s = fgets(buf, BUF_SIZE, fp);
	if (s == NULL)
	{
	    break;
	}
	int size = strlen(buf);

	if (buf[size] == '\n')
	{
	    buf[size] = '\0';
	    has_next = false;
	}
	
	if (line == NULL)
	{
	    line = String_create(buf);
	    if (line == NULL)
	    {
		return NULL;
	    }
	}
	else
	{
	    int status = String_append(line, buf);
	    if (status == -1)
	    {
		String_destroy(line);
		return NULL;
	    }
	}
    }
    return line;
}
