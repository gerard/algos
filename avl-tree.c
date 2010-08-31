#define bst_fixup_insert avl_fixup_insert
#define bst_fixup_delete avl_fixup_delete

struct node;
struct node *avl_fixup_insert(struct node *);
struct node *avl_fixup_delete(struct node *);

#include "bst.c"
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int avl_node_getdepth(struct node *n)
{
    return n ? n->self_balancing_data : 0;
}

void avl_node_setdepth(struct node *n)
{
    int ldepth = avl_node_getdepth(n->left);
    int rdepth = avl_node_getdepth(n->right);

    n->self_balancing_data = MAX(ldepth, rdepth) + 1;
}

struct node *avl_rotate_left(struct node *n)
{
    struct node *ret = bst_rotate_left(n);
    avl_node_setdepth(ret->left);
    avl_node_setdepth(ret);
    return ret;
}

struct node *avl_rotate_right(struct node *n)
{
    struct node *ret = bst_rotate_right(n);
    avl_node_setdepth(ret->right);
    avl_node_setdepth(ret);
    return ret;
}

struct node *avl_fixup(struct node *n)
{
    int ldepth = avl_node_getdepth(n->left);
    int rdepth = avl_node_getdepth(n->right);

    if (ldepth - rdepth > 1) {
        int lldepth = avl_node_getdepth(n->left->left);
        int lrdepth = avl_node_getdepth(n->left->right);

        if (lldepth < lrdepth) {
            assert(n->left->right);
            n->left = avl_rotate_left(n->left);
        }

        n = avl_rotate_right(n);
    }

    if (rdepth - ldepth > 1) {
        int rldepth = avl_node_getdepth(n->right->left);
        int rrdepth = avl_node_getdepth(n->right->right);

        if (rrdepth < rldepth) {
            assert(n->right->left);
            n->right = avl_rotate_right(n->right);
        }

        n = avl_rotate_left(n);
    }

    avl_node_setdepth(n);
    return n;
}

struct node *avl_fixup_insert(struct node *n)
{
    return avl_fixup(n);
}

struct node *avl_fixup_delete(struct node *n)
{
    return avl_fixup(n);
}

#if __INCLUDE_LEVEL__ == 0
int main(int argc, char *argv[])
{
    struct node *check_depth(struct node *n)
    {
        int ldepth = avl_node_getdepth(n->left);
        int rdepth = avl_node_getdepth(n->right);

        assert(avl_node_getdepth(n) == MAX(ldepth, rdepth) + 1);
        assert(abs(ldepth - rdepth) < 2);

        return n;
    }

    struct node *root = NULL;
    int values[N_ELEMS];
    int i;

    struct timeval tv;
    memset(&tv, 0, sizeof(struct timeval));
    gettimeofday(&tv);
    srandom(tv.tv_usec);

    memset(&stats, 0, sizeof(struct bst_stats));
    for (i = 0; i < N_ELEMS; i++) {
        values[i] = random() % 256;
        root = bst_insert(root, values[i]);
    }

    for (i = 0; i < N_DELETES; i++) {
        root = bst_delete(root, values[random() % N_ELEMS]);

        assert(bst_size(root) == (stats.total_mallocs - stats.total_frees));
        assert_bst_properties(root);
        assert_bst_successor(root);
    }

    while (root) {
        bst_postorder(root, check_depth);
        root = bst_delete(root, root->v);
    }

    return 0;
}
#endif
