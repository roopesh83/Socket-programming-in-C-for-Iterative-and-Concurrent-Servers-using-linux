#include <sys/time.h>

// C program to find the execution time of code
static struct timeval tm1;

static inline void start_timer()
{
    gettimeofday(&tm1, NULL);
}

static inline void stop_timer(char text[],FILE *fp)
{
    struct timeval tm2;
    gettimeofday(&tm2, NULL);

    unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
    fprintf(fp,"%s %llu ms\n", text,t);

}