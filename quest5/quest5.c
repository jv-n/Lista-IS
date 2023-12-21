#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

typedef struct Parameter{
    int l;
    int r;
}Parameter;

int *sorted;

void merge(int l, int m, int r){  //Funcao merge do algoritmo mergesort
  int i, j, k;
  int n1 = m - l + 1;
  int n2 =  r - m;
  
  int left[n1], right[n2];
  for (i = 0; i < n1; i++)
    left[i] = sorted[l + i];
  for (j = 0; j < n2; j++)
    right[j] = sorted[m+ 1+ j];
  
  i = 0;
  j = 0;
  k = l;
  while (i < n1 && j < n2){
    if (left[i] <= right[j]){
      sorted[k] = left[i];
      i++;
    }
    else{
      sorted[k] = right[j];
      j++;
    }
    k++;
  }
  while (i < n1){
    sorted[k] = left[i];
    i++;
    k++;
  }
  while (j < n2){
    sorted[k] = right[j];
    j++;
    k++;
  }
}
void* mergeSort(void* arg){
  Parameter parameter = *((Parameter*)arg);
  int l = parameter.l;
  int r = parameter.r;
  
  if (l < r){
    int m = l+(r-l)/2;
    
    pthread_t thread1;
    pthread_t thread2;
    Parameter d1 = {l, m};  //Armazena na struct os parametros do mergesort
    Parameter d2 = {m+1, r};  //""""""""""
    
    pthread_create(&thread1, NULL, mergeSort, &d1);  //Cada particao do mergesort será uma thread
    pthread_create(&thread2, NULL, mergeSort, &d2);  //""""""""""""""""
    
    pthread_join(thread1, NULL);  //Chegou nas folhas e vai esperar a outra threads terminar antes de ir para a funcao merge
    pthread_join(thread2, NULL);  //"""""""""""""""" 
    
    merge(l, m, r);
  }
}

int main(){ 
  int i, n;

  printf("Quantidade de numeros a serem ordenados\n");   
  scanf("%d", &n);

  sorted=(int *) malloc(sizeof(int)*n);

  for(i=0; i<n; i++)    //Guardar numero dentro de uma variavel global
    scanf("%d", &sorted[i]);

  Parameter parameter = {0, n-1};  //Armazena l,r dentro de uma struct
  pthread_t thread;
  
  pthread_create(&thread, NULL, mergeSort, &parameter);   
  pthread_join(thread, NULL);
  
  printf("Numeros ordenados: \n");   //Mostrar na tela do usuario os numeros ordenados
  for (i=0; i < n; i++)
    printf("%d ", sorted[i]);
  return 0;
}
