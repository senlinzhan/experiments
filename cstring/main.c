#include "cstring.h"
#include <stdio.h>

void print_String(const String *s)
{
    printf("string: \n\tcontent:%s\n\tsize:%d\n\tcapacity: %d\n\n", s->data, s->size, s->capacity);
}

int main(void)
{ 
    String *str = String_create("Hello");
    print_String(str);

    String *other = String_copy(str);
    print_String(other);

    String_append(str, ", World!");
    print_String(str);

    for (int i = 0; i < 10; i++)
    {
	String_append(str, "Hello, World");
	print_String(str);
    }

    String_destroy(str);
    String_destroy(other);
    
    return 0;
}
