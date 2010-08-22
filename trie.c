#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LEN         128

struct trie {
    unsigned int count;
    struct trie **next;
};

int trie_find_max(struct trie *t, char *ret)
{
    char s[MAX_LEN] = "";
    int max, i;

    strcpy(ret, s);
    max = t->count;

    if (!t->next) return max;

    for (i = 0; i < UCHAR_MAX; i++) {
        if (!t->next[i]) continue;

        memset(s, 0, MAX_LEN);
        int n = trie_find_max(t->next[i], s);
        if (n > max) {
            max = n;
            ret[0] = (char)i;
            ret[1] = 0;
            strcat(ret, s);
        }
    }

    return max;
}

int main(int argc, char *argv[])
{
    struct trie root;
    struct trie *cur;
    int c;
    unsigned int malloc_pointers_count = 0;
    unsigned int malloc_nodes_count = 0;

    const char *fname = argc == 1 ? "trie.c" : argv[1];

    close(0);
    if (open(fname, 0) < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    memset(&root, 0, sizeof(struct trie));
    cur = &root;

    while ((c = getchar()) != EOF) {
        if (isalpha(c)) {
            if (!cur->next) {
                malloc_pointers_count++;
                cur->next = calloc(UCHAR_MAX, sizeof(struct trie *));
            }
            if (!cur->next[c]) {
                malloc_nodes_count++;
                cur->next[c] = calloc(1, sizeof(struct trie));
            }
            cur = cur->next[c];
        } else {
            cur->count++;
            cur = &root;
        }
    }

    char s[MAX_LEN] = "";
    root.count = 0;
    int n = trie_find_max(&root, s);
    printf("Most common word: \"%s\" [%d]\n", s, n);

    size_t pointer_mem_consumption = malloc_pointers_count * UCHAR_MAX * sizeof(struct trie *);
    size_t node_mem_consumption = malloc_nodes_count * sizeof(struct trie);
    printf("Total consumed bytes: %zukB\n", (pointer_mem_consumption + node_mem_consumption) / 1024);

    return EXIT_SUCCESS;
}
