#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#define true 1
#define false 0

typedef int bool;

typedef struct {
  int * A;
  int tamanhoAtual;
  int tamanhoMaximo;
} HEAP;

long int compRebaixa = 0;
long int compInsere = 0;

void inicializarHeap(HEAP * h, int tamanhoMax) {
  h->A = (int*) malloc(sizeof(int)*(tamanhoMax+1));
  h->tamanhoAtual = 0;
  h->tamanhoMaximo = tamanhoMax;
}

void destruirHeap(HEAP * h) {
  int tamanho = h->tamanhoMaximo;
  free(h->A);
  h->tamanhoMaximo=0;
  h->tamanhoAtual=0;
}

int pai(int i) {
  return i/2;
}

int filhoEsquerda(int i) {
  return 2*i;
}

int filhoDireita(int i) {
  return 2*i + 1;
}

// Insere no final do arranjo do heap
bool inserir(HEAP * h, int valor) {
  compInsere++;
  if (h->tamanhoAtual < h->tamanhoMaximo) {
    (h->tamanhoAtual)++;
    h->A[h->tamanhoAtual] = valor;
    return true;
  }

  return false;
}

bool inserirHeap(HEAP * h, int chave) {
  int i, temp;
  compInsere++;
  if (h->tamanhoAtual == h->tamanhoMaximo) return false;
  (h->tamanhoAtual)++;
  i = h->tamanhoAtual;
  h->A[i] = chave;
  while ((i>1) && (h->A[pai(i)]<h->A[i])) {
     temp = h->A[i];
     h->A[i] = h->A[pai(i)];
     h->A[pai(i)] = temp;       
     i = pai(i);
  }

  return true;
}

/**
 * Função Rebaixa implementada conforme slides da disciplina de EDPA
 */
void rebaixa(HEAP * h, int n, int i) {
  int pai = i;
  int fesq = filhoEsquerda(i);
  int fdir = filhoDireita(i);
  int menor = pai;
  int aux = 0;

  compRebaixa++;
  if ((fesq <= n) && (h->A[fesq] < h->A[menor])) menor = fesq;
  compRebaixa++;
  if ((fdir <= n) && (h->A[fdir] < h->A[menor])) menor = fdir;

  compRebaixa++;
  if (menor != pai) {
    //troca
    aux = h->A[menor];
    h->A[menor] = h->A[pai];
    h->A[pai] = aux;

    rebaixa(h, n, menor);
  }
}

void controi_heap_rebaixa(HEAP * h, int *a, int n) {
  memcpy((h->A)+1, a, sizeof(int) * n);
  for (int i=n/2; i>0; i--) {
    rebaixa(h, n, i);
  }
}

int main() {
  int size=100;
  srand(time(0));

  for (int i=0; i<7; i++) {
    /**
     * meuHeap_i será o heap que será construido a partir da (i)nserção;
     * e meuHeap_r será o heap que será construído a partir do rebaixa
     */
    HEAP meuHeap_i, meuHeap_r;
    size = size*10;
    int vetor[size];
    inicializarHeap(&meuHeap_i, size);
    inicializarHeap(&meuHeap_r, size);

    // gerando o vetor de números aleatorios de tamanho size que será usado na comparação dos dois métodos de criação de heap
    for (int i = 0; i < size; i++) {
      vetor[i] = rand() % 10000000;
    }
	  
    //printf("--------------\n");
    /*Medindo tempo da criação via inserção. O Tempo Decorrido(tDecorrido) dessa operação é dado pela diferença do tempo onde se 
    o "relógio" (tInicio) e o tempo onde para-se o "relógio"(tFim)
    */ 
    clock_t tInicio, tFim;
    tInicio = clock();
    for(int i=0;i<size;i++) inserir(&meuHeap_i, vetor[i]);
    tFim = clock();
    double tDecorrido = ( (double) (tFim - tInicio) ) / CLOCKS_PER_SEC;
    //printf("[Inserção]Tempo :\t[%lf] para %d elementos\n",tDecorrido,size);
    printf("Insere,%d,%lf,%ld\n", size, tDecorrido, compInsere);

	  //gerando meuHeap_R
	  tInicio = clock();

    controi_heap_rebaixa(&meuHeap_r, vetor, size);
    tFim = clock();
    tDecorrido = ( (double) (tFim - tInicio) ) / CLOCKS_PER_SEC; 
    printf("Rebaixa,%d,%lf,%ld\n", size, tDecorrido, compRebaixa);
    //printf("[Rebaixa]Tempo :\t[%lf] para %d elementos\n",tDecorrido,size);
    //printf("--------------\n");

    destruirHeap(&meuHeap_i);
    destruirHeap(&meuHeap_r);
    compRebaixa = 0;
    compInsere = 0;
  }

}