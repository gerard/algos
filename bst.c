#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#define N_ELEMS         100
#define N_DELETES       10000

struct bst_stats {
    int total_mallocs;
    int total_frees;
} stats;

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

struct node *node_sibling(struct node *n)
{
    if (node_is_left_child(n)) {
        return n->parent->right;
    } else if (node_is_right_child(n)) {
        return n->parent->left;
    } else {
        return NULL;
    }
}

int node_childs(struct node *n)
{
    return !!n->left + !!n->right;
}

struct node *bst_alloc(int v) {
    struct node *n = malloc(sizeof(struct node));
    stats.total_mallocs++;

    n->v = v;
    n->left = n->right = n->parent = NULL;

    return n;
}

void bst_free(struct node *n)
{
    free(n);
    stats.total_frees++;
}

int bst_size(struct node *tree)
{
    if (tree == NULL) return 0;
    else return bst_size(tree->left) + bst_size(tree->right) + 1;
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

struct node *bst_delete(struct node *tree, int v)
{
    struct node *n = bst_find(tree, v);

    if (!n) return tree;

    if (node_childs(n) == 0) {
        if (node_is_left_child(n)) {
            n->parent->left = NULL;
        } else if(node_is_right_child(n)) {
            n->parent->right = NULL;
        } else {
            assert(n == tree);
            tree = NULL;
        }
        bst_free(n);
        return tree;
    }

    if (node_childs(n) == 1) {
        struct node *n_only_child = n->left ? n->left : n->right;

        if (node_is_left_child(n)) {
            n->parent->left = n_only_child;
        } else if (node_is_right_child(n)) {
            n->parent->right = n_only_child;
        } else {
            assert(n == tree);
            tree = n_only_child;
        }

        n_only_child->parent = n->parent;
        bst_free(n);
        return tree;
    }

    assert(node_childs(n) == 2);
    struct node *n_succ = bst_successor(tree, n->v);
    assert(n_succ);

    int n_succ_value = n_succ->v;
    bst_delete(tree, n_succ_value);
    n->v = n_succ_value;

    return tree;
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
    if (!tree) return;

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

    assert(bst_size(root) == (stats.total_mallocs - stats.total_frees));
    assert_bst_properties(root);
    assert_bst_successor(root);

    for (i = 0; i < N_DELETES; i++) {
        root = bst_delete(root, values[random() % 100]);

        assert(bst_size(root) == (stats.total_mallocs - stats.total_frees));
        assert_bst_properties(root);
        assert_bst_successor(root);
    }

    return 0;
}
