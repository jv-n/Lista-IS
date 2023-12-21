#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define N 4

int sum_linhas[N]; int sum_colunas[N]; int sum_Diag[2]; 
int mat[N][N];

void* soma_linhas();
void* soma_colunas();
void* soma_diagonais();

int main()
{
    /*
    
    A implementação desse código usa threads para calcular as somas de cada parte da matriz e os coloca num vetor
    N threads para as colunas, N threads para as linhas, 1 thread para cada diagonal (principal e secundária).
    Dois processos (nesse caso threads em execução) não estão ao mesmo tempo em uma região crítica, visto que 
    cada thread só acessa o vetor de somas correspondente a sua threadid.

    */

    for(int j = 0; j<N; j++)
    {
        sum_linhas[j] = 0; sum_colunas[j] = 0; //zera os vetores
        if (j<2)
        {
            sum_Diag[j] = 0;
        }
        printf("Insira os %d valores da linha %d:\n", N, j+1);
        for(int k = 0; k<N; k++)
        {
            scanf("%d", &mat[j][k]);
        }
    }

    pthread_t threads_linhas[N]; 
    pthread_t threads_colunas[N];
    pthread_t threads_diagonais[2];
    int*ids[N+N+2];

    int x = 0;
    for(x = 0; x<N; x++)
    {
        ids[x] = (int*) malloc(sizeof(int)); // aloca espaço para os ponteiros de id
        *ids[x] = x; //define o ponteiro de ids das threads das linhas
  	    pthread_create(&threads_linhas[x], NULL, soma_linhas,(void*)ids[x]); //cria e define a função das threads das linhas

        ids[x+N] = (int*) malloc(sizeof(int));
        *ids[x+N] = x+N; //ponteiro de ids das threads das colunas
        pthread_create(&threads_colunas[x+N], NULL, soma_colunas, (void*)ids[x+N]); //cria e define a função das threads das linhas
    }

    for(int i = 0; i<N; i++)
    {
  	    pthread_join(threads_linhas[i], NULL); // termina as threads
        pthread_join(threads_colunas[i+N], NULL);
        x++; 
    }

    for(int i = x; i<N+N+2; i++)
    {
         ids[i] = (int*) malloc(sizeof(int));
        *ids[i] = i;
  	    pthread_create(&threads_diagonais[i], NULL, soma_diagonais,(void*)ids[i]); //calcula a soma das diagonais
    }

    for (int i = x; i<N+N+2; i++)
    {
        pthread_join(threads_diagonais[i], NULL); // termina as threads das diagonais
    }

    int magic = 0;

    if(sum_Diag[0] == sum_Diag[1]) //se as diagonais n tiverem a mesma soma, a matriz não é quadrado mágico
    {
        int i = 1;
        while (i<N)
        {
            if(sum_colunas[i]!=sum_colunas[0])
            {
                break;
            }
            if(sum_linhas[i]!=sum_linhas[0])
            {
                break;
            }
            i++;
            if(i==N) magic = 1;
        }
    }


    if(!magic) printf("Nao eh magica\n");
    else printf("Eh magica\n");

    pthread_exit(NULL);

}

void* soma_linhas(void* threadid)
{
    int idx = *((int*)threadid);
    int o = 0;
    for(int i = 0; i<N; i++)
    {
        o+=mat[idx][i];
    }

    sum_linhas[idx] = o;

    pthread_exit(NULL);
}
void* soma_colunas(void* threadid)
{
    int idx = *((int*)threadid);
    int o = 0;
    for(int i = 0; i<N; i++)
    {
        o+=mat[i][idx];
    }
    sum_colunas[idx] = o;

    pthread_exit(NULL);
}
int sum_main()
{
    int o = 0;
    for (int i = 0; i<N; i++)
    {
        o+= mat[i][i];
    }
    return o;
}
int sum_alt()
{
    int m = 0;
    for(int i = 0; i<N; i++)
    {
        m += mat[i][N-1-i];
    }
    return m;
}
void* soma_diagonais(void* threadid)
{
    int idx = (*((int*)threadid)) - (N+N);
    int result;
    if(!idx) result = sum_main();
    else result = sum_alt();

    sum_Diag[idx] = result;

    pthread_exit(NULL);
}