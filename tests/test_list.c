#include "list.h"
#include "mbcl_test.h"

#include <stdlib.h>

int *int_new(int i)
{
    int *ip = malloc(sizeof(int *));
    *ip = i;
    return ip;
}

ssize_t compare_int(void *a, void *b)
{
    return *(int *)a - *(int *)b;
}

int test_list_sort()
{
    List *testList = list_new(free, compare_int);
    list_append(testList, int_new(5));
    list_append(testList, int_new(2));
    list_append(testList, int_new(7));
    list_append(testList, int_new(4));
    list_append(testList, int_new(8));
    list_append(testList, int_new(9));
    list_append(testList, int_new(3));

    list_sort(testList);

    Iterator *listIterator = list_begin(testList);
    int last = -1;
    while (iterator_gettable(listIterator))
    {
        int *gotten = iterator_get(listIterator);
        printf("%d\n", *gotten);
        MBCL_TEST_CHECK_OR_FAIL(*gotten > last, "sorted list is out of order");
        last = *gotten;
        iterator_next(listIterator);
    }

    list_free(testList);
    return 0;
}

int main(int argc, char *argv[])
{
    MBCL_ASSERT(argc == 2, "Provide number of list test to run");
    int whichTest = atoi(argv[1]);
    switch (whichTest)
    {
    case 1:
        test_list_sort();
        break;

    default:
        MBCL_ASSERT(0, "Invalid case number");
    }
}