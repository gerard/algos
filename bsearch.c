#include <assert.h>

int bsearch(int *v, int s, int len)
{
    if (len == 1) {
        if (v[0] == s) return 0;
        else return -1;
    }

    int split = len / 2;
    if (v[split] == s) {
        return split;
    } else if (s < v[split]) {
        return bsearch(v, s, split);
    } else {
        return split + bsearch(v + split, s, len - split);
    }

    assert(0);
}

int main(int argc, char *argv[])
{
    int v[] = { 1, 4, 5, 10, 12, 15, 16 };
    int len = sizeof(v) / sizeof(int);

    assert(bsearch(v, 10, len) == 3);
    assert(bsearch(v, 1, len) == 0);
    assert(bsearch(v, 16, len) == 6);
    assert(bsearch(v, 2, len) == -1);

    return 0;
}
