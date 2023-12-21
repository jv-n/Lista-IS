//apenas teste

/*#include <stdio.h>
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
}*/
#define _XOPEN_SOURCE 600
/*#include <stdio.h>
#include <pthread.h>

#define N 2  // Tamanho do sistema de equações
#define NUM_THREADS 4  // Número de threads

double A[N][N] = {{2, 1}, {5, 7}};  // Matriz do sistema
double b[N] = {11, 13};  // Vetor b
double x[N] = {1, 1};  // Valores iniciais das incógnitas

pthread_barrier_t barrier;  // Barreira para sincronização

// Função para resolver uma parte das incógnitas
void *solve_part(void *arg) {
    long thread_id = (long)arg;

    for (int k = 0; k < 10; k++) {
        // Cálculo das incógnitas associadas a esta thread
        for (int i = thread_id; i < N; i += NUM_THREADS) {
            double sum = 0.0;
            for (int j = 0; j < N; j++) {
                if (j != i) {
                    sum += A[i][j] * x[j];
                }
            }
            double x_next = (b[i] - sum) / A[i][i];

            // TODO: Atualize x[i] para x_next de forma segura
        }

        pthread_barrier_wait(&barrier);  // Barreira para sincronização

        // TODO: Atualização dos valores de x para a próxima iteração
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    // Criação das threads
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, solve_part, (void *)i);
    }

    // Join das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}
*/
#include <stdio.h>
#include <pthread.h>

#define N 2  // Tamanho do sistema de equações
#define NUM_THREADS 2  // Número de threads

double A[N][N] = {{2, 1}, {5, 7}};  // Matriz do sistema
double b[N] = {11, 13};  // Vetor b
double x[N] = {1, 1};  // Valores iniciais das incógnitas

pthread_barrier_t barrier;  // Barreira para sincronização

// Função para resolver uma parte das incógnitas
void *solve_part(void *arg) {
    long thread_id = (long)arg;

    for (int k = 0; k < 5; k++) { // Reduzi o número de iterações para facilitar a visualização
        // Cálculo das incógnitas associadas a esta thread
        for (int i = thread_id; i < N; i += NUM_THREADS) {
            double sum = 0.0;
            for (int j = 0; j < N; j++) {
                if (j != i) {
                    sum += A[i][j] * x[j];
                }
            }
            double x_next = (b[i] - sum) / A[i][i];

            // Atualização dos valores de x para a próxima iteração
            x[i] = x_next;
        }

        pthread_barrier_wait(&barrier);  // Barreira para sincronização

        // Impressão dos valores das incógnitas após cada iteração
        printf("Iteração %d - Thread %ld: x1 = %.4f, x2 = %.4f\n", k + 1, thread_id, x[0], x[1]);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    // Criação das threads
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, solve_part, (void *)i);
    }

    // Join das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}
