#include "rainhas.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

unsigned int *remove_vizinhos(unsigned int *conjunto, casa c, unsigned int n){
    unsigned int *diferenca = (unsigned int *) malloc(n*n * sizeof(unsigned int));
    memcpy(diferenca, conjunto, n*n*sizeof(unsigned int));
    for (unsigned int i = 0; i < n*n; i++){
        if (i % n == c.coluna - 1 || i / n == c.linha || i % n + i / n == c.coluna + c.linha - 1 || i % n - i / n == c.coluna - c.linha - 1){
            diferenca[i] = 1;
        }
    }
    return diferenca;
}

//--------------------------------------------------------------  ----------------
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

casa acha_linha_conjunto (unsigned int *conjunto, unsigned int n){
  for (unsigned int i = 0; i < n; i++){
    for (unsigned int j = 0; j < n; j++){
      if (conjunto[i*n +j] == 0){
        conjunto[i*n + j] = 1;
        return (casa) {i, j + 1};
      }
    }
  }
  return (casa) {n, n + 1};

}

unsigned int retorna_tamanho_conjunto(unsigned int *conjunto, unsigned int n){
  unsigned int tamanho = 0;
  for (unsigned int i = 0; i < n; i++){
      if (conjunto[i] == 0){
        tamanho++;
      }
  }
  return tamanho;
}

unsigned int retorna_tamanho(unsigned int *r, unsigned int n){
  unsigned int tamanho = 0;
  for (unsigned int i = 0; i < n; i++){
      if (r[i]){
        tamanho++;
      }
  }
  return tamanho;
}

unsigned int retorna_linha(unsigned int *r, unsigned int n, unsigned int *tabuleiro) {
  for (unsigned int i = 0; i < n; i++) {
    if (r[i] == 0) {
      for (unsigned int j = 0; j < n; j++) {
        if (tabuleiro[i * n + j] == 0) 
          return i;
      }
    }
  }
  return n;
}

unsigned int rainhas_bt_wrapped(unsigned int n, unsigned int k, casa *c, unsigned int *r, unsigned int *tabuleiro) {
  unsigned int linha, aux;
  linha = retorna_linha(r, n, tabuleiro);
  if (linha == n) {
    return 1;
  }
  for (unsigned int i = 1; i <= n; i++) {
    if (ehPossivel(r, linha, i, c, k)) {
      r[linha] = i;
      aux = rainhas_bt_wrapped(n, k, c, r, tabuleiro);
      if (aux)
        return aux;
      r[linha] = 0;
    }
  }
  return 0;
}

unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
  unsigned int *tabuleiro = (unsigned int *) calloc(n*n, sizeof(unsigned int));
  unsigned int contadorcol = 0, contadorlin = 0;
  for (unsigned int i = 0; i < k; i++){
    tabuleiro[(c[i].linha - 1) * n + c[i].coluna - 1] = 1;
  }
  for (unsigned int i = 0; i < n; i++) {
    unsigned int contcol = 0, contlin = 0;
    r[i] = 0;
    for (unsigned int j = 0; j < n; j++) {
      if (tabuleiro[j * n + i]) {
        contcol++;
      }
      if (tabuleiro[i * n + j]) {
        contlin++;
      }
    }
    if (contcol == n){
      contadorcol++;
    }
    if (contlin == n){
      contadorlin++;
    }
  }
  if (contadorcol > contadorlin){
    for (unsigned int i = 1; i <= contadorcol - contadorlin; i++){
      for (unsigned int j = 0; j < n; j++){
        tabuleiro[i*n + j] = 1;
      }
    }
  }
  rainhas_bt_wrapped(n, k, c, r, tabuleiro);
  free(tabuleiro);
  return r; 
}
//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n
// rainhas com casas proibidas usando a modelagem do problema como
// conjunto independente de um grafo
//
// n, c e r são como em rainhas_bt()

unsigned int rainhas_ci_wrapped(unsigned int *conjunto, unsigned int n, unsigned int *r, unsigned int *tabuleiro) {
  unsigned int aux;
  unsigned int *aux_conjunto;
  unsigned int tamanho  = retorna_linha(r, n, tabuleiro);
  unsigned int tamanho_conjunto = retorna_tamanho_conjunto(conjunto, n * n);
  if (tamanho == n){
    return 1;
  }
  if (tamanho_conjunto + tamanho < n){
    return 0;
  }
  casa c = acha_linha_conjunto(conjunto, n);
  r[c.linha] = c.coluna;
  aux_conjunto = remove_vizinhos(conjunto, c, n);
  aux = rainhas_ci_wrapped(aux_conjunto, n, r, tabuleiro);
  free(aux_conjunto);
  if (aux) {
    return aux;
  }
  r[c.linha] = 0;
  return rainhas_ci_wrapped(conjunto, n, r, tabuleiro);
}


unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
  unsigned int contadorcol = 0, contadorlin = 0;
  unsigned int *conjunto = (unsigned int *) calloc(n * n, sizeof(unsigned int));
  for (unsigned int i = 0; i < k; i++){
    conjunto[(c[i].linha - 1) * n + c[i].coluna - 1] = 1;
  }
  for (unsigned int i = 0; i < n; i++) {
    unsigned int contcol = 0, contlin = 0;
    r[i] = 0;
    for (unsigned int j = 0; j < n; j++) {
      if (conjunto[j * n + i]) {
        contcol++;
      }
      if (conjunto[i * n + j]) {
        contlin++;
      }
    }
    if (contcol == n){
      contadorcol++;
    }
    if (contlin == n){
      contadorlin++;
    }
  }
  if (contadorcol > contadorlin){
    for (unsigned int i = 1; i <= contadorcol - contadorlin; i++){
      for (unsigned int j = 0; j < n; j++){
        conjunto[i*n + j] = 1;
      }
    }
  }
  rainhas_ci_wrapped(conjunto, n, r, conjunto);
  free(conjunto);
  return r;
}
