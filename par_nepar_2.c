// Testirajte svoj kod i provjerite rezultira li pokretanje programa
// svaki put istim i očekivanim rezultatom. Ukoliko zbroj pojavljivanja
// parnih i neparnih brojeva i dalje nije jednak očekivanom
// ukupnom broju svih brojeva, proučite kod i pokušajte uočiti i ispraviti grešku.

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <assert.h>

pthread_t thr_counter[8];

unsigned long n_odd = 0;
unsigned long n_even = 0;

pthread_mutex_t mtx_even;
pthread_mutex_t mtx_odd;

void *counter(void *arg)
{
    int n;
    int c = (int)arg; /* NIJE VISE ADRESA */
    double d;

    for (int k = 0; k < c; k++) /* NIJE VISE ADRESA */
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
        // k-ti thread bi trebao generirati (k + 1) * 1000 brojeva
        cnt = (k + 1) * 1000;

        /* ************************************************************
         * Prema starom kodu, salje se pokazivac na lokalnu varijablu
         * cnt, koja stalno mijenja vrijednost, a na kraju se zadrzi na
         * k = 7 (zadnji k u for petlji) odnosno cnt = 8000, tako da svi
         * threadovi broje do 8000
         * // pthread_create(&thr_counter[k], NULL, counter, (void *)&cnt);
         *************************************************************** */

        // Jedno od rjesenja je poslat trenutnu vrijednost cnt variable
        // preko argumenta u pthread_create. Ovo nije preporuceno, ali je
        // moguce jer vrlo vjerojatno vrijedi sizeof(void*) >= sizeof(int)
        /* ***********************************************************
         * Moze provjeriti preko:
         */
        assert(sizeof(void *) >= sizeof(int));
        /*
         * Ako nije istina, program puca u gornjoj liniji sa porukom:
         *   par_nepar_2: par_nepar_2.c:77: main: Assertion `sizeof(void*) >= sizeof(int)' failed.
         *   Aborted (core dumped)
         * ************************************************************ */

        // Inace bi trebalo ili preko globalnog ili alociranog niza integera

        pthread_create(&thr_counter[k], NULL, counter, (void *)cnt); /* NIJE VISE ADRESA */
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
