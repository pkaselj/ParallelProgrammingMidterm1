// Promijenite program na način da sve niti prekinu izvršavanje
// kada broj parnih ili neparnih brojeva (što se prvo ostvari)
// postane jednak ili veći od polovine očekivanog ukupnog broja
// generiranih brojeva. Nakon što sve niti završe, glavna nit
// treba ispisati stanje u tom trenutku (broj parnih, broj
// neparnih i ukupan broj generiranih brojeva).

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

// Dodamo makro za broj threadova
// tako da svaki thread moze izracunat ocekivna ukupni broj
// Malo glupo al radi
#define N_THREADS 8

pthread_t thr_counter[N_THREADS];

unsigned long n_odd = 0;
unsigned long n_even = 0;

pthread_mutex_t mtx_even;
pthread_mutex_t mtx_odd;

void *counter(void *arg)
{
    int n;
    int c = (int)arg;
    // Ako imamo N_THREADS threadova, i svaki thread
    // broj do (k+1)*1000 brojeva pocevsi od k = 0(ukljucujuci)
    // do k = N_THREADS - 1 (ukljucujuci), tada je ocekivani broj
    // generiranih brojeva jednak zbroju niza:
    // 1000, 2000, 3000, ... , (N_THREADS) * 1000
    // odnosno prema Gaussu:
    int total_count = 1000 * (N_THREADS) * (N_THREADS + 1) / 2;
    double d;

    for (int k = 0; k < c; k++)
    {
        // Uvjet se provjerava u svakoj iteraciji petlje,
        // bez sinkronizacije nece nikad stat na tocno (total_count / 2)
        if (n_even >= total_count / 2 || n_odd >= total_count / 2)
        {
            pthread_exit(NULL);
        }

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

    for (k = 0; k < N_THREADS; k++)
    {
        cnt = (k + 1) * 1000;
        pthread_create(&thr_counter[k], NULL, counter, (void *)cnt);
    }

    for (k = 0; k < N_THREADS; k++)
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
