#include <stdlib.h>
#include <time.h>
#include <assert.h>
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

    v[1] = v[*len];
    (*len)--;
    minheap_heapify(v, 1, *len);

    return ret;
}

void minheap_build(int *v, int len)
{
    int i;

    for (i = len; i > 0; i--) {
        minheap_heapify(v, i, len);
    }
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
    int v[N_ELEMS + 1];
    int len = N_ELEMS;
    int i;

    struct timeval tv;
    gettimeofday(&tv);
    srandom(tv.tv_usec);

    for (i = 1; i < N_ELEMS + 1; i++) {
        v[i] = random() % 255;
    }

    minheap_build(v, len);
    assert_minheap_properties(v, 1, len);

    return 0;
    int min_prev = -1;
    while (len) {
        int min = minheap_extract(v, &len);
        assert(min_prev <= min);
        min_prev = min;
    }

    return 0;
}
