#include <assert.h>
#include <string.h>

/* TODO: Do sorting in place */

void assert_inc_order(int *v, int len)
{
    int i;
    for (i = 1; i < len; i++) {
        assert(v[i-1] <= v[i]);
    }
}

void merge(int *dest, int *v1, int *v2, int l1, int l2)
{
    int i = 0;
    int j = 0;
    int k = 0;

    while (j < l1 && k < l2) {
        if (v1[j] < v2[k]) {
            dest[i++] = v1[j++];
        } else {
            dest[i++] = v2[k++];
        }
    }

    while (j < l1) dest[i++] = v1[j++];
    while (k < l2) dest[i++] = v2[k++];
}

void msort(int *v, int len)
{
    if (len == 0) return;
    if (len == 1) return;

    int split = len / 2;
    int vt1[split];
    int vt2[len - split];

    memcpy(vt1, v, split * sizeof(int));
    memcpy(vt2, v + split, (len - split) * sizeof(int));

    msort(vt1, split);
    msort(vt2, len - split);
    merge(v, vt1, vt2, split, len - split);
}

int main(int argc, char *argv[])
{
    int v[] = { 3, 1, 19, 48, 12, 48, 8, 91, 9};
    int len = sizeof(v) / sizeof(int);

    msort(v, len);
    assert_inc_order(v, len);

    return 0;
}
