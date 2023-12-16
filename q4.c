#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define N 5

int sum_linhas[N]; int sum_colunas[N]; int sum_Diag[2]; 
int mat[N][N];

pthread_mutex_t* lines;
pthread_mutex_t* columns;

void* soma_linhas();
void* soma_colunas();

int main()
{

    for(int j = 0; j<N; j++)
    {
        sum_linhas[j] = 0; sum_colunas[j] = 0;
        printf("Insira os %d valores da linha %d:\n", N, j);
        for(int k = 0; k<N; k++)
        {
            scanf("%d", mat[j][k]);
        }
    }

    lines = (pthread_mutex_t*) malloc(N*sizeof(pthread_mutex_t));
    columns = (pthread_mutex_t*) malloc(N*sizeof(pthread_mutex_t));
    
    for(int j = 0; j<N; j++)
    {
        pthread_mutex_init(&lines[j], NULL);
        pthread_mutex_init(&columns[j], NULL);
    }

    pthread_t threads_linhas[N];
    pthread_t threads_colunas[N];
    pthread_t threads_diagonais[2];
    int*ids[N+N+2];

    for(int i = 0; i<N; i++)
    {
        ids[i] = (int*) malloc(sizeof(int));
        *ids[i] = i;
  	    pthread_create(&threads_linhas[i], NULL, soma_linhas,(void*)ids[i]);

        ids[i+N] = (int*) malic(sizeof(int));
        *ids[i+N] = i+N;
        pthread_create(&threads_colunas[i+N], NULL, soma_colunas, (void*)ids[i+N]); 
    }
  
    for(int i = N+N; i<N+N+2; i++)
    {
         ids[i] = (int*) malloc(sizeof(int));
        *ids[i] = i;
  	    pthread_create(&threads_diagonais[i], NULL, soma_diagonais,(void*)ids[i]);
    }

    for(int i = 0; i<N; i++)
    {
  	    pthread_join(threads_linhas[i], NULL);
        pthread_join(threads_colunas[i+N], NULL); 
    }

    int magic = 0;
    if(sum_Diag[0]==sum_Diag[1])
    {
        int i = 1;
        while (i<N)
        {
            if(sum_colunas[i]!=sum_colunas[0])
            {
                break;
            }
            if(sum_linhas[i]!=sum_colunas[0])
            {
                break;
            }
            i++;
            if(i==N) magic = 1;
        }
    }

    if(!magic) printf("Nao eh magica");
    else printf("Eh magica");

    pthread_exit(NULL);

}

void* soma_linhas(void* threadid)
{
    int idx = *((int*)threadid);
    for(int i = 0; i<N; i++)
    {
        sum_linhas[idx]+=mat[idx][i];
    }

    pthread_exit(NULL);
}
void* soma_colunas(void* threadid)
{
    int idx = *((int*)threadid);
    for(int i = 0; i<N; i++)
    {
        sum_linhas[idx]+=mat[i][idx];
    }

    pthread_exit(NULL);
}
void sum_main(int x)
{
    for (int i = 0; i = N; i++)
    {
        sum_Diag[x]+= mat[i][i];
    }
}
void sum_alt(int x)
{
    for(int i = 0; i = N; i++)
    {
        sum_Diag[x]+= mat[i][N-1-i];
    }
}
void* soma_diagonais(void* threadid)
{
    int idx = *((int*)threadid);
    if(!idx) sum_main(idx);
    else sum_alt(idx);

    pthread_exit(NULL);
}