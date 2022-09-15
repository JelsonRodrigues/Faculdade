#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include "vetorInt.h"
#include "grafo.h"

#define NUMERO_ITERACOES_ALGORITMO_APROXIMATIVO 10000
#define ID_GRAFO 1

// TODO: implementar o algoritmo de blossom

/* 
    A velocidade do programa será muito maior se eu implementar o vetor de vértices (grafo)
    e o vetor de arestar (vertice) de forma ordenada e realizar as buscas por procura binária
    Pra fazer isso é bem fácil na verdade, após a leitura dos valores do arquivo é só rodar um
    quicksort, ou alterar as funções de inserir aresta e vertice para inserir de forma ordenada.
    
    Ou se eu souber de antemão o número máximo de vértices e/ou de arestas eu possso implementar
    como uma hashtable que a velocidade seria maior ainda (a maior de todas na verdade)
    Na verdade como eu leio do arquivo o grafo e a cada linha/coluna é um vértice, dá pra mim
    criar uma hash pequena de início e como os valores de vértice são sempre incrementais (sempre diferentes),
    quando estiver enchendo a tabela eu aumento, não vai haver colisão neste caso. 
    Já para as arestas não é bem assim porque os valores não são incrementais (não necessariamente).
*/

// Funções do programa
typedef enum {LER_VERTICES_DE_ARQUIVO=1, MOSTRAR_GRAFO, MENOR_CICLO_HAMILTONIANO_EXATO, MENOR_CICLO_HAMILTONIANO_APROXIMATIVO, SAIR} OPCAO_MENU;
OPCAO_MENU menu();
int lerNumeroInteiro();

// Opção do menu 
void lerArquivo(Grafo **grafo, char *local_arquivo, char delimitador[]);
void mostrarGrafo(Grafo *grafo);
void menorCicloHamiltonianoExato(Grafo *grafo);
void menorCicloHamiltonianoAproximativo(Grafo *grafo);
void sair(Grafo *grafo);
void mostrarAjuda();
bool validaParametros(int argc, char **argv);

