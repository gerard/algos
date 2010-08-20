#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#define ARRAY_SIZE      103

int binarysearch(int *v, int s, int len)
{
    if (len == 1) {
        if (v[0] == s) return 0;
        else return -1;
    }

    int split = len / 2;
    if (v[split] == s) {
        return split;
    } else if (s < v[split]) {
        return binarysearch(v, s, split);
    } else {
        return split + binarysearch(v + split, s, len - split);
    }

    assert(0);
}

int main(int argc, char *argv[])
{
    int v[ARRAY_SIZE];
    int i;

    struct timeval tv;
    memset(&tv, 0, sizeof(struct timeval));
    gettimeofday(&tv);
    srandom(tv.tv_usec);

    v[0] = random() % 256;
    for (i = 1; i < ARRAY_SIZE; i++) {
        v[i] = v[i-1] + random() % 256 + 1;
    }

    for (i = 0; i < ARRAY_SIZE; i++) {
        assert(binarysearch(v, v[i], ARRAY_SIZE) == i);
    }

    return 0;
}
