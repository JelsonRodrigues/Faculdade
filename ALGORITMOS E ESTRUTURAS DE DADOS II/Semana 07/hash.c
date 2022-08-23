// Implementação simples de uma hash table, utilizando endereçamento aberto linear para o tratamento de colisões

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *posicoes;
    int posicoes_total;
    int posicoes_ocupadas;
} HashTable;

int hash(int valor, int numero_elementos);
void adicionarItem(HashTable *tabela, int item);
void imprimirHashTable(HashTable tabela_imprimir);
int procurarValorHashTable(HashTable tabela, int valor_procurar);
HashTable criaTabela(int numero_posicoes);
void liberaHashTable(HashTable *tabela);

int main(){
    HashTable tabela = criaTabela(10);

    for (int i = 0; i < tabela.posicoes_total; i++){
        printf("\nAdicionando o valor: %d", i * i + 3);
        adicionarItem(&tabela, i * i + 3);
    }

    imprimirHashTable(tabela);

    int posicao = procurarValorHashTable(tabela, 12);
    if (posicao != -1){
        printf("\nO valor %d esta na Tabela, na posicao %d", tabela.posicoes[posicao], posicao);
    }
    else {
        printf("\nO valor nao esta na Tabela");
    }

    liberaHashTable(&tabela);
}

int hash(int valor, int numero_elementos){
    return valor % numero_elementos;
}
void adicionarItem(HashTable *tabela, int item){
    if (tabela && tabela->posicoes_ocupadas < tabela->posicoes_total){
        int posicao_item = hash(item, tabela->posicoes_total);
        while (tabela->posicoes[posicao_item] != 0) {   // Zero seria o valor para não preenchido
            posicao_item = (posicao_item + 1) % tabela->posicoes_total;
        }
        tabela->posicoes[posicao_item] = item;
        (tabela->posicoes_ocupadas)++;
        printf("\nValor %d adicionado na posicao %d", item, posicao_item);
    }
}
void imprimirHashTable(HashTable tabela_imprimir){
    printf("\n");
    printf("\nA tabela esta %.2f%% preenchida", ((float) tabela_imprimir.posicoes_ocupadas / (float) tabela_imprimir.posicoes_total) * 100.0);
    for (int c = 0; c < tabela_imprimir.posicoes_total; c++){
        printf("\nTabela[%d]: %d", c, tabela_imprimir.posicoes[c]);
        
    }
    printf("\n");   
}
HashTable criaTabela(int numero_posicoes){
    HashTable tabela = {.posicoes = (int *) calloc(numero_posicoes, sizeof(int)),
                        .posicoes_ocupadas = 0,
                        .posicoes_total = numero_posicoes};
    if (tabela.posicoes == NULL){
        tabela.posicoes_total = 0;
        printf("\nNao foi possivel criar a tabela HASH!!");
    }

    return tabela;
}
void liberaHashTable(HashTable *tabela){
    if (tabela){
        free(tabela->posicoes);
        tabela->posicoes = NULL;
        tabela->posicoes_ocupadas = 0;
        tabela->posicoes_total = 0;
    }
}
int procurarValorHashTable(HashTable tabela, int valor_procurar){
    int posicao_item = hash(valor_procurar, tabela.posicoes_total);
    int posicao_item_copia = posicao_item;
    while (tabela.posicoes[posicao_item] != valor_procurar){
        posicao_item = (posicao_item + 1) % tabela.posicoes_total;
        if (posicao_item == posicao_item_copia){
            posicao_item = -1;
            break;
        }
    }
    return posicao_item;
}