int main(int argc, char **argv) {
    Grafo *grafo = criaGrafo(ID_GRAFO);
    if (grafo == NULL) {
        exit(EXIT_FAILURE);
    }
    
    srand(time(NULL));

    // Verificação dos parâmetros
    if (validaParametros(argc, argv)){
        char *arquivo = argv[1] + strlen("--path=");
        char *delimitador =  argv[2] + strlen("--delim=");
        char algoritmo = argv[3][strlen("--alg=")];

        lerArquivo(&grafo, arquivo, delimitador);

        if (algoritmo == 'e'){
            menorCicloHamiltonianoExato(grafo);
        }
        else {
            menorCicloHamiltonianoAproximativo(grafo);
        }
        sair(grafo);
    }
    else {
        printf("\nNÃO FORAM PASSADOS PARÂMETROS OU OS PARÂMETROS CONTÉM ERRO\n");
        mostrarAjuda(argv[0]);
        printf("ENTRANDO NO MODO INTERATIVO");
        
        while (true){
            switch (menu())
            {
            case LER_VERTICES_DE_ARQUIVO:
                lerArquivo(&grafo, NULL, NULL);
                break;
            case MOSTRAR_GRAFO:
                mostrarGrafo(grafo);
                break;
            case MENOR_CICLO_HAMILTONIANO_EXATO:
                menorCicloHamiltonianoExato(grafo);
                break;
            case MENOR_CICLO_HAMILTONIANO_APROXIMATIVO:
                menorCicloHamiltonianoAproximativo(grafo);
                break;  
            case SAIR:
                sair(grafo);
                break;
            default:
                printf("\nOPCAO ERRADA");
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}

void sair(Grafo *grafo){
    printf("\n");
    liberaGrafo(grafo);
    exit(0);
}

OPCAO_MENU menu(){
    OPCAO_MENU escolha;
    do {
        printf("\n");
        printf("\n\t%d LER VERTICES DE ARQUIVO", LER_VERTICES_DE_ARQUIVO);
        printf("\n\t%d MOSTRAR GRAFO", MOSTRAR_GRAFO);
        printf("\n\t%d MOSTRAR MENOR CICLO HAMILTONIANO (TSP) COM ALGORITMO EXATO", MENOR_CICLO_HAMILTONIANO_EXATO);
        printf("\n\t%d MOSTRAR MENOR CICLO HAMILTONIANO (TSP) COM ALGORITMO APROXIMATIVO", MENOR_CICLO_HAMILTONIANO_APROXIMATIVO);
        printf("\n\t%d SAIR", SAIR);
        printf("\n");
        escolha = lerNumeroInteiro();
    } while (escolha != LER_VERTICES_DE_ARQUIVO &&
             escolha != MOSTRAR_GRAFO &&
             escolha != MENOR_CICLO_HAMILTONIANO_EXATO &&
             escolha != MENOR_CICLO_HAMILTONIANO_APROXIMATIVO &&
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

void lerArquivo(Grafo **grafo, char *local_arquivo, char *delimitador){
    if (grafo != NULL) liberaGrafo(*grafo);
    bool local_arquivo_alocado_dinamicamente = false;
    bool delimitador_alocado_dinamicamente = false;
    if (local_arquivo == NULL){
        printf("\nDigite o local do arquivo: ");
        size_t tamanho_buffer = 0;
        int read = getline(&local_arquivo, &tamanho_buffer, stdin);
        local_arquivo[read - 1] = '\0'; // Remove \n
        local_arquivo_alocado_dinamicamente = true;
    }
    if (delimitador == NULL){
        printf("\nDigite o delimitador do arquivo: ");
        size_t tamanho_buffer = 0;
        int read = getline(&delimitador, &tamanho_buffer, stdin);
        delimitador[read - 1] = '\0'; // Remove \n
        delimitador_alocado_dinamicamente = true;
    }

    *grafo = lerDeArquivo(local_arquivo, delimitador, ID_GRAFO);
    if (*grafo == NULL){
        printf("\nERRO AO LER ARQUIVO!!!\n");
    }

    if (local_arquivo_alocado_dinamicamente){
        free(local_arquivo);
        local_arquivo = NULL;
    }
    if (delimitador_alocado_dinamicamente){
        free(delimitador);
        delimitador = NULL;
    }
}

void mostrarGrafo(Grafo *grafo){
    printf("\nIMPRIMINDO O GRAFO\n");
    imprimeGrafo(grafo);
    printf("\n");
}

// Este algoritmo atualmente implementa a solucao 2-aproximativo,
// A complexidade de tempo e a complexidade do algoritmo de kruskal O(A*A*V) e a complexidade de espaco e O(n)
void menorCicloHamiltonianoAproximativo(Grafo *grafo){
    if (grafo == NULL) return;
    if (grafo->numero_vertices == 0) return;
    
    float peso_grafo = 0.0;
    float menor_peso = __FLT_MAX__;
    float maior_peso = __FLT_MIN__;
    for (int c = 0; c < NUMERO_ITERACOES_ALGORITMO_APROXIMATIVO; c++){
        Grafo *mst = kruskal(grafo);
        
        int inicial = rand() % mst->numero_vertices;
        VetorInt vertices_percorridos = visitarVertices(mst, mst->vertices[inicial]);
        
        Grafo *ciclo_hamiltoniano = recriaGrafo(grafo, &vertices_percorridos);

        peso_grafo = pesoTotalGrafo(ciclo_hamiltoniano) / (float) 2.0;
        if (peso_grafo < menor_peso){
            menor_peso = peso_grafo;
            printf("\n MENOR = %.2f", menor_peso);
        }
        if (peso_grafo > maior_peso){
            maior_peso = peso_grafo;
            printf("\n MAIOR = %.2f", maior_peso);
        }

        liberaGrafo(ciclo_hamiltoniano);
        liberarVetorInt(&vertices_percorridos);
        liberaGrafo(mst);
    }

    printf("\n\n");
    printf("\n\tMENOR: %f", menor_peso);
    printf("\n\tMAIOR: %f", maior_peso);
    printf("\n\n");
}

// Este algoritmo tem complexidade de tempo O(n!) e de espaco O(n) 
void menorCicloHamiltonianoExato(Grafo *grafo){
    if (grafo == NULL || grafo->numero_vertices == 0) return;
    
    printf("\n\n\nRODANDO O ALGORITMO EXATO!!!");

    VetorInt indices_percorridos = criaVetorInt();
    float min = __FLT_MAX__;
    float max = __FLT_MIN__;
    
    /* Utilizar a forma recursiva - a a mais lenta */

    // int verticeInicial = rand() % grafo->numero_vertices;
    // adicionaItemVetor(&indices_percorridos, verticeInicial);
    // percorrerTodosCaminhos(grafo, verticeInicial, &indices_percorridos, &min, &max);


    /* Utilizar o algoritmo de Heap - mais rápido que a forma recursiva */

    // for (int c = 0; c < grafo->numero_vertices; c++){
    //     adicionaItemVetor(&indices_percorridos, c);
    // }
    // GrafoMatriz grafo_matriz = converteGrafoParaGrafoMatriz(grafo);
    // percorreTodosCaminhosGrafoMatrizUsandoHeap(&grafo_matriz, &indices_percorridos, &min, &max, grafo_matriz.numero_vertices);
    // liberaGrafoMatriz(grafo_matriz);


    /* Utilizar ordem lexicográfica - a forma mais rápida */
    // for (int c = 0; c < grafo->numero_vertices; c++){
    //     adicionaItemVetor(&indices_percorridos, c);
    // }

    // GrafoMatriz grafo_matriz = converteGrafoParaGrafoMatriz(grafo);
    // percorreTodosCaminhosGrafoMatriz(&grafo_matriz, &indices_percorridos, &min, &max);
    // liberaGrafoMatriz(grafo_matriz);

    /* Quebrando os testes em threads usando ordem lexicografica, talvez a forma mais rapida de todas */

    for (int c = 0; c < grafo->numero_vertices; c++){
        adicionaItemVetor(&indices_percorridos, c);
    }
    
    // int numero_threads = 4;
    // VetorInt ordem_n[numero_threads];
    // float resultados[numero_threads][2];
    // GrafoMatriz grafo_matriz = converteGrafoParaGrafoMatriz(grafo);
    // long long unsigned int testes_por_thread = fatorial(grafo_matriz.numero_vertices-1);

    // for (int c = 0; c <=numero_threads; c++){
    //     ordem_n[c] = ordemLexicograficaN(indices_percorridos, (testes_por_thread) * c);
    //     resultados[c][0] = min;
    //     resultados[c][0] = max;

    //     Como colocar isso em cada thread????
    //     testaCaminhosInRange(&grafo_matriz, &ordem_n[c], &resultados[c][0], &resultados[c][1], testes_por_thread);
    // }
    // liberaGrafoMatriz(grafo_matriz);

    // for (int c = 0; c <=numero_threads; c++){
    //     if (resultados[c][0] < min) min = resultados[c][0];
    //     if (resultados[c][1] > min) min = resultados[c][1];
    //     liberarVetorInt(&ordem_n[c]);
    // }
        

    VetorInt ordem_n = ordemLexicograficaN(indices_percorridos, 1);
    GrafoMatriz grafo_matriz = converteGrafoParaGrafoMatriz(grafo);
    testaCaminhosInRange(&grafo_matriz, &ordem_n, &min, &max, fatorial(grafo_matriz.numero_vertices));
    liberaGrafoMatriz(grafo_matriz);
    liberarVetorInt(&ordem_n);

    
    liberarVetorInt(&indices_percorridos);
    printf("\nMENOR CAMINHO -> %f", min);
    printf("\nMAIOR CAMINHO -> %f", max);
}

void mostrarAjuda(char *nome_programa){
    printf("\n");
    printf("A forma de uso do programa é a seguinte\n");
    printf("%s --path=caminho_arquivo.csv --delim='delimitador' --alg=algoritmo\n", nome_programa);
    printf("As opções de algoritmo disponíveis são: \n");
    printf("\t-e  : algoritmo exato\n");
    printf("\t-a  : algoritmo aproximativo\n");
    printf("O delimitador deve ser um caractere e passado entre aspas simples ou duplas\n");
    printf("\n");
}

bool validaParametros(int argc, char **argv){
    if (argc == 4) {
        if ((strcmp(argv[1], "--path=") > 0) &&
            (strcmp(argv[2], "--delim=") > 0) &&
            (strcmp(argv[3], "--alg=") > 0))
            {
            return true;
        }
    }
    return false;
}