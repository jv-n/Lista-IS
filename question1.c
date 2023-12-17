#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>

#define N 5
#define M 5
#define Q 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;

typedef struct Pedido { //Armazena os pedidos
    struct Pedido* proximo;
} Pedido;

Pedido* fila = NULL; // Inicializa a fila como vazia
int tamanho_fila = 0; // Inicializa o tamanho da fila como 0

void* funcao_robo(void* arg);
void* funcao_mesa(void* arg);

int main() {

  pthread_t robos[N];
  pthread_t mesas[M];

  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < N; i++) { // Cria a thread
    pthread_create(&robos[i], NULL, funcao_robo, NULL);
  }

  for (int i = 0; i < M; i++) { 
    pthread_create(&mesas[i], NULL, funcao_mesa, NULL);
  }

  for (int i = 0; i < N; i++) { //Bloqueia a thread que a chamou até que ela termine
    pthread_join(robos[i], NULL);
  }

  for (int i = 0; i < M; i++) {
    pthread_join(mesas[i], NULL);
  }
}

void* funcao_robo(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

          while (tamanho_fila == 0) { // Fica aguardando um pedido enquanto a fila esta vazia
              pthread_cond_wait(&fill, &mutex);
          }

          // Remove um pedido da fila
          Pedido* pedido = fila;
          fila = fila->proximo;
          tamanho_fila--;
          if (tamanho_fila < Q) pthread_cond_signal(&empty); //Avisa aos clientes que tem vaga na fila de pedidos
          printf("pedido consumido, resta: %d\n", tamanho_fila);

        pthread_mutex_unlock(&mutex);
    }
}

void* funcao_mesa(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

          while (tamanho_fila == Q) { // Espera pra colocar um pedido na fila se ela estiver cheia
              pthread_cond_wait(&empty, &mutex);
          }

          // Adiciona um pedido à fila
          Pedido* pedido = malloc(sizeof(Pedido));
          pedido->proximo = fila;
          fila = pedido;
          tamanho_fila++;
          if (tamanho_fila > 0) pthread_cond_signal(&fill); // Avisa aos robôs que há um novo pedido
          printf("pedidos: %d\n", tamanho_fila);

        pthread_mutex_unlock(&mutex);
    }
}
