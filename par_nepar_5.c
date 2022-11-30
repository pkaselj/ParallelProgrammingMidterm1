// Može li se program realizirati na način da ne koristite mutexe,
// a da pri tom rezultat pokretanja programa (ukupan broj generiranih
// brojeva) uvijek bude isti i očekivani rezultat. Pokušajte riješiti
// zadatak bez korištenja mutexa

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define N_THREADS 8

// Struktura koju saljemo threadu
typedef struct
{
    unsigned long *pN_odd;
    unsigned long *pN_even;
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
            *(pParams->pN_even) += 1;
        }
        else
        {
            *(pParams->pN_odd) += 1;
        }
    }

    pthread_exit(NULL);
}

int main()
{
    int cnt, k;
    srand(time(NULL));

    pthread_t thr_counter[N_THREADS];

    /* ******************************************************
     * Vise nema dijeljenih varijabli, sada svaki thread
     * dobiva svoju varijablu kojoj pristupa, a kasnije se
     * te varijable sve zbroje u glavnu varijablu koja je lokalna
     * main funkciji:
     * * STARO:
     *      unsigned long *pN_odd = malloc(sizeof(unsigned long));
     *      unsigned long *pN_even = malloc(sizeof(unsigned long));
     ******************************************************** */

    // NOVO:
    unsigned long n_odd = 0;
    unsigned long n_even = 0;

    // Spremnik svih POKAZIVACA na thread_params_t strukture
    // tako da ih mozemo kasnije delaocirat
    thread_params_t *arrThreadParams[N_THREADS];

    for (k = 0; k < N_THREADS; k++)
    {
        cnt = (k + 1) * 1000;

        arrThreadParams[k] = malloc(sizeof(thread_params_t));

        *(arrThreadParams[k]) = (thread_params_t){
            .pN_odd = malloc(sizeof(unsigned long)),  /* Vlastita alocirana mem. lokacija */
            .pN_even = malloc(sizeof(unsigned long)), /* Vlastita alocirana mem. lokacija */
            .counter = cnt};

        pthread_create(&thr_counter[k], NULL, counter, (void *)arrThreadParams[k]);
    }

    for (k = 0; k < N_THREADS; k++)
    {
        pthread_join(thr_counter[k], NULL);

        // Izvlacenje rezultata iz threada:
        n_even += *(arrThreadParams[k]->pN_even);
        n_odd += *(arrThreadParams[k]->pN_odd);

        // Dealokacija varijabli kojih je
        // arrThreadParams[k] vlasnik:
        free(arrThreadParams[k]->pN_even);
        free(arrThreadParams[k]->pN_odd);

        // Dealokacija thread_params_t struktura
        free(arrThreadParams[k]);
    }

    printf("Broj parnih:   \t %lu\n", n_even);
    printf("Broj neparnih: \t %lu\n", n_odd);
    printf("Ukupno:        \t %lu\n", n_odd + n_even);

    return 0;
}
