Este trabalho pedia a implementacao de duas solucoes para o problema do caixeiro viajante
Uma utilizando um algoritmo aproximativo e outra utilizando um algoritmo exato

O algoritmo aproximativo implementado foi o algoritmo 2-aproximativo, a intenção era implementar o 
algoritmo de Christofides que é 1.5-aproximativo, mas por falta de tempo não foi possível implementar
o algoritmo que realiza o min weight perfect maching (algoritmo de Blossom) dos vértices de grau émpar e portanto ele 
não vai ser 1.5-aproximativo.

O algoritmo exato implementado foi o algoritmo força bruta que testa todas as possibilidades,
roda em tempo fatorial (super exponencial) e utiliza espaço constante, após pesquisas foi descoberto
o algoritmo de Held-Karp que executa em tempo exponencial (O(n*n*2^n)) e utiliza espaço exponencial (O(n*2^n)),
tambem não foi possível implementar por falta de tempo.

Várias otimizacoes podem ser relizadas no codigo, para rodar mais rapidamente, mas mesmo com todas as otimizacoes
o algoritmo exato ainda assim é impraticavel de rodar.

Para o algoritmo exato existem 3 diferentes formas implementadas de testar os caminhos:
1 - Usando recursão para percorrer os caminhos diferentes
2 - Usando o algoritmo de Heap para testar os caminhos
3 - Usando ordem lexicográfica

A forma mais rápida é a 3, executou o teste tsp3_1194.csv em 30 minutos em r5 5600x
A forma 1 é a mais lenta, executou teste tsp3_1194.csv em 2 dias em um i7 4790k
A forma 2 não foi testada no tsp3_1194.csv, mas é consideravelmente mais rápida que a 1 e consideravelmente mais lenta que a forma 3