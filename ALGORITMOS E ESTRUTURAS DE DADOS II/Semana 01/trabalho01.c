#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
<<<<<<< HEAD
#include <string.h>
=======
#include <stdint.h>
>>>>>>> refs/remotes/origin/main

#define NUMERO_MAXIMO_VERTICES 20

/* Outros */
typedef unsigned int uint;
int lerNumeroInteiro();
float lerNumeroReal();

/* Menu */
typedef enum { ADICIONAR_NO = 1, MOSTRAR_GRAFO = 2, ALTERAR_ARESTAS = 3,  SAIR = 4 } OpcoesMenu;
OpcoesMenu menu();
void limparEntrada(FILE *entrada);

/* Nodo */
typedef struct {
    char *nome_cidade;
} Nodo;
Nodo criaNodo(char *nome);
void imprimeNodo(Nodo *no);
Nodo lerNodo();

/* Grafo */
typedef struct {
    Nodo *nos;
<<<<<<< HEAD
    uint numero_nos;
=======
    uint8_t numero_nos; // Já que o máximo é 20
>>>>>>> refs/remotes/origin/main
    float **pesos;
} Grafo;
Grafo *criaGrafo();
void adicionarNo(Grafo *grafo);
bool nodoPertenceAoGrafo(Grafo *grafo, Nodo no);
void alterarConexoesNo(Grafo *grafo);
void imprimirGrafo(Grafo *g1);
void liberarGrafo(Grafo *g1);
<<<<<<< HEAD
int maiorStringTamanho(Nodo *nos, int numero_nos);
=======
>>>>>>> refs/remotes/origin/main


/* Modelagem do grafo como uma matriz */
int main() {
    Grafo *g1 = criaGrafo();
    if (!g1) {
        printf("Não foi possível criar o grafo!\n");
        return EXIT_FAILURE;
    }

    OpcoesMenu escolha_menu = 0;
    while (true) {
        escolha_menu = menu();
        switch (escolha_menu)
        {
        case ADICIONAR_NO:
            adicionarNo(g1);
            break;
        case MOSTRAR_GRAFO:
            imprimirGrafo(g1);
            break;
        case ALTERAR_ARESTAS:
            alterarConexoesNo(g1);
            break;
        case SAIR:
            liberarGrafo(g1);
            printf("SAINDO...\n");
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("Opção errada!");
            break;
        }
    }

    return EXIT_SUCCESS;
}

int lerNumeroInteiro(){
    int numero = 0;
    while (scanf("%d%*c", &numero) == 0) {
        limparEntrada(stdin);
    }
    return numero;
}

float lerNumeroReal() { 
    float numero = 0.0f;
    while (scanf("%f%*c", &numero) == 0) {
        limparEntrada(stdin);
    }
    return numero;
}

Grafo *criaGrafo() {
    Grafo *temp = (Grafo *) malloc(sizeof(Grafo));
    if (temp) {
        temp->nos = NULL;
        temp->numero_nos = 0;
        temp->pesos = NULL;
    }
    return temp;
}

OpcoesMenu menu(){
    OpcoesMenu escolha;
    do {
        printf("\n");
        printf("\nDIGITE UMA OPÇÃO");
        printf("\n\t%d. ADICIONAR NÓ AO GRAFO", ADICIONAR_NO);
        printf("\n\t%d. MOSTRAR O GRAFO", MOSTRAR_GRAFO);
        printf("\n\t%d. ALTERAR ARESTAS", ALTERAR_ARESTAS);
        printf("\n\t%d. SAIR", SAIR);
        printf("\n");
        escolha = lerNumeroInteiro();
    } while (escolha != ADICIONAR_NO &&
            escolha != MOSTRAR_GRAFO && 
            escolha != ALTERAR_ARESTAS &&
            escolha != SAIR);
    
    return escolha;
}

void limparEntrada(FILE *entrada){
    while (getc(entrada) != EOF);
}

Nodo criaNodo(char *nome){
    Nodo temp = {.nome_cidade=nome};
    return temp;
}
Nodo lerNodo() {
    Nodo no;
    
    char *nome = NULL;
    size_t numero_caracteres = 0;
    
    printf("\nDigite o nome da cidade: ");
    getline(&nome, &numero_caracteres, stdin);

    // Remove o enter do final
    int tamanho_nome = strlen(nome);
    if (nome[tamanho_nome - 1] == '\n') {
        nome[tamanho_nome - 1] = '\0';
    }

    no.nome_cidade = nome;
    return no;
}

