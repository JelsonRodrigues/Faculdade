#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct _chave {
    int numero;
} Chave;

typedef struct _nodo {
    Chave item;
    struct _nodo *direita;
    struct _nodo *esquerda;
} Nodo;

bool inserir(Nodo **raiz, Chave item);
Nodo **procurar(Nodo **raiz, Chave item);
bool apagar(Nodo **raiz, Chave item);
void imprimir(Nodo *raiz);
void liberarArvore(Nodo **raiz);
int FatorBalanceamento(Nodo *raiz);
int altura(Nodo *raiz);
Nodo **menor(Nodo **raiz);
Nodo **maior(Nodo **raiz);
bool eFolha(Nodo *raiz);
bool eArvoreAVL(Nodo *raiz);
void rebalancear(Nodo **raiz);

void rotacaoSimplesEsquerda(Nodo **raiz);
void rotacaoDuplaEsquerda(Nodo **raiz);
void rotacaoSimplesDireita(Nodo **raiz);
void rotacaoDuplaDireita(Nodo **raiz);

void preOrdem(Nodo *raiz);
void central(Nodo *raiz);
void posOrdem(Nodo *raiz);
void imprimeNodo(Nodo *raiz);

int main(){
    Nodo *raiz = NULL;

    // Inserção aleatória
    unsigned int numero_nos = 0;
    printf("\nDigite quantos nos: ");
    while (scanf("%u", &numero_nos) == 0){
        stdin->_IO_read_ptr = stdin->_IO_read_end;
    }
    srand(time(0));
    for (unsigned int c = 0; c < numero_nos; c++){
        int numero = rand() % numero_nos;
        printf("Numero %u -> %d\n",c, numero);
        inserir(&raiz, (Chave) {numero});
    }
    printf("\n-------ARVORE ALEATORIA-------\n");
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    liberarArvore(&raiz);

    // Casos do StackOverflow
    // Caso 1a
    printf("\n-------StackOverflow caso 1a-------\n");
    inserir(&raiz, (Chave) {20});
    inserir(&raiz, (Chave) {4});
    inserir(&raiz, (Chave) {15});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    liberarArvore(&raiz);

    // Caso 2a 
    printf("\n-------StackOverflow caso 2a-------\n");
    inserir(&raiz, (Chave) {20});
    inserir(&raiz, (Chave) {4});
    inserir(&raiz, (Chave) {26});
    inserir(&raiz, (Chave) {3});
    inserir(&raiz, (Chave) {9});
    inserir(&raiz, (Chave) {15});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    liberarArvore(&raiz);

    // Caso 3a
    printf("\n-------StackOverflow caso 3a-------\n");
    inserir(&raiz, (Chave) {20});
    inserir(&raiz, (Chave) {4});
    inserir(&raiz, (Chave) {26});
    inserir(&raiz, (Chave) {3});
    inserir(&raiz, (Chave) {9});
    inserir(&raiz, (Chave) {21});
    inserir(&raiz, (Chave) {30});
    inserir(&raiz, (Chave) {2});
    inserir(&raiz, (Chave) {7});
    inserir(&raiz, (Chave) {11});
    inserir(&raiz, (Chave) {15});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    liberarArvore(&raiz);

    // Caso 1b
    printf("\n-------StackOverflow caso 1b-------\n");
    inserir(&raiz, (Chave) {20});
    inserir(&raiz, (Chave) {4});
    inserir(&raiz, (Chave) {8});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    liberarArvore(&raiz);

    // Caso 2b 
    printf("\n-------StackOverflow caso 2b-------\n");
    inserir(&raiz, (Chave) {20});
    inserir(&raiz, (Chave) {4});
    inserir(&raiz, (Chave) {26});
    inserir(&raiz, (Chave) {3});
    inserir(&raiz, (Chave) {9});
    inserir(&raiz, (Chave) {8});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    liberarArvore(&raiz);

    // Caso 3b
    printf("\n-------StackOverflow caso 3b-------\n");
    inserir(&raiz, (Chave) {20});
    inserir(&raiz, (Chave) {4});
    inserir(&raiz, (Chave) {26});
    inserir(&raiz, (Chave) {3});
    inserir(&raiz, (Chave) {9});
    inserir(&raiz, (Chave) {21});
    inserir(&raiz, (Chave) {30});
    inserir(&raiz, (Chave) {2});
    inserir(&raiz, (Chave) {7});
    inserir(&raiz, (Chave) {11});
    inserir(&raiz, (Chave) {8});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    liberarArvore(&raiz);

    // Caso apagar 1
    printf("\n-------StackOverflow caso apagar 1-------\n");
    inserir(&raiz, (Chave) {2});
    inserir(&raiz, (Chave) {1});
    inserir(&raiz, (Chave) {4});
    inserir(&raiz, (Chave) {3});
    inserir(&raiz, (Chave) {5});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    apagar(&raiz, (Chave) {1});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    liberarArvore(&raiz);

    // Caso apagar 2
    printf("\n-------StackOverflow caso apagar 2-------\n");
    inserir(&raiz, (Chave) {6});
    inserir(&raiz, (Chave) {2});
    inserir(&raiz, (Chave) {9});
    inserir(&raiz, (Chave) {1});
    inserir(&raiz, (Chave) {4});
    inserir(&raiz, (Chave) {8});
    inserir(&raiz, (Chave) {11});
    inserir(&raiz, (Chave) {3});
    inserir(&raiz, (Chave) {5});
    inserir(&raiz, (Chave) {7});
    inserir(&raiz, (Chave) {11});
    inserir(&raiz, (Chave) {12});
    inserir(&raiz, (Chave) {13});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    apagar(&raiz, (Chave) {1});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    liberarArvore(&raiz);

    // Caso apagar 3
    printf("\n-------StackOverflow caso apagar 3-------\n");
    inserir(&raiz, (Chave) {5});
    inserir(&raiz, (Chave) {2});
    inserir(&raiz, (Chave) {8});
    inserir(&raiz, (Chave) {1});
    inserir(&raiz, (Chave) {3});
    inserir(&raiz, (Chave) {7});
    inserir(&raiz, (Chave) {10});
    inserir(&raiz, (Chave) {4});
    inserir(&raiz, (Chave) {6});
    inserir(&raiz, (Chave) {9});
    inserir(&raiz, (Chave) {11});
    inserir(&raiz, (Chave) {12});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    apagar(&raiz, (Chave) {1});
    imprimir(raiz);
    printf("E AVL? %s\n", eArvoreAVL(raiz)? "True" : "False");
    liberarArvore(&raiz);
    
    //liberarArvore(&raiz);
    printf("\n");
    return 0;
}

