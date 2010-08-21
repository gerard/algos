#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct trie {
    char c;
    unsigned int count;
    struct trie **next;
};

int main(int argc, char *argv[])
{
    struct trie root;
    struct trie *cur;
    int c;
    unsigned int malloc_pointers_count = 0;
    unsigned int malloc_nodes_count = 0;

    if (argc == 1) {
        close(0);

        /* Any decent *nix should have this file */
        open("/usr/share/dict/words", 0);
    }

    memset(&root, 0, sizeof(struct trie));
    cur = &root;

    while ((c = getchar()) != EOF) {
        switch (c) {
        case '\n':
            cur->count++;
            cur = &root;
            break;
        default:
            if (!cur->next) {
                malloc_pointers_count++;
                cur->next = calloc(UCHAR_MAX, sizeof(struct trie *));
            }
            if (!cur->next[c]) {
                malloc_nodes_count++;
                cur->next[c] = calloc(1, sizeof(struct trie));
            }
            cur = cur->next[c];
            break;
        }
    }

    size_t pointer_mem_consumption = malloc_pointers_count * UCHAR_MAX * sizeof(struct trie *);
    size_t node_mem_consumption = malloc_nodes_count * sizeof(struct trie);
    printf("Total consumed bytes: %zukB\n", (pointer_mem_consumption + node_mem_consumption) / 1024);

    return 0;
}
