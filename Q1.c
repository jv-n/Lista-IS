#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sem_pedidos = PTHREAD_COND_INITIALIZER;
pthread_cond_t fila_cheia = PTHREAD_COND_INITIALIZER;

void* fazer_pedido();
void* pedido_concluido();

int* buffer; int pedidos_na_fila; int fila;

int main ()
{
    int robos, mesas;
    printf("Indique o numero de robos: ");
    scanf("%d", &robos);
    printf("Indique o numero de mesas: ");
    scanf("%d", &mesas);
    printf("Indique o tamanho da fila: ");
    scanf("%d", &fila);

    buffer = (int*) malloc(sizeof(int)*fila);
    int* ids_robos[robos];
    int* ids_disp_mesas[mesas];

    pthread_t garcons_robos[robos];
    pthread_t disp_mesas[mesas];

    for(int i = 0; i<mesas; i++)
    {
        ids_disp_mesas[i] = (int*) malloc(sizeof(int));
        *ids_disp_mesas[i] = i;
        pthread_create(&disp_mesas[i], NULL, fazer_pedido, (void *)ids_disp_mesas[i]);

        if(i<robos)
        {
            ids_robos[i] = (int*) malloc(sizeof(int));
            *ids_robos[i] = i;
            pthread_create(&garcons_robos[i], NULL, pedido_concluido, (void* )ids_robos[i]);
        }

    }

    pthread_exit(NULL);
}

void add_pedido(int i)
{
    pthread_mutex_lock(&mutex);
    while(pedidos_na_fila == fila)
    {
        pthread_cond_wait(&fila_cheia, &mutex);
    }

    int last = fila-1;
}