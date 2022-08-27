#ifndef VETORINT_H
#define VETORINT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int *vetor;
    int tamanho;
} VetorInt;

void adicionaItemVetor(VetorInt *vetor_adicionar, int valor_adicionar);
int procuraItemVetor(VetorInt *vetor_procurar, int valor_procurar);
void removerItemVetor(VetorInt *vetor_excluir, int item_excluir);
void inserirItemNaPosicao(VetorInt *vetor_inserir, int posicao, int valor_inserir);
int pegarItemDoVetor(VetorInt *vetor, int posicao);
VetorInt criaVetorInt();
bool estaVazio(VetorInt *vetor);
void liberarVetorInt(VetorInt *vetor);
int removerItemVetorPorPosicao(VetorInt *vetor, int posicao);

#endif