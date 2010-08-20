#define bst_fixup_insert avl_fixup_insert
#define bst_fixup_delete avl_fixup_delete

struct node;
struct node *avl_fixup_insert(struct node *);
struct node *avl_fixup_delete(struct node *);

#include "bst.c"
#define MAX(a, b) ((a) < (b) ? (a) : (b))

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

struct node *avl_fixup_insert(struct node *n)
{
    int ldepth = avl_node_getdepth(n->left);
    int rdepth = avl_node_getdepth(n->right);
    avl_node_setdepth(n);

    if (ldepth - rdepth > 1) {
        if (!n->left->left) {
            assert(n->left->right);

            bst_rotate_left(n->left);
        }

        n = bst_rotate_right(n);
        avl_node_setdepth(n->left);
        avl_node_setdepth(n->right);
        avl_node_setdepth(n);
    }

    if (rdepth - ldepth > 1) {
        if (!n->right->right) {
            assert(n->right->left);

            bst_rotate_right(n->right);
        }

        n = bst_rotate_left(n);
        avl_node_setdepth(n->left);
        avl_node_setdepth(n->right);
        avl_node_setdepth(n);
    }

    return n;
}

struct node *avl_fixup_delete(struct node *n)
{
    return n;
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

    bst_main(argc, argv);

    memset(&stats, 0, sizeof(struct bst_stats));
    for (i = 0; i < N_ELEMS; i++) {
        values[i] = random() % 256;
        root = bst_insert(root, values[i]);
    }

    bst_postorder(root, check_depth);
    bst_free_all(root);

    return 0;
}
#endif