void adicionarNo(Grafo *grafo){
    if (grafo == NULL) return;
    if (grafo->numero_nos >= NUMERO_MAXIMO_VERTICES) {
        printf("Não é possível adicionar mais nodos ao grafo, o máximo é %d", NUMERO_MAXIMO_VERTICES);
        return;
    }

    Nodo nodo_adicionar = lerNodo();
    grafo->nos = realloc(grafo->nos, sizeof(Nodo) * (grafo->numero_nos +1));
    if (grafo->nos == NULL) {
        printf("Não foi possível adicionar o nó!");
        return;
    }
    grafo->nos[grafo->numero_nos] = nodo_adicionar;
    grafo->numero_nos++;

    grafo->pesos = realloc(grafo->pesos, sizeof(float *) * grafo->numero_nos);
    if (grafo->pesos == NULL) {
        printf("Não foi possível adicionar o nó!");
        return;
    }
    
    /*
    grafo->pesos[grafo->numero_nos-1] = calloc(grafo->numero_nos, sizeof(float));
    if (grafo->pesos[grafo->numero_nos-1] == NULL) {
        printf("Não foi possível adicionar o nó!");
        return;
    }
    */

    // Adiciona uma nova coluna em cada vetor
    for (uint c = 0; c < grafo->numero_nos; c++) {
        grafo->pesos[c] = realloc(grafo->pesos[c], sizeof(float) * grafo->numero_nos);
        if (grafo->pesos[c] == NULL) {
            printf("Não foi possível adicionar o nó!");
            return;
        }
        grafo->pesos[c][grafo->numero_nos -1] = 0.0;
    }

}
void imprimirGrafo(Grafo *g1) {
    if (g1){
        if (g1->numero_nos == 0) {
            printf("O grafo está vazio.\n");
            return;
        }

        int tamanho_maior_nome = maiorStringTamanho(g1->nos, g1->numero_nos);

        // Mostra o nome de cada coluna
        printf("%16s", " | ");
        for (uint c = 0; c < g1->numero_nos; c++){
            printf("%-13s | ", g1->nos[c].nome_cidade);
        }

        // Imprime cada linha e o conteúdo
        for (uint c = 0; c < g1->numero_nos; c++){
            printf("\n%+13s | ", g1->nos[c].nome_cidade);
            for (uint i = 0; i < g1->numero_nos; i++) {
                printf("%-13.3f | ", g1->pesos[c][i]);
            }
        }

    }
}

int maiorStringTamanho(Nodo *nos, int numero_nos) {
    int maior = 0;

    for (int c = 0; c < numero_nos; c++) {
        if (c == 0) {
            maior = strlen(nos[c].nome_cidade);
        }

        if (strlen(nos[c].nome_cidade) > maior) {
            maior = strlen(nos[c].nome_cidade);
        }

    }

    return maior;

}
void alterarConexoesNo(Grafo *grafo) {
    if (!grafo) return;
    for (uint c = 0; c < grafo->numero_nos; c++) {
        printf("\n%02u - %s", c+1,  grafo->nos[c].nome_cidade);
    }
    uint posicao_primeiro_no = 0;
    do {
        printf("\nDigite o número do primeiro nodo da conexão: ");
        posicao_primeiro_no = lerNumeroInteiro();
    } while (posicao_primeiro_no <= 0 ||  posicao_primeiro_no > grafo->numero_nos);
    
    uint posicao_segundo_no = 0;
    do {
        printf("\nDigite o número do segundo nodo da conexão: ");
        posicao_segundo_no = lerNumeroInteiro();
    } while (posicao_segundo_no <= 0 || posicao_segundo_no > grafo->numero_nos);

    printf("\n\nOs nodos selecionados foram");
    printf("\n%s", grafo->nos[posicao_primeiro_no-1].nome_cidade);
    printf("\n%s", grafo->nos[posicao_segundo_no-1].nome_cidade);
    printf("\nO valor da conexão atual entre estes nodos é %.03f", grafo->pesos[posicao_primeiro_no-1][posicao_segundo_no-1]);
    printf("\n");

    float nova_conexao = 0.0;
    do {
        printf("Agora digite o novo valor da conexão: ");
        nova_conexao = lerNumeroReal();
    } while (nova_conexao < 0.0);

    grafo->pesos[posicao_primeiro_no-1][posicao_segundo_no-1] = nova_conexao;

    return;
}

void liberarGrafo(Grafo *g1){
    if (g1){
        // Libera o espaço do nome da cidade
        for (uint c = 0; c < g1->numero_nos ; c++){
            free(g1->nos[c].nome_cidade);
        }
        free(g1->nos);
        g1->nos = NULL;
        for (uint c = 0; c < g1->numero_nos ; c++){
            free(g1->pesos[c]);
        }
        free(g1->pesos);
        g1->pesos = NULL;
        g1->numero_nos = 0;
        
        free(g1);
    }
}