#include "rainhas.h"
#include <stdlib.h>
#include <stdio.h>

struct conjunto {
    int max;    /* tamanho maximo do vetor atualmente alocado     */
    int card;   /* cardinalidade, isto eh, tamanho usado ate max  */
    int ptr;    /* ponteiro para algum indice do vetor (iterador) */
    casa *v;     /* vetor de casas com no maximo max elementos  */
};
typedef struct conjunto conjunto_t;

conjunto_t *cria_cjt (int max){
	conjunto_t* c;

	if(!(c = malloc(sizeof(conjunto_t))))
		return NULL;
    
    if(!(c->v = malloc(sizeof(casa)*max))){
        return NULL;
    }

	c->max = max;
	c->card = 0;

	return c;
}

/*
 * Remove todos os elementos do conjunto, libera espaco e devolve NULL.
 */
conjunto_t *destroi_cjt (conjunto_t *c){
    free(c->v);
    free(c);

    return NULL;
}

/*
 * Retorna 1 se o conjunto esta vazio e 0 caso contrario.
 */
int vazio_cjt (conjunto_t *c){
    if (c->card == 0)
        return 1;
    
    return 0;
}

/*
 * Retorna a cardinalidade do conjunto, isto eh, o numero de elementos presentes nele.
 */
int cardinalidade_cjt (conjunto_t *c){
    int cardinalidade;
    cardinalidade = c->card;

    return cardinalidade;
}


/*
 * Insere o elemento no conjunto, garante que nao existam repeticoes.
 * Retorna 1 se a operacao foi bem sucedida. Se tentar inserir elemento existente,
 * nao faz nada e retorna 1 tambem. Retorna 0 em caso de falha por falta de espaco.
 */
int insere_cjt (conjunto_t *c, casa elemento){
    int i;
    i = c->card;

    if (c->card >= c->max)
        return 0;

    if ((pertence_cjt(c, elemento)))
        return 1;

    *(c->v+i) = elemento;
    c->card++;

    return 1;
}

/*
 * Retorna a posicao do elemento se o elemento pertence ao conjunto e cardinalidade + 1 caso contrario.
 */
int posicao_no_cjt (conjunto_t *c, casa elemento){
    int i;

    for (i = 0; i < c->card; i++)
        if (c->v[i].linha == elemento.linha && c->v[i].coluna == elemento.coluna)
            return i;
        
    return c->card + 1;
}

/*
 * Remove o elemento 'elemento' do conjunto caso ele exista.
 * Retorna 1 se a operacao foi bem sucedida e 0 se o elemento nao existe.
 */
int retira_cjt (conjunto_t *c, casa elemento){
    int i;

	if(!(pertence_cjt(c, elemento)))
		return 0;

	for(i = posicao_no_cjt(c, elemento); i < c->card; i++)
			*(c->v+i) = *(c->v+i+1);
	
	c->card--;

	return 1;
}

/*
 * Retorna 1 se o elemento pertence ao conjunto e 0 caso contrario.
 */
int pertence_cjt (conjunto_t *c, casa elemento){
    int i;

    for(i = 0; i < c->card; i++)
        if (c->v[i].linha == elemento.linha && c->v[i].coluna == elemento.coluna)
            return 1;

    return 0;
}


/*
 * Cria e retorna o conjunto diferenca entre c1 e c2, nesta ordem.
 * Retorna NULL se a operacao falhou.
 */
conjunto_t *diferenca_cjt (conjunto_t *c1, conjunto_t *c2){
    int i, elemento;
    conjunto_t* diferenca;
    diferenca = cria_cjt(c1->max);

    if (diferenca == NULL)
        return NULL;

    for (i = 0; i < c1->card; i++){
        elemento = *(c1->v+i);
        if (!(pertence_cjt(c2, elemento))){
            insere_cjt(diferenca, elemento);
        }
    }
    return diferenca;
}

/*
 * Escolhe um elemento qualquer do conjunto para ser removido, o remove e
 * o retorna.
 * Nao faz teste se conjunto eh vazio, deixa para main fazer isso       
 */
casa retira_um_elemento_cjt (conjunto_t *c){
    casa x = c->v[rand() % c->card];
    retira_cjt(c, x->linha, x->coluna);
    return x;
}

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

unsigned int ultima_linha = 0;

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

unsigned int acha_linha (unsigned int *r, unsigned int n, unsigned int *linhas_impossiveis) {
  unsigned int i;
  for (i = 0; i < n; i++) {
    if (r[i] == 0 && linhas_impossiveis[i] == 0) {
      return i;
    }
  }
  return n;
}

unsigned int *rainhas_bt_wrapped(unsigned int n, unsigned int k, casa *c, unsigned int *r, unsigned int *linhas_impossiveis) {
  unsigned int linha, *aux;
  linha = acha_linha(r, n, linhas_impossiveis);
  ultima_linha = linha;
  if (linha == n) {
    return r;
  }
  for (unsigned int i = 1; i <= n; i++) {
    if (ehPossivel(r, linha, i, c, k)) {
      r[linha] = i;
      aux = rainhas_bt_wrapped(n, k, c, r, linhas_impossiveis);
      if (aux != NULL)
        return aux;
      r[linha] = 0;
    }
  }
  if (linha == 0){
    linhas_impossiveis[ultima_linha] = 1;
    rainhas_bt_wrapped(n, k, c, r, linhas_impossiveis);
  }
  return NULL;
}

unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
  unsigned int *linhas_impossiveis = (unsigned int *) malloc(n * sizeof(unsigned int));
  unsigned int *aux = r;
  for (unsigned int i = 0; i < n; i++) {
    linhas_impossiveis[i] = 0;
  }
  if (r = rainhas_bt_wrapped(n, k, c, r, linhas_impossiveis))
    return r;
  return aux;
}
//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n
// rainhas com casas proibidas usando a modelagem do problema como
// conjunto independente de um grafo
//
// n, c e r são como em rainhas_bt()

unsigned int* conj_independente(int **matriz_adjacencia, conjunto_t *ind, conjunto_t *ind_aux, unsigned int n, unsigned int k, casa *c, unsigned int *r){
  unsigned int *aux;
  casa x;
  if (cardinalidade_cjt(ind) == n){
    return r;
  }
  if (cardinalidade_cjt(ind) + cardinalidade_cjt(ind_aux) < n){
    return NULL;
  }
  x = retira_um_elemento_cjt(ind_aux);
  aux = conj_independente(matriz_adjacencia, insere_cjt(ind, x), ind_aux, n, k, c, r);
}


unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
  int **matriz_adjacencia = malloc(n * sizeof(int *));
  conjunto_t *ind = cria_cjt(n);
  conjunto_t *ind_aux = cria_cjt(n*n);
  for (int i = 0; i < n; i++) {
    matriz_adjacencia[i] = (int *) malloc(n * sizeof(int));
    for (int j = 0; j < n; j++) {
      matriz_adjacencia[i][j] = 0;
      insere_cjt(ind, (casa) {i + 1, j + 1});
    }
  }
  for (int i = 0; i < k; i++) {
    matriz_adjacencia[c[i].linha - 1][c[i].coluna - 1] = -1;
    retira_cjt(ind, (casa) {c[i].linha, c[i].coluna});
  }
}
