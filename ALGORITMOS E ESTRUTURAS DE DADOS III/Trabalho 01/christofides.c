#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include "vetorInt.h"
#include "grafo.h"

// TODO: Corrigir leaks de memória e implementar o algoritmo de blossom

// REMOVER LEAKS DE MEMÓRIA

// Funções do programa
typedef enum {LER_VERTICES_DE_ARQUIVO=1, MOSTRAR_GRAFO, MENOR_CICLO_HAMILTONIANO, SAIR} OPCAO_MENU;
OPCAO_MENU menu();
int lerNumeroInteiro();

// Opção do menu 
void lerArquivo(Grafo *grafo);
void mostrarGrafo(Grafo *grafo);
void menorCicloHamiltoniano(Grafo *grafo);
void sair(Grafo *grafo);

int main() {
    Grafo *grafo = criaGrafo();
    if (grafo == NULL) {
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    OPCAO_MENU escolha_usuario;
    while (true){
        escolha_usuario = menu();

        switch (escolha_usuario)
        {
        case LER_VERTICES_DE_ARQUIVO:
            lerArquivo(grafo);
            break;
        case MOSTRAR_GRAFO:
            mostrarGrafo(grafo);
            break;
        case MENOR_CICLO_HAMILTONIANO:
            menorCicloHamiltoniano(grafo);
            break;
        case SAIR:
            sair(grafo);
            break;
        default:
            printf("\nOPCAO ERRADA");
            break;
        }

    }

    return EXIT_SUCCESS;
}

void sair(Grafo *grafo){
    liberaGrafo(grafo);
    exit(0);
}

OPCAO_MENU menu(){
    OPCAO_MENU escolha;
    do {
        printf("\n");
        printf("\n\t%d LER VERTICES DE ARQUIVO", LER_VERTICES_DE_ARQUIVO);
        printf("\n\t%d MOSTRAR GRAFO", MOSTRAR_GRAFO);
        printf("\n\t%d MOSTRAR MENOR CICLO HAMILTONIANO (TSP)", MENOR_CICLO_HAMILTONIANO);
        printf("\n\t%d SAIR", SAIR);
        printf("\n");
        escolha = lerNumeroInteiro();
    } while (escolha != LER_VERTICES_DE_ARQUIVO &&
             escolha != MOSTRAR_GRAFO &&
             escolha != MENOR_CICLO_HAMILTONIANO &&
             escolha != SAIR
    );
    return escolha;
}

int lerNumeroInteiro(){
    int numero = 0;
    while (scanf("%d%*c", &numero) == 0) {
        while (getc(stdin) != '\n');
    }
    return numero;
}

void lerArquivo(Grafo *grafo){
    liberaGrafo(grafo);
    /*
    delimitador = " "
    arquivo = tsp1_253

    ------Com erro de leitura-------
    delimitador = ?
    arquivo = tsp2_1248
    
    delimitador = ?
    arquivo = tsp3_1194
    --------------------------------

    delimitador = "\t"
    arquivo = tsp4_7013

    delimitador = " "
    arquivo = tsp5_27603
    */

    char delimitador[] = " ";
    grafo = lerDeArquivo("./tsp1_253.txt", delimitador);
    if (grafo == NULL){
        printf("\nERRO AO LER ARQUIVO!!!\n");
    }
}

void mostrarGrafo(Grafo *grafo){
    printf("\nIMPRIMINDO O GRAFO\n");
    imprimeGrafo(grafo);
    printf("\n");
}

void menorCicloHamiltoniano(Grafo *grafo){
    float peso_grafo = 0.0;
    float menor_peso = 0.0;
    float maior_peso = 0.0;
    for (int c = 0; c < 1000; c++){
        Grafo *mst = kruskal(grafo);
        //imprimeGrafo(mst);

        //printf("\n\nPeso do grafo original e de %.2f\n", pesoTotalGrafo(grafo) / (float) 2.0);
        //printf("\n\nPeso da arvore geradora minima e de %.2f\n", pesoTotalGrafo(mst) / (float) 2.0);

        int inicial = rand() % mst->numero_vertices;
        VetorInt vertices_percorridos = visitarVertices(mst, mst->vertices[inicial]);
        
        Grafo *ciclo_hamiltoniano = recriaGrafo(grafo, &vertices_percorridos);
        //imprimeGrafo(ciclo_hamiltoniano);

        peso_grafo = pesoTotalGrafo(ciclo_hamiltoniano) / (float) 2.0;
        if (peso_grafo < menor_peso || c == 0){
            menor_peso = peso_grafo;
            printf("\n MENOR = %.2f", menor_peso);
        }
        if (peso_grafo > maior_peso || c == 0){
            maior_peso = peso_grafo;
            printf("\n MAIOR = %.2f", maior_peso);
        }
        //printf("\n\nPeso e de %.2f\n", pesoTotalGrafo(ciclo_hamiltoniano) / (float) 2.0);
        
        liberaGrafo(ciclo_hamiltoniano);
        liberarVetorInt(&vertices_percorridos);
        liberaGrafo(mst);

    }

    printf("\n\n");
    printf("\n\tMENOR: %f", menor_peso);
    printf("\n\tMAIOR: %f", maior_peso);
    printf("\n\n");


    printf("\n\n\nRODANDO O ALGORITMO EXATO!!!");
    
    int pos_vertice_inicial = 0;
    VetorInt indices_percorridos = criaVetorInt();
    adicionaItemVetor(&indices_percorridos, pos_vertice_inicial);
    float min = __FLT_MAX__;
    float max = __FLT_MIN__;
    percorrerTodosCaminhos(grafo, pos_vertice_inicial, &indices_percorridos, &min, &max);
    
    liberarVetorInt(&indices_percorridos);
    printf("\nMENOR CAMINHO -> %f", min);
    printf("\nMAIOR CAMINHO -> %f", max);
}