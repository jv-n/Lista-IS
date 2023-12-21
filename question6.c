#include <pthread.h> 
#include <stdio.h>  
#include <stdlib.h>  

#define NUM_THREADS 4 
#define LINHAS 4     
#define COLUNAS 4     

// Define uma estrutura para representar um par de valores: um índice de coluna, um valor e um indicador de validade.
typedef struct {
    int coluna;
    float valor;
    int valido; // 1 se o par é válido, 0 caso contrário
} Par;

// Define duas matrizes esparsas e um vetor denso.
Par matriz1[4][3] = {{{3, 2.0, 1}, {1, -1.0, 1}}, {{0, -1.0, 1}, {1, 2.0, 1}, {2, -1.0, 1}}, {{1, -1.0, 1}, {2, 2.0, 1}, {3, -1.0, 1}}, {{2, -1.0, 1}, {3, 2.0, 1}}};
Par matrizEsparsa[4][3] = {{{3, 2.0, 1}, {1, -1.0, 1}}, {{0, -1.0, 1}, {1, 2.0, 1}, {2, -1.0, 1}}, {{1, -1.0, 1}, {2, 2.0, 1}, {3, -1.0, 1}}, {{2, -1.0, 1}, {3, 2.0, 1}}};
float vetorDense[4] = {1.0, 2.0, 3.0, 4.0};

// Define uma matriz densa e uma matriz para armazenar os resultados.
float matrizDense[4][4] = {{1.0, 2.0, 3.0, 4.0}, {2.0, 3.0, 4.0, 5.0}, {3.0, 4.0, 5.0, 6.0}, {4.0, 5.0, 6.0, 7.0}};
float resultado[4][4] = {{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}};

// Função que será executada por cada thread.
void *threadCode(void *tid)
{
  int threadId = (*(int *)tid); // Obtém o ID da thread.

  // Loop que processa uma linha da matriz por vez.
  for (int i = threadId; i < LINHAS; i = i + NUM_THREADS)
  {
    // Loop que processa cada elemento válido na linha i da matriz esparsa.
    for (int k = 0; k < 3; k++) 
    {
      // Verifica se o par é válido antes de acessar o valor e a coluna.
      if (matriz1[i][k].valido) {
        int coluna = matriz1[i][k].coluna;
        float valor = matriz1[i][k].valor;
        resultado[i][coluna] = valor * vetorDense[coluna];
        
        float valorEsparsa = 0.0;
        
        // Encontra o elemento correspondente em matrizEsparsa.
        for (int j = 0; j < 3; j++) {
          if (matrizEsparsa[i][j].valido && matrizEsparsa[i][j].coluna == coluna) {
            valorEsparsa = matrizEsparsa[i][j].valor;
            break;
          }
        }
        
        resultado[i][coluna] = resultado[i][coluna] * valorEsparsa;
        resultado[i][coluna] = resultado[i][coluna] * matrizDense[i][coluna];
      }
    }
  }
}

int main(int argc, char *argv[])
{
  pthread_t threads[NUM_THREADS]; 
  int *taskids[NUM_THREADS];      

  // Cria as threads.
  for (int i = 0; i < NUM_THREADS; i++)
  {
    taskids[i] = (int *) malloc(sizeof(int)); *taskids[i] = i;
    pthread_create(&threads[i], NULL, threadCode, (void *)taskids[i]);
  }

  // Espera todas as threads terminarem.
  for (int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }

  // Imprime a matriz de resultados.
  for (int i = 0; i < LINHAS; i++)
  {
    for (int j = 0; j < COLUNAS; j++)
    {
      printf("%f ", resultado[i][j]);
    }
    printf("\n");
  }

  pthread_exit(NULL);
}
