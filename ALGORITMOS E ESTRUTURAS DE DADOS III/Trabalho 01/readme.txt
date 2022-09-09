Este trabalho pedia a implementacao de duas solucoes para o problema do caixeiro viajante
Uma utilizando um algoritmo aproximativo e outra utilizando um algoritmo exato

O algoritmo aproximativo implementado foi o algoritmo 2-aproximativo, a intencao era implementar o 
algoritmo de Christofides que e 1.5-aproximativo, mas por falta de tempo nao foi possivel implementar
o algoritmo que realiza o min weight perfect maching (algoritmo de Blossom) dos vertices de grau impar e portanto ele 
nao vai ser 1.5-aproximativo.

O algoritmo exato implementado foi o algoritmo forca bruta que testa todas as possibilidades,
roda em tempo fatorial (super exponencial) e utiliza espaco constante, apos pesquisas foi descoberto
o algoritmo de Held-Karp que executa em tempo exponencial (O(n*n*2^n)) e utiliza espaco exponencial (O(n*2^n)),
tambem nao foi possivel implementar por falta de tempo.

Varias otimizacoes podem ser relizadas no codigo, para rodar mais rapidamente, mas mesmo com todas as otimizacoes
o algoritmo exato ainda assim e impraticavel de rodar.