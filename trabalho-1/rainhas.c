#include "rainhas.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int ehPossivel (unsigned int *r, unsigned int linha, unsigned int coluna, casa *c, unsigned int k);

unsigned int rainhas_bt_wrapped(unsigned int n, unsigned int k, casa *c, unsigned int *r, unsigned int linha, unsigned int cont);

unsigned int retorna_tamanho(unsigned int *vetor, unsigned int n);

casa retorna_primeiro(unsigned int *tabuleiro, unsigned int n, unsigned int linha);

void imprime_tabuleiro(unsigned int *tabuleiro, unsigned int n);

unsigned int *remove_vizinhos(unsigned int *conjunto, casa c, unsigned int n);

unsigned int rainhas_ci_wrapped(unsigned int n, unsigned int k, casa *c, unsigned int *r, unsigned int *tabuleiro, unsigned int linha, unsigned int cont);

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

unsigned int rainhas_bt_wrapped(unsigned int n, unsigned int k, casa *c, unsigned int *r, unsigned int linha, unsigned int cont){
  if (linha == n){
    return 1;
  }
  for (unsigned int i = 1; i <= n; i++){
    if (ehPossivel(r, linha, i, c, k)){
      r[linha] = i;
      if (rainhas_bt_wrapped(n, k, c, r, linha + 1, cont)){
        return 1;
      }
      r[linha] = 0;
    }
  }
  if (cont){
    return rainhas_bt_wrapped(n, k, c, r, linha + 1, cont - 1);
  }
  return 0;
}

unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r){
  for (unsigned int i = 0; i < n; i++){
    r[i] = 0;
  }
  for (unsigned int i = 0; i < n; i++){
    if (rainhas_bt_wrapped(n, k, c, r, 0, i)){
      return r;
    }
  }
  return r;
}

unsigned int retorna_tamanho(unsigned int *vetor, unsigned int n){
    unsigned int tamanho = 0;
    for (unsigned int i = 0; i < n; i++){
        if (vetor[i] == 0){
            tamanho++;
        }
    }
    return tamanho;
}

casa retorna_primeiro(unsigned int *tabuleiro, unsigned int n, unsigned int linha){
    casa c;
    for (unsigned int i = n*linha; i < n*n; i++){
        if (tabuleiro[i] == 0){
            tabuleiro[i] = 1;
            c.linha = i / n;
            c.coluna = i % n + 1;
            return c;
        }
    }
    c.linha = n;
    c.coluna = n+1;
    return c;
}

void imprime_tabuleiro(unsigned int *tabuleiro, unsigned int n){
    for (unsigned int i = 0; i < n; i++){
        for (unsigned int j = 0; j < n; j++){
            printf("%d ", tabuleiro[i * n + j]);
        }
        printf("\n");
    }
    printf ("\n");
}

unsigned int *remove_vizinhos(unsigned int *conjunto, casa c, unsigned int n){
    unsigned int *diferenca = (unsigned int *) malloc(n*n * sizeof(unsigned int));
    memcpy(diferenca, conjunto, n*n*sizeof(unsigned int));
    for (unsigned int i = c.linha * n + c.coluna - 1; i < n*n; i++){
        if (i % n == c.coluna - 1 || i / n == c.linha || i % n + i / n == c.coluna + c.linha - 1 || i % n - i / n == c.coluna - c.linha - 1){
            diferenca[i] = 1;
        }
    }
    return diferenca;
}

unsigned int rainhas_ci_wrapped(unsigned int n, unsigned int k, casa *c, unsigned int *r, unsigned int *tabuleiro, unsigned int linha, unsigned int cont){
    if (linha == n){
        return 1;
    }
    unsigned int tamanhot = retorna_tamanho(tabuleiro, n*n);
    if (tamanhot + linha < n - cont){
        return 0;
    }
    casa aux = retorna_primeiro(tabuleiro, n, linha);
    if (aux.linha > linha + cont){
        return 0;
    }
    if (aux.linha == n){
        return 1;
    }
    while (aux.linha > linha){
        cont--;
        linha++;
    }
    r[aux.linha] = aux.coluna;
    unsigned int *tabuleiro_aux = remove_vizinhos(tabuleiro, aux, n);
    if (rainhas_ci_wrapped(n, k, c, r, tabuleiro_aux, linha + 1, cont)){
        free(tabuleiro_aux);
        return 1;
    }
    free(tabuleiro_aux);
    r[aux.linha] = 0;
    return rainhas_ci_wrapped(n, k, c, r, tabuleiro, linha, cont);
}

unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r){
  for (unsigned int i = 0; i < n; i++){
    r[i] = 0;
  }
  unsigned int *tabuleiro = calloc(n * n, sizeof(unsigned int));
  unsigned int *aux = calloc(n * n, sizeof(unsigned int));
  for (unsigned int i = 0; i < k; i++){
    tabuleiro[(c[i].linha - 1) * n + c[i].coluna - 1] = 1;
    aux[(c[i].linha - 1) * n + c[i].coluna - 1] = 1;
  }
  for (unsigned int i = 0; i < n; i++){
    if (rainhas_ci_wrapped(n, k, c, r, tabuleiro, 0, i)){
      free(tabuleiro);
      free(aux);
      return r;
    }
    memcpy(tabuleiro, aux, n*n*sizeof(unsigned int));
  }
  free(tabuleiro);
  free(aux);
  return r;
}