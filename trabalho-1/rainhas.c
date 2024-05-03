#include "rainhas.h"
#include <stdlib.h>

//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n rainhas 
// com casas proibidas usando backtracking
//
//    n é o tamanho (número de linhas/colunas) do tabuleiro
//
//    c é um vetor de k 'struct casa' indicando as casas proibidas
//
//    r é um vetor de n posições (já alocado) a ser preenchido com a resposta:
//      r[i] = j > 0 indica que a rainha da linha i+1 fica na coluna j;
//      r[i] = 0     indica que não há rainha nenhuma na linha i+1
//
// devolve r

int ehPossivel (unsigned int *r, unsigned int linha, unsigned int coluna, casa *c, unsigned int k) {
  for (unsigned int i = 0; i < linha; i++) {
    if (r[i] == coluna) {
      return 0;
    }
    if (r[i] != 0) {
      if (abs((int)(r[i] - coluna)) == abs((int)(i - linha))) {
        return 0;
      }
    }
  }
  for (unsigned int i = 0; i < k; i++) {
    if (c[i].linha == linha + 1 && c[i].coluna == coluna) {
      return 0;
    }
  }
  return 1;
}

unsigned int acha_linha (unsigned int *r, unsigned int n) {
  unsigned int i;
  for (i = 0; i < n; i++) {
    if (r[i] == 0) {
      return i;
    }
  }
  return n;
}

unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
  unsigned int *aux, linha;
  linha = acha_linha(r, n);
  if (linha == n) {
    return r;
  }
  for (unsigned int i = 1; i <= n; i++) {
    if (ehPossivel(r, linha, i, c, k)) {
      r[linha] = i;
      aux = rainhas_bt(n, k, c, r);
      if (aux != NULL) {
        return aux;
      }
      r[linha] = 0;
    }
  }
  return NULL;
}
//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n
// rainhas com casas proibidas usando a modelagem do problema como
// conjunto independente de um grafo
//
// n, c e r são como em rainhas_bt()

unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r) {

  n = n;
  k = k;
  c = c;

  return r;
}
