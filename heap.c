#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "perf.h"
#define N_ELEMS         100

int LEFT(int pos)       { return 2 * pos; }
int RIGHT(int pos)      { return 2 * pos + 1; }

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void minheap_heapify(int *v, int p, int len)
{
    int l = LEFT(p);
    int r = RIGHT(p);
    int *min_child;

    if (l > len) return;

    if (r > len) {
        min_child = &v[l];
    } else {
        min_child = v[l] < v[r] ? &v[l] : &v[r];
    }

    if (v[p] > *min_child) {
        swap(&v[p], min_child);
        if (min_child == &v[l]) minheap_heapify(v, l, len);
        else minheap_heapify(v, r, len);
    }
}

int minheap_extract(int *v, int *len)
{
    int ret = v[1];

    PERF_START();
    PERF_PROBLEM_SIZE(*len);
    v[1] = v[*len];
    (*len)--;
    minheap_heapify(v, 1, *len);
    PERF_STOP();

    return ret;
}

void minheap_build(int *v, int len)
{
    int i;

    PERF_START();
    PERF_PROBLEM_SIZE(len);
    for (i = len; i > 0; i--) {
        minheap_heapify(v, i, len);
    }
    PERF_STOP();
}

void assert_minheap_properties(int *v, int p, int len)
{
    int l = LEFT(p);
    int r = RIGHT(p);

    if (l >= len) return;
    assert(v[p] <= v[l]);

    if (r >= len) return;
    assert(v[p] <= v[r]);

    assert_minheap_properties(v, l, len);
    assert_minheap_properties(v, r, len);
}

int main(int argc, char *argv[])
{
    int len;
    int i;
    int long_run = argc > 1 ? 1 : 0;

    struct timeval tv;
    memset(&tv, 0, sizeof(struct timeval));
    gettimeofday(&tv, NULL);
    srandom(tv.tv_usec);

    const int step = 20000;
    int runs;

    if (!long_run) {
        runs = 1;
    } else {
        runs = 50;
    }

    for (len = step; len <= step * runs; len += step) {
        int *v = malloc(sizeof(int) * len);

        for (i = 1; i < len + 1; i++) {
            v[i] = random() & 0xFFFFFF;
        }

        minheap_build(v, len);
        assert_minheap_properties(v, 1, len);

        int min_prev = -1;
        int heap_size = len;
        while (heap_size) {
            int min = minheap_extract(v, &heap_size);
            if (heap_size == len - 5000) {
                /* Calculate first 5000 iters */
                if (long_run) PERF_GNUPLOT_PRINT(len, "minheap_extract");
            }
            assert(min_prev <= min);
            min_prev = min;
        }

        free(v);

        char s[200];
        sprintf(s, "HEAP: Number of elements = %d", len);
        if (long_run) {
            PERF_PRINT(stdout, s, "\t");
            PERF_GNUPLOT_PRINT(len, "minheap_build");
        }

        PERF_CLEAR();
    }

    return 0;
}
