#define _XOPEN_SOURCE 600
#define N 2
#define P 2
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

double A[N][N]= {{2,1},{5,7}}; // Matriz dos coeficientes   
double B[]={11,13}; // Vetor de termos independentes
//double X[]={1,1}; // Vetor solução      resultado de x1 =0.902...   x2=1.609...    x3=0.463...
double** X_new; // Vetor solução temporário
pthread_barrier_t barrier;
pthread_mutex_t* mutex_uso;
int* buffer_uso;

int num_threads; // Número de threads

double jacobi(int k, int x){

    double retorno, interno; double temp = 0;
    
    for(int j = 0; j<N; j++)
    {
        if(j!=x){
            temp+=A[x][j]*X_new[j][k-1];
        } else temp = temp;
    }
 
    interno = B[x] - temp;
    retorno = (1/A[x][x])*interno;

    return retorno;
}

void *calculate(void *thread_id) {
    int id = *((int *)thread_id);
    if(id<N){
    pthread_mutex_lock(&mutex_uso[id]);
    buffer_uso[id] = 1;
    pthread_mutex_unlock(&mutex_uso[id]);
    
        int iteracao = 1;
        while(iteracao-1<P)
        {
            X_new[id][iteracao] = jacobi(iteracao, id);
            iteracao++;
            pthread_barrier_wait(&barrier);    
        }
        if(num_threads<N)
        {
            iteracao = 1;
            
            for(int i = num_threads; i<N; i++)
            {
                pthread_mutex_lock(&mutex_uso[i]);
                if(!buffer_uso[i])
                {
                    
                    buffer_uso[i] = 1;
                    pthread_mutex_unlock(&mutex_uso[i]);
                    while(iteracao-1<P)
                    {
                        X_new[i][iteracao] = jacobi(iteracao, i);
                        iteracao++;
                        pthread_barrier_wait(&barrier);
                    }
                }pthread_mutex_unlock(&mutex_uso[i]);
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
  
    printf("Qual o numero de threads que irao ser utilizadas?\n");
    scanf("%d",&num_threads);
    
    /*printf("Um sistema é NxN, qual o tamanho N do sistema?: \n");
    scanf("%d", &N);*/

    X_new = (double**) malloc(sizeof(double*)*N);
    buffer_uso = (int*) calloc(N, sizeof(int));
    mutex_uso = (pthread_mutex_t*) malloc(N*sizeof(pthread_mutex_t));

    for(int i = 0; i<N; i++)
    {
        X_new[i] = (double*) malloc(sizeof(double)*(P+1));
        X_new[i][0] = 1;       
        pthread_mutex_init(&mutex_uso[i], NULL); 
    }

    pthread_t threads[num_threads];
    int* thread_ids[num_threads];
    
    if(num_threads<N)
        pthread_barrier_init(&barrier,NULL,num_threads);
    else
        pthread_barrier_init(&barrier,NULL,N);

    
    for (int i = 0; i < num_threads; i++) {//j==Num_threads-1 j=0; threads[j] threadid=incognita  //threads que forem maiores que o numero de incognitas nao serao inicializadas
        thread_ids[i] = (int*) malloc(sizeof(int));
        *thread_ids[i] = i;
        pthread_create(&threads[i], NULL, calculate, (void *)thread_ids[i]);
    }

       for(int i=0;i<num_threads;i++) pthread_join(threads[i],NULL);
        pthread_barrier_destroy(&barrier);

    // Exibição da solução
    printf("Solucao:\n");  
    for (int i = 0; i < N; i++) {
        printf("X%d = %lf\n", i+1, X_new[i][P]);
        pthread_mutex_destroy(&mutex_uso[i]);
    }
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tempo de execução: %f segundos com %d threads.\n", cpu_time_used,num_threads);


    pthread_exit(NULL);
}