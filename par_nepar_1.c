// Osmislite i implementirajte sinkronizaciju pristupa dijeljenoj memoriji,
// tako da se pri svakom pokretanju programa dobije isti i očekivani rezultat.
// Sinhronizaciju realizirajte na način da se zaključava pristup samo onoj
// varijabli koja se u tom trenutku koristi.

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

pthread_t thr_counter[8];

unsigned long n_odd = 0;
unsigned long n_even = 0;

// 2 mutexa jer se mora zakljucavat samo
// varijabla koja se koristi.
// Da je samo jedan mutex, svako zakljucavanje
// bi zakljucavalo obe varijable
pthread_mutex_t mtx_even;
pthread_mutex_t mtx_odd;

void *counter(void *arg)
{
    int n;
    int *c = (int *)arg;
    double d;

    for (int k = 0; k < *c; k++)
    {
        n = rand() % 100;
        for (int j = 0; j < n; j++)
        {
            d = sqrt((double)j);
        }
        if (n % 2 == 0)
        {
            pthread_mutex_lock(&mtx_even);
            n_even++;
            pthread_mutex_unlock(&mtx_even);
        }
        else
        {
            pthread_mutex_lock(&mtx_odd);
            n_odd++;
            pthread_mutex_unlock(&mtx_odd);
        }
    }

    pthread_exit(NULL);
}

int main()
{
    int cnt, k;
    srand(time(NULL));

    pthread_mutex_init(&mtx_even, NULL);
    pthread_mutex_init(&mtx_odd, NULL);

    for (k = 0; k < 8; k++)
    {
        cnt = (k + 1) * 1000;
        pthread_create(&thr_counter[k], NULL, counter, (void *)&cnt);
    }

    for (k = 0; k < 8; k++)
    {
        pthread_join(thr_counter[k], NULL);
    }

    pthread_mutex_destroy(&mtx_even);
    pthread_mutex_destroy(&mtx_odd);

    printf("Broj parnih:   \t %lu\n", n_even);
    printf("Broj neparnih: \t %lu\n", n_odd);
    printf("Ukupno:        \t %lu\n", n_odd + n_even);
    return 0;
}
