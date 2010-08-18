#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define ARRAY_SIZE  100

void fill_random_array(int *v, int len)
{
    int i;

    srandom(time(NULL));
    for (i = 0; i < len; i++) {
        v[i] = (int)random() % 256;
    }
}

void assert_inc_order(int *v, int len)
{
    int i;

    for (i = 1; i < len; i++) {
        assert(v[i-1] <= v[i]);
    }
}

void assert_partition(int *v, int start, int end, int p)
{
    int i;

    for (i = start; i < p; i++) assert(v[i] <= v[p]);
    for (i = p + 1; i < end; i++) assert(v[i] >= v[p]);
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(int *v, int start, int end)
{
    int i, j;
    int r = v[end - 1];

    for (i = start - 1, j = start; j < end - 1; j++) {
        if (v[j] < r) swap(&v[++i], &v[j]);
    }
    swap(&v[++i], &v[end - 1]);

    assert_partition(v, start, end, i);

    return i;
}

void quicksort(int *v, int start, int end)
{
    if (start < end - 1) {
        int p = partition(v, start, end);
        quicksort(v, start, p);
        quicksort(v, p + 1, end);
    }
}

int main(int argc, char *argv[])
{
    int v[ARRAY_SIZE];

    fill_random_array(v, ARRAY_SIZE);
    quicksort(v, 0, ARRAY_SIZE);
    assert_inc_order(v, ARRAY_SIZE);

    return 0;
}
