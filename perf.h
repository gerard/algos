#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#define PERF_START()            __perf_start(__FILE__, __LINE__, __func__)
#define PERF_STOP()             __perf_stop(__FILE__, __LINE__, __func__)
#define PERF_PROBLEM_SIZE(x)    __perf_problem_size(__func__, x)

struct perf_entry {
    char *file;
    char *func;
    int problem_size;
    int start_line;
    int stop_line;
    int start_hits;
    int stop_hits;
    FILE *gnuplot_fd;
    uint64_t usecs;
    uint64_t usecs_div_size;
    struct timeval current;
};

#define PERF_ARRAY_SIZE 100
struct perf_entry __perf_array[PERF_ARRAY_SIZE];
int __perf_len;

uint64_t timeval_sub(struct timeval a, struct timeval b)
{
    uint64_t ret = (a.tv_sec - b.tv_sec) * 1000000;
    ret += (a.tv_usec - b.tv_usec);

    return ret;
}

struct perf_entry *__perf_find(const char *func)
{
    int i = 0;

    for (i = 0; i < __perf_len; i++) {
        struct perf_entry *entry = &__perf_array[i];
        if (!strcmp(func, entry->func)) {
            return entry;
        }
    }

    return NULL;
}

void __perf_start(char *file, int line, const char *func)
{
    struct perf_entry *entry = __perf_find(func);

    if (!entry) {
        entry = &__perf_array[__perf_len++];

        entry->file = strdup(file);
        entry->func = strdup(func);
        entry->start_line = line;
        entry->start_hits++;
    } else {
        assert(entry->start_hits == entry->stop_hits);
        assert(entry->start_line == line);

        entry->start_hits++;
    }

    gettimeofday(&entry->current, NULL);
}

void __perf_stop(char *file, int line, const char *func)
{
    struct timeval now_tv;
    int i;

    gettimeofday(&now_tv, NULL);
    
    struct perf_entry *entry = __perf_find(func);
    assert(entry);

    if (!entry->stop_line) entry->stop_line = line;

    entry->stop_hits++;
    entry->usecs += timeval_sub(now_tv, entry->current);
    if (entry->problem_size) {
        entry->usecs_div_size += entry->usecs / entry->problem_size;
    }
}

void __perf_problem_size(const char *func, uint64_t size)
{
    struct perf_entry *entry = __perf_find(func);
    assert(entry);

    entry->problem_size = size;
}

void PERF_CLEAR(void)
{
    int i;

    for (i = 0; i < __perf_len; i++) {
        struct perf_entry *entry = &__perf_array[i];

        entry->problem_size = 0;
        entry->start_hits = 0;
        entry->stop_hits = 0;
        entry->usecs = 0;
        entry->usecs_div_size = 0;
        memset(&entry->current, 0, sizeof(struct timeval));
    }
}

void PERF_PRINT(FILE *fd, char *s, char *s_entry)
{
    int i;

    if (s) fprintf(fd, "%s\n", s);
    for (i = 0; i < __perf_len; i++) {
        struct perf_entry *entry = &__perf_array[i];
        fprintf(fd, "%s%s:%s:%d-%d => %llu\n", s_entry
                                             , entry->file
                                             , entry->func
                                             , entry->start_line
                                             , entry->stop_line
                                             , entry->usecs
                                             );
    }
}

void PERF_GNUPLOT_PRINT(int n, char *func)
{
    struct perf_entry *entry = __perf_find(func);

    if (!entry->gnuplot_fd) {
        char gnuplot_fname[200];
        sprintf(gnuplot_fname, "%s.data", func);
        FILE *fd = fopen(gnuplot_fname, "w");
        assert(fd);

        entry->gnuplot_fd = fd;
    }

    fprintf(entry->gnuplot_fd, "%d %llu\n", n, entry ? entry->usecs : 0);
    fflush(entry->gnuplot_fd);
}
