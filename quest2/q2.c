#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int* arq_buffer = NULL;
int* contadores = NULL;

pthread_mutex_t* mutex;
pthread_mutex_t* mutex_file;
void* leitura(void* thread_id);

int num_arq, num_threads, produtos;

int main(){

    printf("Quantos arquivos serao lidos?:\n");
    scanf("%d", &num_arq);
    printf("Quantas threads?:\n");
    scanf("%d", &num_threads);
    printf("Quantos tipos de produtos?:\n");
    scanf("%d", &produtos);

    arq_buffer = (int*) calloc(num_arq, sizeof(int));
    contadores = (int*) calloc(produtos, sizeof(int));

    mutex = (pthread_mutex_t*) malloc(produtos*sizeof(pthread_mutex_t));
    mutex_file = (pthread_mutex_t*) malloc(num_arq*sizeof(pthread_mutex_t));
    
    for(int j = 0; j<produtos; j++)
    {
        pthread_mutex_init(&mutex[j], NULL);
    }
    for(int j = 0; j<num_arq; j++)
    {
        pthread_mutex_init(&mutex_file[j], NULL);
    }

    pthread_t reader[num_threads];
    int* ids[num_threads];

	for(int i = 0; i < num_threads; i++) 
    {
        ids[i] = (int*) malloc(sizeof(int));
        *ids[i] = i;
  	    pthread_create(&reader[i], NULL, leitura,(void*)ids[i]); 
    } 
    for(int i = 0; i<num_threads; i++)
        {
            pthread_join(reader[i], NULL);
            printf("Liberando thread %d\n", i);
        }    
    
    printf("O valor final de cada produto foi:\n");
    for(int i = 0; i<produtos; i++)
    {
        printf("Produto tipo %d = %d\n", i+1, contadores[i]);
    }

    pthread_exit(NULL);
}

int read_file(int i)
{
    arq_buffer[i-1] = 1;
    FILE* dados;
    char name[100];
    char linha[100];
    sprintf(name, "%d", i);
    strcat(name, ".txt");
    dados = fopen(name, "r");
    if(dados == NULL)
    {
        printf("Erro ou arquivo nao existe\n");
        exit(1);
    }
    int control = 0;
    while(control<produtos)
    {
        fscanf(dados, "%100[^\n]\n", linha);
        int temp_qtd = atoi(linha);
        
        pthread_mutex_lock(&mutex[control]);
        contadores[control]+=temp_qtd;        
        control++;
        pthread_mutex_unlock(&mutex[control-1]);
    }
    fclose(dados);
    return i;
}

void* leitura(void* threadid)
{
    int v;
    //checa se o ciclo ta vendo certo
    printf("Thread %d iniciada, lendo arquivo %d\n", *((int*) threadid), *((int*) threadid)+1);
    read_file(*((int*)threadid)+1);
    printf("Thread %d terminou arquivo %d\n", *((int*) threadid), *((int*) threadid)+1);

    if(num_threads<num_arq)
    {
        for(int i = num_threads; i<num_arq; i++)
        {
            pthread_mutex_lock(&mutex_file[i]);
            if(!arq_buffer[i])
            {
                arq_buffer[i] = 1;
                pthread_mutex_unlock(&mutex_file[i]);
                printf("Thread %d lendo arquivo %d\n", *((int*) threadid), i+1);
                v = read_file(i+1);
                printf("Thread %d leu arquivo %d\n", *((int*) threadid), v);
            } else pthread_mutex_unlock(&mutex_file[i]);
        }
        printf("Thread %d terminou\n", *((int*) threadid));
    }
    pthread_exit(NULL);
}