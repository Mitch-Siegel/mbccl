#include "deque.h"
#include "mbcl_test.h"

#include <stdio.h>
#include <stdlib.h>

int *int_new(int i)
{
    int *ip = malloc(sizeof(int *));
    *ip = i;
    return ip;
}

int deque_pop_back_int(Deque *deque)
{
    int *data = deque_pop_back(deque);
    int value = *data;
    free(data);
    return value;
}

int deque_pop_front_int(Deque *deque)
{
    int *data = deque_pop_front(deque);
    int value = *data;
    free(data);
    return value;
}

int test_deque_forward_queue()
{
    Deque *deque = deque_new(free);

    for (int i = 0; i < 10; i++)
    {
        deque_push_front(deque, int_new(i));
    }

    for (int i = 0; i < 10; i++)
    {
        int popped = deque_pop_back_int(deque);
        printf("%d\n", popped);
        MBCL_TEST_CHECK_OR_FAIL(popped == i, "deque forwards queue popped unexpected value");
    }
    printf("\n");

    deque_free(deque);
    return 0;
}

int test_deque_backward_queue()
{
    Deque *deque = deque_new(free);

    for (int i = 0; i < 10; i++)
    {
        deque_push_back(deque, int_new(i));
    }

    for (int i = 0; i < 10; i++)
    {
        int popped = deque_pop_front_int(deque);
        printf("%d\n", popped);
        MBCL_TEST_CHECK_OR_FAIL(popped == i, "deque backwards queue popped unexpected value");
    }
    printf("\n");

    deque_free(deque);
    return 0;
}

int test_deque_interleaved()
{
    Deque *testDeque = deque_new(free);
    deque_push_front(testDeque, int_new(10));
    deque_push_back(testDeque, int_new(20));
    deque_push_front(testDeque, int_new(30));
    deque_push_back(testDeque, int_new(40));

    MBCL_TEST_CHECK_OR_FAIL(deque_pop_front_int(testDeque) == 30, "unexpected result from interleaved back/front pops");
    MBCL_TEST_CHECK_OR_FAIL(deque_pop_back_int(testDeque) == 40, "unexpected result from interleaved back/front pops");
    MBCL_TEST_CHECK_OR_FAIL(deque_pop_front_int(testDeque) == 10, "unexpected result from interleaved back/front pops");
    MBCL_TEST_CHECK_OR_FAIL(deque_pop_back_int(testDeque) == 20, "unexpected result from interleaved back/front pops");

    deque_free(testDeque);
    return 0;
}

int main(int argc, char *argv[])
{
    MBCL_ASSERT(argc == 2, "Provide number of deque test to run");
    int whichTest = atoi(argv[1]);
    switch (whichTest)
    {
    case 0:
        return test_deque_forward_queue();
        break;

    case 1:
        return test_deque_backward_queue();
        break;

    case 2:
        return test_deque_interleaved();

    default:
        MBCL_ASSERT(0, "Invalid case number");
    }
}