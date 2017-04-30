#ifndef CSTRING_H
#define CSTRING_H

#include <stdio.h>

typedef struct String
{
    int    size;
    int    capacity;
    char  *data;
} String;


String *String_create(const char *str);

int String_append(String *s, const char *str);

void String_destroy(String *s);

int String_size(const String *s);

int String_capacity(const String *s);

String *String_substring(const String *s, int first, int last);

String *String_copy(const String *s);

String *String_merge(const String *first, const String *second);

char *String_find(const String *s, char c);

char *String_at(const String *s, int n);

String *readline(FILE *fp);


#endif /* CSTRING_H */
