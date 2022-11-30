#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

pthread_t thr_counter[8];

unsigned long n_odd = 0;
unsigned long n_even = 0;

void *counter(void *arg)
{
    int n;
    int *c = (int *)arg;
    double d;

    for (int k = 0; k < *c; k++)
    {
        n = rand() % 100;
        for (int j = 0; j < n; j++)
            d = sqrt((double)j);
        if (n % 2 == 0)
            n_even++;
        else
            n_odd++;
    }

    pthread_exit(NULL);
}

int main()
{
    int cnt, k;
    srand(time(NULL));
    for (k = 0; k < 8; k++)
    {
        cnt = (k + 1) * 1000;
        pthread_create(&thr_counter[k], NULL, counter, (void *)&cnt);
    }

    for (k = 0; k < 8; k++)
    {
        pthread_join(thr_counter[k], NULL);
    }

    printf("Broj parnih:   \t %lu\n", n_even);
    printf("Broj neparnih: \t %lu\n", n_odd);
    printf("Ukupno:        \t %lu\n", n_odd + n_even);
    return 0;
}