void imprimir(Nodo *raiz){
    printf("\n");
    preOrdem(raiz);
    //central(raiz);
    printf("\n");
}

void imprimeNodo(Nodo *raiz){
    if (raiz){
        printf("%d ", raiz->item.numero);
    }
}

void central(Nodo *raiz){
    if (raiz){
        central(raiz->esquerda);
        imprimeNodo(raiz);
        central(raiz->direita);
    }
}

void preOrdem(Nodo *raiz){
    if (raiz){
        imprimeNodo(raiz);
        preOrdem(raiz->esquerda);
        preOrdem(raiz->direita);
    }
}

void posOrdem(Nodo *raiz){
    if (raiz){
        posOrdem(raiz->esquerda);
        posOrdem(raiz->direita);
        imprimeNodo(raiz);
    }
}

void liberarArvore(Nodo **raiz){
    if (raiz == NULL) {  // Ponteiro invalido
        return;
    }
    if (*raiz) {
        liberarArvore(&(*raiz)->esquerda);
        liberarArvore(&(*raiz)->direita);
        free((void *) *raiz);
        *raiz = NULL;
    }
}

bool inserir(Nodo **raiz, Chave item){
    if (raiz == NULL){
        return false;
    }
    if (*raiz == NULL){
        *raiz = (Nodo *) malloc(sizeof(**raiz));
        if (*raiz == NULL){
            printf("\nNao foi possivel alocar memoria.\n");
            return false;
        }
        (*raiz)->esquerda = NULL;
        (*raiz)->direita = NULL;
        (*raiz)->item.numero = item.numero;
        return true;
    }
    if (item.numero < (*raiz)->item.numero){
        inserir(&(*raiz)->esquerda, item);
    }
    else{
        inserir(&(*raiz)->direita, item);
    }
    rebalancear(raiz);
    return true;
}

Nodo **procurar(Nodo **raiz, Chave item){
    if (raiz == NULL){ // Ponteiro inválido
        return raiz;
    }
    while (*raiz != NULL && (*raiz)->item.numero != item.numero){
        if ( item.numero < (*raiz)->item.numero){
            raiz = &(*raiz)->esquerda;
        }
        else {
            raiz = &(*raiz)->direita;
        }
    }
    return raiz;
}

