#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define N_ELEMS 100

struct node {
    int v;
    struct node *left;
    struct node *right;
    struct node *parent;
};

int node_is_right_child(struct node *n)
{
    if (n == NULL || n->parent == NULL) return 0;
    return n->parent->right == n;
}

int node_is_left_child(struct node *n)
{
    if (n == NULL || n->parent == NULL) return 0;
    return n->parent->left == n;
}

struct node *bst_alloc(int v) {
    struct node *n = malloc(sizeof(struct node));

    n->v = v;
    n->left = n->right = n->parent = NULL;

    return n;
}

void bst_free(struct node *n)
{
    free(n);
}

struct node *bst_insert(struct node *tree, struct node *parent, int v)
{
    if (!tree) {
        struct node *n = bst_alloc(v);
        n->parent = parent;
        return n;
    }

    if (v < tree->v) {
        tree->left = bst_insert(tree->left, tree, v);
    } else if (v > tree->v) {
        tree->right = bst_insert(tree->right, tree, v);
    }

    return tree;
}

struct node *bst_find(struct node *tree, int v)
{
    if (!tree) return NULL;

    if (v < tree->v) return bst_find(tree->left, v);
    if (v > tree->v) return bst_find(tree->right, v);
    return tree;
}

struct node *bst_min(struct node *tree)
{
    if (!tree) return NULL;

    if (tree->left) return bst_min(tree->left);
    else return tree;
}

struct node *bst_successor(struct node *tree, int v)
{
    struct node *n = bst_find(tree, v);
    if (!n) return NULL;

    if (n->right) return bst_min(n->right);
    else {
        if (node_is_left_child(n)) return n->parent;
        else {
            while (node_is_right_child(n)) n = n->parent;
            return n->parent;
        }
    }
}

void assert_bst_properties(struct node *tree)
{
    if (!tree) return;

    if (tree->left) {
        assert(tree->left->v < tree->v);
        assert_bst_properties(tree->left);
    }

    if (tree->right) {
        assert(tree->right->v > tree->v);
        assert_bst_properties(tree->right);
    }
}

void assert_bst_successor(struct node *tree)
{
    struct node *snode;
    int last_min = bst_min(tree)->v;
    do {
        snode = bst_successor(tree, last_min);
        if (snode) {
            assert(last_min < snode->v);
            last_min = snode->v;
        }
    } while (snode);
}

int main(int argc, char *argv[])
{
    struct node *root = NULL;
    int values[N_ELEMS];
    int i;

    srandom(time(NULL));

    for (i = 0; i < N_ELEMS; i++) {
        values[i] = random() % 256;
        root = bst_insert(root, NULL, values[i]);
    }

    assert_bst_properties(root);
    assert_bst_successor(root);
    return 0;
}
