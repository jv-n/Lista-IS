//apenas teste

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 2
#define P 4

double A[N][N] = {{2, 1}, {5, 7}};
double b[N] = {11, 13};
double x[N] = {1, 1};

pthread_t threads[P];
pthread_barrier_t barrier;

void *jacobi_thread(void *arg) {
    int id = *(int *)arg;
    int i, j;
    double sigma;

    for (i = id; i < N; i += P) {
        while (1) {
            sigma = 0;
            for (j = 0; j < N; j++) {
                if (j != i) {
                    sigma += A[i][j] * x[j];
                }
            }
            x[i] = (b[i] - sigma) / A[i][i];
            pthread_barrier_wait(&barrier);
            // check for convergence
            // ...
            pthread_barrier_wait(&barrier);
        }
    }

    return NULL;
}

int main() {
    int i, id[P];

    pthread_barrier_init(&barrier, NULL, P);

    for (i = 0; i < P; i++) {
        id[i] = i;
        pthread_create(&threads[i], NULL, jacobi_thread, &id[i]);
    }

    for (i = 0; i < P; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}