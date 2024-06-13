#include "rainhas.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

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
  while (cont){
    for (unsigned int i = 1; i <= n; i++){
      if (ehPossivel(r, linha, i, c, k)){
        r[linha] = i;
        if (rainhas_bt_wrapped(n, k, c, r, linha + 1, cont)){
          return 1;
        }
        r[linha] = 0;
      }
    }
    cont--;
    linha++;
    if (linha == n){
      return 0;
    }
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

void marca_vizinhos(unsigned int *tabuleiro, unsigned int n, unsigned int linha, unsigned int coluna, unsigned int marcador){
    tabuleiro[linha * n + coluna - 1] = 1;
    for (unsigned int i = 0; i < n*n; i++){
        if (i % n == coluna - 1 || i / n == linha || i % n + i / n == coluna + linha - 1 || i % n - i / n == coluna - linha - 1){
            tabuleiro[i] = marcador;
        }
    }
}

unsigned int rainhas_ci_wrapped(unsigned int n, unsigned int k, casa *c, unsigned int *r, unsigned int *tabuleiro, unsigned int linha, unsigned int cont){
  if (linha == n){
    return 1;
  }
  while (cont){
    for (unsigned int i = 1; i <= n; i++){
      if (tabuleiro[(linha) * n + i - 1] == 0){
        r[linha] = i;
        marca_vizinhos(tabuleiro, n, linha, i, 1);
        if (rainhas_ci_wrapped(n, k, c, r, tabuleiro, linha + 1, cont)){
          return 1;
        }
        marca_vizinhos(tabuleiro, n, linha, i, 0);
        r[linha] = 0;
      }
    }
    cont--;
    linha++;
    if (linha == n){
      return 0;
    }
  }
  return 0;

}

unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r){
  for (unsigned int i = 0; i < n; i++){
    r[i] = 0;
  }
  unsigned int *tabuleiro = calloc(n * n, sizeof(unsigned int));
  for (int i = 0; i < k; i++){
    tabuleiro[(c[i].linha - 1) * n + c[i].coluna - 1] = 1;
  }
  for (unsigned int i = 0; i < n; i++){
    if (rainhas_ci_wrapped(n, k, c, r, tabuleiro, 0, i)){
      free(tabuleiro);
      return r;
    }
  }
  free(tabuleiro);
  return r;
}