bool apagar(Nodo **raiz, Chave item){
    if (raiz == NULL){
        return false;
    }
    if (*raiz == NULL) {
        return false;
    }
    bool resultado;
    if ((*raiz)->item.numero == item.numero) {
        if ((*raiz)->direita != NULL){
            Nodo **substituto = menor(&(*raiz)->direita);
            (*raiz)->item.numero = (*substituto)->item.numero;
            Nodo *substituto_direita = (*substituto)->direita;
            free(*substituto);
            // Com a função menor é garantido que não existe nenhum outro 
            // Item do lado esquerdo, porém pode ou não existir do lado direito
            (*substituto) = substituto_direita;
        }
        else if ((*raiz)->esquerda != NULL) {
            Nodo **substituto = maior(&(*raiz)->esquerda);
            (*raiz)->item.numero = (*substituto)->item.numero;
            Nodo *substituto_esquerda = (*substituto)->esquerda;
            free(*substituto);
            *substituto = substituto_esquerda;
        }
        else {  // O item a ser excluido é uma folha
            free(*raiz);
            *raiz = NULL;
        }
        resultado = true;
    }
    else if (item.numero < (*raiz)->item.numero){
        resultado = apagar(&(*raiz)->esquerda, item);
    }
    else {
        resultado = apagar(&(*raiz)->direita, item);
    }

    rebalancear(raiz);
    return resultado;
}

int FatorBalanceamento(Nodo *raiz){
    if (raiz == NULL) return 0;
    return altura(raiz->esquerda) - altura(raiz->direita);
}

int altura(Nodo *raiz){
    if (raiz){
        int altura_sub_arvore_esquerda;
        int altura_sub_arvore_direita;
        // É somado 1 por que a cada chamada da função está descendo um nível,
        // ou seja, a altura aumenta em 1
        altura_sub_arvore_esquerda = altura(raiz->esquerda) + 1;
        altura_sub_arvore_direita = altura(raiz->direita) + 1;
        if (altura_sub_arvore_esquerda > altura_sub_arvore_direita){
            return altura_sub_arvore_esquerda;
        }
        else {
            return altura_sub_arvore_direita;
        }
    }
    else {
        return 0;
    }
}
Nodo **menor(Nodo **raiz){
    if (raiz == NULL) {  // Ponteiro inválido
        return raiz;
    }
    if (*raiz == NULL) {    // Árvore vazia
        return raiz;
    }
    while ((*raiz)->esquerda) { // Percorre a árvore até chegar no menor
        raiz = &(*raiz)->esquerda;
    }
    return raiz;
}

Nodo **maior(Nodo **raiz){
    if (raiz == NULL) {  // Ponteiro inválido
        return raiz;
    }
    if (*raiz == NULL) {    // Árvore vazia
        return raiz;
    }
    while ((*raiz)->direita) {  // Percorre a árvore até chegar no maior
        raiz = &(*raiz)->direita;
    }
    return raiz;
}

bool eFolha(Nodo *raiz){
    if (raiz == NULL) return false;
    return (bool) (raiz->esquerda == NULL && raiz->direita == NULL);
}

void rebalancear(Nodo **raiz){
    int FB = FatorBalanceamento(*raiz);
    if (FB < -1){
        if (FatorBalanceamento((*raiz)->direita) <= 0){
            rotacaoSimplesEsquerda(raiz);
        }
        else {
            rotacaoDuplaEsquerda(raiz);
        }
    }
    else if (FB > 1){
        if (FatorBalanceamento((*raiz)->esquerda) >= 0){
            rotacaoSimplesDireita(raiz);
        }
        else {
            rotacaoDuplaDireita(raiz);
        }
    }
}

void rotacaoDuplaDireita(Nodo **raiz){
    if (raiz == NULL){
        return;
    }
    rotacaoSimplesEsquerda(&(*raiz)->esquerda);
    rotacaoSimplesDireita(raiz);
}

void rotacaoDuplaEsquerda(Nodo **raiz){
    if (raiz == NULL){
        return;
    }
    rotacaoSimplesDireita(&(*raiz)->direita);
    rotacaoSimplesEsquerda(raiz);
}

void rotacaoSimplesDireita(Nodo **raiz){
    if (raiz == NULL){
        return;
    }
    if (*raiz){
        Nodo *novo_pai = (*raiz)->esquerda;
        (*raiz)->esquerda = novo_pai->direita;
        novo_pai->direita = (*raiz);
        *raiz = novo_pai;
    }
}
void rotacaoSimplesEsquerda(Nodo **raiz){
    if (raiz == NULL){
        return;
    }
    if (*raiz){
        Nodo *novo_pai = (*raiz)->direita;
        (*raiz)->direita = novo_pai->esquerda;
        novo_pai->esquerda = *raiz;
        *raiz = novo_pai;
    }
}

bool eArvoreAVL(Nodo *raiz){
    if (raiz == NULL){  // Árvore vazia
        return true;
    }
    if (!eArvoreAVL(raiz->esquerda)){
        return false;
    }
    if (!eArvoreAVL(raiz->direita)){
        return false;
    }
    int fb = FatorBalanceamento(raiz);
    if (fb < -1 || fb > 1){
        return false;
    }
    else {
        return true;
    }
}