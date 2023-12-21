#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<unistd.h>

void* trem();

int intersecao[5] = {0,0,0,0,0};

pthread_cond_t intersecao_ocupd[5];
pthread_cond_t intersecao_livre[5];

pthread_mutex_t mutex[5];

int main(){

    pthread_t threads[10];
    int* ids[10];

    for(int j = 0; j<5; j++)
    {
        pthread_mutex_init(&mutex[j], NULL);
        pthread_cond_init(&intersecao_ocupd[j], NULL);
        pthread_cond_init(&intersecao_livre[j], NULL); 
    }

    for(int i = 0; i<10; i++)
    {
        ids[i] = (int*) malloc(sizeof(int));
        *ids[i] = i;
        pthread_create(&threads[i], NULL, trem, (void*)ids[i]);
    }

    for(int i = 0; i<10; i++)
    {
        pthread_join(threads[i], NULL);
    }
    for(int j = 0; j<5; j++)
    {
        pthread_mutex_destroy(&mutex[j]);
        pthread_cond_destroy(&intersecao_ocupd[j]);
        pthread_cond_destroy(&intersecao_livre[j]); 
    }

    pthread_exit(NULL);

}

void entrando(int cont, int i)
{
    pthread_mutex_lock(&mutex[cont]);
        printf("Trem %d tentando entrar na intersecao %d \n", i, cont);
        while(intersecao[cont] == 2)//se ha 2 trens em 1 intersecao, espera
        {
            printf("Trem %d esperando entrar na intersecao %d \n", i, cont);
            pthread_cond_wait(&intersecao_ocupd[cont], &mutex[cont]);
        }
        intersecao[cont]++;
        
    pthread_mutex_unlock(&mutex[cont]);

}

void saindo(int cont, int i)
{
    pthread_mutex_lock(&mutex[cont]);
        intersecao[cont]--;
        if(intersecao[cont]<2) pthread_cond_signal(&intersecao_ocupd[cont]); //sinaliza que um trem saiu para quem estiver esperando
        printf("Trem %d saiu de %d, %d trens na intersecao \n", i, cont, intersecao[cont]);
    pthread_mutex_unlock(&mutex[cont]);

}

void* trem(void* threadid)
{
    int cont = 0;
    int i = *((int*)threadid);
    while(1)
    {
        entrando(cont, i); //entra na intersecao
        printf("Trem %d entrou em %d, %d trens na intersecao\n", i, cont, intersecao[cont]);  
        sleep(0.5); // espera 500 ms (0.5s)
        saindo(cont, i); //sai da intersecao
        cont++;
        if(cont == 5) cont = 0; //se chega em 5, volta para o inicio
    }

    pthread_exit(NULL);
}

