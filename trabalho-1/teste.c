#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "rainhas.h"

static clock_t inicio, fim;

//------------------------------------------------------------------------------
#define CRONOMETRA(call,t) inicio = clock(); (call); fim = clock(); t=fim-inicio

//------------------------------------------------------------------------------
static void mostra_resposta(unsigned int n, unsigned int *r) {

  for (unsigned int i=0; i<n; i++) {
    printf("%u ", r[i]);
  }

  printf("\n");
}
//------------------------------------------------------------------------------
// preenche proibido[0..2n-1] com as posições das diagonais do tabuleiro n x n
//
// devolve &(proibido[2n]): o endereço a partir do qual proibir novas posições

static casa *proibe_diagonais(unsigned int n, casa *proibido) {

  for (int i = 0; i<7; i++){
    proibido[i].linha = 1;
    proibido[i].coluna = i + 1;
  }
  for (int i = 7; i<13; i++){
    proibido[i].linha = 2;
    proibido[i].coluna = i - 6;
  }
  for (int i = 13; i<18; i++){
    proibido[i].linha = 3;
    proibido[i].coluna = i - 12;
  }
  for (int i = 18; i<23; i++){
    proibido[i].linha = 5;
    proibido[i].coluna = i - 17;
    proibido[i+5].linha = 6;
    proibido[i+5].coluna = i - 17;
    proibido[i+10].linha = 7;
    proibido[i+10].coluna = i - 17;
    proibido[i+15].linha = 8;
    proibido[i+15].coluna = i - 17;
  }


  // proíbe todas as casas nas diagonais
  // for(unsigned int i = 0, p = 1; i < 2 * n; i+=2, p++) {

  //   // diagonal "principal"
  //   proibido[i].linha = proibido[i].coluna = p;

  //   // "outra" diagonal
  //   proibido[i+1].linha = p;
  //   proibido[i+1].coluna = n - p + 1;
  // }

  return proibido + 2*n;
}

//------------------------------------------------------------------------------
int main (void) {

  unsigned int n = 8;
  unsigned int *resposta = malloc(n*sizeof(unsigned int));

  unsigned int k = 38;
  casa *proibido = malloc(k*sizeof(casa));
  
  proibe_diagonais(k, proibido);

  printf("backtracking: ");
  long int tempo_bt;
  CRONOMETRA(rainhas_bt(n, k, proibido, resposta), tempo_bt);
  printf("%ld\n", tempo_bt);
  mostra_resposta(n, resposta);

  printf("grafo: ");
  long int tempo_ci;
  CRONOMETRA(rainhas_ci(n, k, proibido, resposta), tempo_ci);
  printf("%ld\n", tempo_ci);
  mostra_resposta(n, resposta);

  printf("%.2f\n", (double)tempo_ci/(double)tempo_bt);

  free(proibido);
  free(resposta);

  return 0;
}
