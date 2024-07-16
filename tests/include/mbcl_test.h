#include <stdio.h>

#define MAX(a, b) ((a > b) ? a : b)

#define MBCL_TEST_CHECK_OR_FAIL(condition, ...) if (!(condition)) { printf(__VA_ARGS__); return 1; }