#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#define N_ELEMS         100
#define N_DELETES       100

#ifndef bst_fixup_insert
#define bst_fixup_insert(n)     (n)
#endif

#ifndef bst_fixup_delete
#define bst_fixup_delete(n)     (n)
#endif

struct bst_stats {
    int total_mallocs;
    int total_frees;
    int left_rotations;
    int right_rotations;
} stats;

struct node {
    struct node *left;
    struct node *right;
    struct node *parent;
    int v;
    int self_balancing_data;
};

void bst_stats_clear(void)
{
    memset(&stats, 0, sizeof(struct bst_stats));
}

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

    memset(n, 0, sizeof(struct node));
    n->v = v;

    stats.total_mallocs++;
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

struct node *bst_root(struct node *n)
{
    while (n->parent) n = n->parent;
    return n;
}

struct node *bst_insert(struct node *tree, int v)
{
    if (!tree) {
        struct node *n = bst_alloc(v);
        return bst_fixup_insert(n);
    }

    if (v < tree->v) {
        tree->left = bst_insert(tree->left, v);
        tree->left->parent = tree;

        tree->left = bst_fixup_insert(tree->left);
    } else if(v > tree->v) {
        tree->right = bst_insert(tree->right, v);
        tree->right->parent = tree;

        tree->right = bst_fixup_insert(tree->right);
    }

    return bst_fixup_insert(tree);
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

struct node *bst_delete_simple(struct node *tree, int v)
{
    if (!tree) return NULL;

    if (tree->v > v) {
        tree->left = bst_delete_simple(tree->left, v);
        return bst_fixup_delete(tree);
    }

    if (tree->v < v) {
        tree->right = bst_delete_simple(tree->right, v);
        return bst_fixup_delete(tree);
    }

    if (node_childs(tree) == 0) {
        bst_free(tree);
        return NULL;
    }

    if (node_childs(tree) == 1) {
        struct node *tree_child = tree->left ? tree->left : tree->right;

        tree_child->parent = tree->parent;
        bst_free(tree);
        return tree_child;
    }

    /* We omit this case which is handled with bst_delete() */
    assert(node_childs(tree) == 2);
    return tree;
}

struct node *bst_delete(struct node *tree, int v)
{
    struct node *del_node = NULL;
    tree = bst_delete_simple(tree, v);

    if (bst_find(tree, v)) {
        int v_succ = bst_successor(tree, v)->v;
        tree = bst_delete_simple(tree, v_succ);

        bst_find(tree, v)->v = v_succ;
    }

    return tree;
}

void assert_bst_properties(struct node *tree)
{
    if (!tree) return;

    if (tree->left) {
        assert(tree->left->v <= tree->v);
        assert_bst_properties(tree->left);
    }

    if (tree->right) {
        assert(tree->right->v >= tree->v);
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
            assert(last_min <= snode->v);
            last_min = snode->v;
        }
    } while (snode);
}

struct node *bst_rotate_left(struct node *root)
{
    assert(root && root->right);

    struct node *new_root = root->right;
    struct node *beta = new_root->left;

    root->right = beta;
    new_root->left = root;

    new_root->parent = root->parent;
    root->parent = new_root;
    if (beta) beta->parent = root;

    stats.left_rotations++;
    return new_root;
}

struct node *bst_rotate_right(struct node *root)
{
    assert(root && root->left);

    struct node *new_root = root->left;
    struct node *beta = new_root->right;

    root->left = beta;
    new_root->right = root;

    new_root->parent = root->parent;
    root->parent = new_root;
    if (beta) beta->parent = root;

    stats.right_rotations++;
    return new_root;
}

void bst_free_all(struct node *tree)
{
    while (tree) tree = bst_delete(tree, tree->v);
}

typedef struct node *(*bst_postorder_f)(struct node *);
struct node *bst_postorder(struct node *tree, bst_postorder_f node_process)
{
    tree = node_process(tree);
    if (tree->left) tree->left = bst_postorder(tree->left, node_process);
    if (tree->right) tree->right = bst_postorder(tree->right, node_process);

    return tree;
}

int bst_main(int argc, char *argv[])
{
    struct node *random_rotation(struct node *n)
    {
        if (random() % 2) {
            if (random() % 2 && n->left) return bst_rotate_right(n);
            if (random() % 2 && n->right) return bst_rotate_left(n);
        }
        return n;
    }

    struct node *root = NULL;
    int values[N_ELEMS];
    int i;

    struct timeval tv;
    memset(&tv, 0, sizeof(struct timeval));
    gettimeofday(&tv);
    srandom(tv.tv_usec);

    for (i = 0; i < N_ELEMS; i++) {
        values[i] = random() % 256;
        root = bst_insert(root, values[i]);
    }

    assert(bst_size(root) == (stats.total_mallocs - stats.total_frees));
    assert_bst_properties(root);
    assert_bst_successor(root);

    stats.left_rotations = 0;
    stats.right_rotations = 0;
    for (i = 0; root->left; i++) root = bst_rotate_right(root);

    assert(bst_size(root) == (stats.total_mallocs - stats.total_frees));
    assert_bst_properties(root);
    assert_bst_successor(root);

    for (; i > 0; i--) root = bst_rotate_left(root);

    assert(stats.left_rotations == stats.right_rotations);
    root = bst_postorder(root, random_rotation);

    for (i = 0; i < N_DELETES; i++) {
        root = bst_delete(root, values[random() % 100]);

        assert(bst_size(root) == (stats.total_mallocs - stats.total_frees));
        assert_bst_properties(root);
        assert_bst_successor(root);
    }

    bst_free_all(root);

    return 0;
}

#if __INCLUDE_LEVEL__ == 0
int main(int argc, char *argv[]) { bst_main(argc, argv); }
#endif
