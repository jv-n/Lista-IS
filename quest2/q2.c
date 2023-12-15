#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct arq
{
    int index;
    int read;
} arq;


arq* arq_buffer = NULL;
int* contadores = NULL;

pthread_mutex_t* mutex;
void* leitura();

int num_arq, num_threads, produtos;

int main(){

    printf("Quantos arquivos serao lidos?:\n");
    scanf("%d", &num_arq);
    printf("Quantas threads?:\n");
    scanf("%d", &num_threads);
    printf("Quantos tipos de produtos?:\n");
    scanf("%d", &produtos);

    arq_buffer = (arq*) malloc(sizeof(arq)*num_arq);
    for (int j = 0; j<num_arq; j++)
    {
        arq_buffer[j].index = j;
        arq_buffer[j].read = 0;
    }

    contadores = (int*) calloc(produtos, sizeof(int));
    
    pthread_t reader[num_threads];
    int* ids[num_threads];

    int i = 0;
	for(i = 0; i < num_threads; i++) 
    {
	    ids[i] = (int*) malloc(sizeof(int)); 
		*ids[i] = i;
  	    pthread_create(&reader[i],NULL, leitura,(void *) ids[i]);  
    }

    pthread_exit(NULL);
}

int read_file(int i)
{
    FILE* dados;
    char name[100];
    char linha[100];
    sprintf(name, "%d", i);
    strcat(name, ".txt");
    dados = fopen(name, "r");
    if(dados == NULL)
    {
        printf("Erro ou arquivo nao existe");
        exit(1);
    }
    int control = 0;
    while(control<=produtos);
    {
        fscanf(dados, "%100[^\n]\n", linha);

    }
    fclose(dados);
    arq_buffer[i].read = 1;

}


void* leitura(void* threadid)
{
    int v;
    printf("Thread %d iniciada, lendo arquivo %d: ", *((int*) threadid), *((int*) threadid)+1);
    read_file(*((int*)threadid)+1);
    if(num_threads!=num_arq){
        for(int i = num_threads; i<num_arq; i++)
        {
            v = read_file(i);
            printf("Thread %d leu arquivo %d ", *((int*) threadid), v);
        }
        printf("Thread %d terminou: ", *((int*) threadid));
    }
    pthread_exit(NULL);
}