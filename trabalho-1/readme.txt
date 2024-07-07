Integrantes: Matheus Sebastian Alencar de Carvalho (GRR20220065) e Tiago Mendes Bottamedi (GRR20220068)

Funções:
rainhas_bt_wrapped(): Função envolvida pela rainhas_bt. É a função que, de fato, resolve o problema utilzando backtracking.
rainhas_ci_wrapped(): Função envolvida pela rainhas_ci. É a função que, de fato, resolve o problema pela modelagem de conjunto independente.
ehPossivel(): Verifica se é possível colocar uma rainha naquela posição e retorna 1 caso possível e 0 caso contrário;
retorna_tamanho(): Retorna o tamanho do conjunto;
retorna_primeiro(): Retorna a primeira casa viável para continuação do problema;
remove_vizinhos(): Retorna o tabuleiro com os vizinhos a serem removidos (inviabilizados); 
imprime_tabuleiro(): Função exclusivamente para depuração.


Escolhas para a resolução do problema: 
A implementação adota a ideia de busca a partir da posição viável mais próxima, ou seja, a primeira "casa" disponível será a primeira ser considerada viável para a continuação da resolução. 
Tanto para a resolução por meio do backtracking quanto por grafos, foi utilizado um contador auxiliar. Esse contador é mantido pelas funções rainhas_bt e rainhas_ci, e passado como parâmetro para as funções que elas envolvem.
Tal contador inicia em 0, e indica quantas linhas podem ser puladas para obter uma solução. Se saímos da execução de rainhas_bt_wrapped ou rainhas_ci_wrapped sem encontrar uma solução para o tamanho desejado, incrementamos esse contador e tentamos de novo. Dessa forma, garantimos que a solução máxima será encontrada.

Escolhas para estruturas: 
Não foi utilizada uma estrutura auxiliar para a implementação do código que resolve o problema das n rainhas por meio de backtracking.
Para o método com grafos, foi escolhida a utilização de vetor n*n, que representa o tabuleiro em si, no qual as posições representam casa viável(0) ou casa inviável(1). Nessa escolha, interpretamos as casas marcadas com 0 como sendo o conjunto C do enunciado, e utilizamos o próprio vetor resposta (r) como I.

Otimizações: 
Ao invés de utilizar uma estrutura convencional de um grafo (matriz ou lista de adjacência), foi-se utilizado um vetor representando o tabuleiro, tendo em vista o alto custo de tempo e memória que seria necessário para manipulação de uma estrutura padrão.
A busca do primeiro é feita pela função retorna_primeiro, que tem sua busca otimizada ao começar a partir da posição da última casa ocupada em diante, invés de começar do índice 0. 
A funçao remove_vizinhos remove apenas os vizinhos que estão adiante do vértice escolhido, pois os vizinhos que estão atrás já foram marcados previamente.
Na função rainhas_ci_wrapped, temos como parâmetro a próxima linha na qual uma rainha deve ser colocada, e dentro da função, buscamos a primeira linha na qual uma rainha pode ser colocada. Caso essas linhas sejam diferentes, e o contador não seja suficiente para suprir a diferença, já sabemos que não há solução por aquele caminho e retornamos da chamada recursiva, ao invés de insistir até o final.


