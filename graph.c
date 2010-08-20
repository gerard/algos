#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N_VERTEX    100
#define N_EDGES     200

struct edge {
    int vertex;
    int weight;
    struct edge *next;
};

struct vertex {
    struct edge *first;
};

struct vertex graph_vertex[N_VERTEX];
struct edge graph_edges[N_EDGES];

int graph_add_edge(struct edge *e, int v)
{
    struct edge *el_ptr = graph_vertex[v].first;

    if (!el_ptr) {
        graph_vertex[v].first = e;
    } else {
        while (el_ptr->next) el_ptr = el_ptr->next;
        el_ptr->next = e;
    }
}

int main(int argc, char *argv[])
{
    int i;

    struct timeval tv;
    memset(&tv, 0, sizeof(struct timeval));
    gettimeofday(&tv);
    srandom(tv.tv_usec);

    for (i = 0; i < N_EDGES; i++) {
        int source = random() % N_VERTEX;
        int dest = random() % N_VERTEX;
        int w = random() % 16 + 1;

        if (source == dest) dest = ~dest;

        graph_edges[i].vertex = dest;
        graph_edges[i].weight = w;
        graph_edges[i].next = NULL;
        graph_add_edge(&graph_edges[i], source);
    }

    return 0;
}
