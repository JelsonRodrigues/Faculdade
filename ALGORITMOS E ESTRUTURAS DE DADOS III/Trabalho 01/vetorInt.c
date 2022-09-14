#include "vetorInt.h"

void adicionaItemVetor(VetorInt *vetor_adicionar, int valor_adicionar){
    if (vetor_adicionar){
        vetor_adicionar->tamanho++;
        vetor_adicionar->vetor = realloc(vetor_adicionar->vetor, vetor_adicionar->tamanho * sizeof(int));
        if (vetor_adicionar->vetor){
            vetor_adicionar->vetor[vetor_adicionar->tamanho - 1] = valor_adicionar;
        }
        else{
            vetor_adicionar->tamanho = 0;
        }
    }
}
int procuraItemVetor(VetorInt *vetor_procurar, int valor_procurar){
    if (vetor_procurar){
        for (int c = 0; c < vetor_procurar->tamanho; c++){
            if (valor_procurar == vetor_procurar->vetor[c]){
                return c;
            }
        }
    }
    return -1;
}
void removerItemVetor(VetorInt *vetor_excluir, int item_excluir){
    if (vetor_excluir){
        removerItemVetorPorPosicao(vetor_excluir, procuraItemVetor(vetor_excluir, item_excluir));
    }
}
int removerItemVetorPorPosicao(VetorInt *vetor, int posicao) {
    if (vetor){
        if (posicao >=0 && posicao < vetor->tamanho){
            int valor = vetor->vetor[posicao];
            if (posicao < vetor->tamanho - 1){
                memmove(&vetor->vetor[posicao], &vetor->vetor[posicao + 1], sizeof(int) * (vetor->tamanho - posicao));
            }
            vetor->tamanho--;
            vetor->vetor = realloc(vetor->vetor, vetor->tamanho * sizeof(int));
            if (vetor->vetor == NULL){
                vetor->tamanho = 0;
            }
            return valor;
        }   
    }
    return 0;
}
int pegarItemDoVetor(VetorInt *vetor, int posicao){
    if (vetor){
        if (posicao < 0){
            posicao += vetor->tamanho;
        }
        if (posicao >=0 && posicao < vetor->tamanho){
            return vetor->vetor[posicao];
        }
    }
    return 0;
}
void inserirItemNaPosicao(VetorInt *vetor_inserir, int posicao, int valor_inserir){
    if (vetor_inserir){
        if (posicao < 0) {
            posicao += vetor_inserir->tamanho;
        }
        if (posicao < vetor_inserir->tamanho && posicao >= 0){
            if (posicao == vetor_inserir->tamanho - 1){
                adicionaItemVetor(vetor_inserir, valor_inserir);
                return;
            }
            vetor_inserir->tamanho++;
            vetor_inserir->vetor = realloc(vetor_inserir->vetor, vetor_inserir->tamanho * sizeof(int));
            if (vetor_inserir->vetor == NULL){
                vetor_inserir->tamanho = 0;
                return;
            }
            memmove(&vetor_inserir[posicao + 1], &vetor_inserir[posicao], sizeof(int) * (vetor_inserir->tamanho - posicao));
            vetor_inserir->vetor[posicao] = valor_inserir;
        }
    }
}
VetorInt criaVetorInt(){
    VetorInt temp = {  
        .tamanho = 0,
        .vetor = NULL
    };
    return temp;
}
bool estaVazio(VetorInt *vetor){
    if (vetor->tamanho == 0) return true;
    return false;
}
void liberarVetorInt(VetorInt *vetor){
    if (vetor){
        if (vetor->vetor){
            free(vetor->vetor);
            vetor->vetor = NULL;
        }
        vetor->tamanho = 0;
    }
}

void inverterVetor(VetorInt vetor){
    int metade = vetor.tamanho >> 1;
    for (int c = 0; c < metade; c++){
        if (vetor.vetor[c] != vetor.vetor[vetor.tamanho - c - 1]) swap(vetor.vetor[c], vetor.vetor[vetor.tamanho - c - 1]);
    }
}