// Promijenite program na način da ne koristite globalne varijable.

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define N_THREADS 8

/* ***********************************
 * pthread_t thr_counter[N_THREADS];
 *
 * unsigned long n_odd = 0;
 * unsigned long n_even = 0;
 *
 * pthread_mutex_t mtx_even;
 * pthread_mutex_t mtx_odd;
 ************************************* */

// Struktura koju saljemo threadu
typedef struct
{
    unsigned long *pN_odd;
    unsigned long *pN_even;
    pthread_mutex_t *pMtx_even;
    pthread_mutex_t *pMtx_odd;
    int counter;
} thread_params_t;

void *counter(void *arg)
{
    thread_params_t *pParams = (thread_params_t *)arg;

    int n;
    int c = pParams->counter;
    double d;

    for (int k = 0; k < c; k++)
    {
        n = rand() % 100;
        for (int j = 0; j < n; j++)
        {
            d = sqrt((double)j);
        }
        if (n % 2 == 0)
        {
            pthread_mutex_lock(pParams->pMtx_even);
            *(pParams->pN_even) += 1;
            pthread_mutex_unlock(pParams->pMtx_even);
        }
        else
        {
            pthread_mutex_lock(pParams->pMtx_odd);
            *(pParams->pN_odd) += 1;
            pthread_mutex_unlock(pParams->pMtx_odd);
        }
    }

    pthread_exit(NULL);
}

int main()
{
    int cnt, k;
    srand(time(NULL));

    pthread_t thr_counter[N_THREADS];

    // Deklaracija dijeljenih varijabli
    unsigned long *pN_odd = malloc(sizeof(unsigned long));
    unsigned long *pN_even = malloc(sizeof(unsigned long));

    pthread_mutex_t *pMtx_even = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_t *pMtx_odd = malloc(sizeof(pthread_mutex_t));

    // Inicijalizacija dijeljenih varijabli
    *pN_odd = 0;
    *pN_even = 0;
    pthread_mutex_init(pMtx_even, NULL);
    pthread_mutex_init(pMtx_odd, NULL);

    // Spremnik svih POKAZIVACA na thread_params_t strukture
    // tako da ih mozemo kasnije delaocirat
    thread_params_t *arrThreadParams[N_THREADS];

    for (k = 0; k < N_THREADS; k++)
    {
        cnt = (k + 1) * 1000;

        arrThreadParams[k] = malloc(sizeof(thread_params_t));

        *(arrThreadParams[k]) = (thread_params_t){
            .pN_odd = pN_odd,
            .pN_even = pN_even,
            .pMtx_even = pMtx_even,
            .pMtx_odd = pMtx_odd,
            .counter = cnt};

        pthread_create(&thr_counter[k], NULL, counter, (void *)arrThreadParams[k]);
    }

    for (k = 0; k < N_THREADS; k++)
    {
        pthread_join(thr_counter[k], NULL);
        // Dealokacija thread_params_t struktura
        free(arrThreadParams[k]);
    }

    // Deinicijalizacija dijeljenih varijabli
    pthread_mutex_destroy(pMtx_even);
    pthread_mutex_destroy(pMtx_odd);

    printf("Broj parnih:   \t %lu\n", *pN_even);
    printf("Broj neparnih: \t %lu\n", *pN_odd);
    printf("Ukupno:        \t %lu\n", *pN_odd + *pN_even);

    // Dealokacija dijeljenih varijabli
    free(pN_even);
    free(pN_odd);
    free(pMtx_even);
    free(pMtx_odd);

    return 0;
}
