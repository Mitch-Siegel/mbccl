#include "set.h"
#include <stdlib.h>
#include <string.h>
/*

does [fun] already exist in bucket 6?
        no
fun goes in bucket 6
does [uart_putc] already exist in bucket 6?
        no
uart_putc goes in bucket 6
does [u8] already exist in bucket 6?
        no
u8 goes in bucket 6
does [] already exist in bucket 1?
        no
 goes in bucket 1
does [c] already exist in bucket 2?
        no
c goes in bucket 2
does [] already exist in bucket 1?
        yes
does [fun] already exist in bucket 6?
        no*/

ssize_t compare_string(void *a, void *b)
{
    return strcmp(a, b);
}

int main()
{
    Set *testSet = set_new(free, compare_string);
    set_insert(testSet, strdup("fun"));
    set_insert(testSet, strdup("uart_putc"));
    set_insert(testSet, strdup("u8"));
    set_insert(testSet, strdup(""));
    set_insert(testSet, strdup("c"));
    // set_insert(testSet, strdup(""));
    set_insert(testSet, strdup("fun"));
}