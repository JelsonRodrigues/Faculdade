#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef char * string;

// Nodo
typedef struct {
    string nome;
} Nodo;


// Aresta
typedef struct {
    Nodo *n1;
    Nodo *n2;
    float peso;
} Aresta;

typedef struct _Lista {
    struct _Lista *anterior;
    void *dado;
    struct _Lista *proximo;
} Lista;

typedef struct {
    Lista *nos;
    Lista *arestas;
} Grafo;

// Protótipos das funções
Nodo *criaNodo(string nome);
void imprimirNodo(Nodo *nodo_imprimir);
void imprimirNodoLista(void *nodo_imprimir);
void liberarNodo(Nodo *nodo_liberar);
void liberarNodoLista(void *nodo_liberar);

Lista *criaLista();
void adicionaItemLista(Lista **lista, void *item);
Lista *primeiroItem(Lista *lista);
Lista *ultimoItem(Lista *lista);
void imprimirLista(Lista *lista, void (*funcao_imprime)(void *));
void liberarLista(Lista **lista, void (*fucao_libera)(void *));


// Função principal
int main() {
    Grafo g1;
    g1.nos = criaLista();
    g1.arestas = criaLista();

    adicionaItemLista(&g1.nos, (void *) criaNodo("Maria"));
    adicionaItemLista(&g1.nos, (void *) criaNodo("Pedro"));
    adicionaItemLista(&g1.nos, (void *) criaNodo("Luiz"));
    adicionaItemLista(&g1.nos, (void *) criaNodo("Joana"));

    imprimirLista(g1.nos, &imprimirNodoLista);
    printf("\n");
    liberarLista(&g1.nos, &liberarNodoLista);
    liberarLista(&g1.arestas, &liberarNodoLista);   // Criar uma função para liberar arestas

    return 0;
}

Nodo *criaNodo(string nome) {
    if (!nome) return (Nodo *) NULL;
    
    Nodo *novo_nodo = (Nodo *) malloc(sizeof(Nodo));
    if (!novo_nodo) return (Nodo *) NULL;
    
    novo_nodo->nome = nome;

    return novo_nodo;
}

void imprimirNodo(Nodo *nodo_imprimir){
    if (nodo_imprimir) {
        printf("\nNome: %s", nodo_imprimir->nome);
    }
}
void imprimirNodoLista(void *nodo_imprimir){
    imprimirNodo((Nodo *) nodo_imprimir);
}
void liberarNodo(Nodo *nodo_liberar) {
    if (nodo_liberar) {
        free(nodo_liberar);
    }
}
void liberarNodoLista(void *nodo_liberar){
    liberarNodo((Nodo *) nodo_liberar);
}

Lista *criaLista(){
    Lista *nova_lista = (Lista *) malloc(sizeof(Lista));
    if (!nova_lista) return (Lista *) NULL;

    nova_lista->anterior = NULL;
    nova_lista->dado = NULL;
    nova_lista->proximo = NULL;

    return nova_lista;
}

void adicionaItemLista(Lista **lista, void *item){
    if (!(lista && *lista)) return;

    Lista *l1 = *lista;
    // Primeiro item da lista
    if (!l1->anterior && !l1->proximo && !l1->dado) {
        l1->dado = item;
    }
    else {
        Lista *novo_item = criaLista();
        if (!novo_item) return;
        // Insere o valor ao novo item
        novo_item->dado =  item;

        // Adiciona o item na lista
        novo_item->anterior = l1;
        novo_item->proximo = l1->proximo;

        l1->proximo = novo_item;
        if (novo_item->proximo) novo_item->proximo->anterior = novo_item;

        
    }
}
Lista *primeiroItem(Lista *lista){
    if (lista){
        while (lista->anterior){
            lista = lista->anterior;
        }
        return lista;
    }
    return NULL;
}

Lista *ultimoItem(Lista *lista) {
    if (lista){
    while (lista->proximo){
        lista = lista->proximo;
    }
    return lista;
    }
    return NULL;
}

void imprimirLista(Lista *lista, void (*funcao_imprime)(void *)){
    Lista *item = primeiroItem(lista);
    while (item) {
        funcao_imprime(item->dado);
        item = item->proximo;
    }

}
void liberarLista(Lista **lista, void (*fucao_libera)(void *)){
    Lista *item = primeiroItem(*lista);
    Lista *proximo_item = NULL;
    while (item)
    {
        proximo_item = item->proximo;
        fucao_libera(item->dado);
        free(item);
        item = proximo_item;
    }
    lista = NULL;
}