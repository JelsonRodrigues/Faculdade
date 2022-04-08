#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define NUMERO_MAXIMO_VERTICES 20

/* Outros */
typedef unsigned int uint;
int lerNumeroInteiro();
float lerNumeroReal();

/* Menu */
typedef enum { ADICIONAR_NO = 1, MOSTRAR_GRAFO = 2, ALTERAR_ARESTAS = 3, MENOR_CAMINHO = 4, SAIR = 5 } OpcoesMenu;
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
// A estrutura do grafo é composta de um vetor de nós e uma matriz com os pesos
typedef struct {
    Nodo *nos;
    uint numero_nos;
    float **pesos;
} Grafo;
Grafo *criaGrafo();
void adicionarNo(Grafo *grafo);
bool nodoPertenceAoGrafo(Grafo *grafo, Nodo no);
void alterarConexoesNo(Grafo *grafo);
void imprimirGrafo(Grafo *g1);
void liberarGrafo(Grafo *g1);
uint maiorStringTamanho(Nodo *nos, int numero_nos);
int procuraNodo(Grafo *grafo, Nodo no);


/* Dijktra */
void mostrarMenorCaminho(Grafo *grafo);
typedef struct {
    bool vertice_aberto;
    int indice_vertice_anterior;
    float menor_caminho;
} AuxDijkstra;
AuxDijkstra *inicializaVetoresDijkstra(Grafo *grafo);
AuxDijkstra *Dijkstra(Grafo *grafo, int indice_vertice_inicial);
bool possuiVerticesAbertos(AuxDijkstra *aux_dijkstra, int total_nos);
int indiceVerticeAbertoComMenorEstimativa(AuxDijkstra *aux_dijkstra, int numero_nos);

typedef struct {
    int *verticesAdjacentes;
    int tamanho;
} VerticesAdjacentes;
VerticesAdjacentes verticesAdjacentes(Grafo *grafo, int posicao_no);



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
        case MENOR_CAMINHO:
            mostrarMenorCaminho(g1);
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
        printf("\n\t%d. PROCURAR MENOR CAMINHO", MENOR_CAMINHO);
        printf("\n\t%d. SAIR", SAIR);
        printf("\n");
        escolha = lerNumeroInteiro();
    } while (escolha != ADICIONAR_NO &&
            escolha != MOSTRAR_GRAFO && 
            escolha != ALTERAR_ARESTAS &&
            escolha != MENOR_CAMINHO && 
            escolha != SAIR);
    
    return escolha;
}

void limparEntrada(FILE *entrada){
    while (getc(entrada) != '\n');
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
    grafo->pesos[grafo->numero_nos-1] = calloc(grafo->numero_nos, sizeof(float));
    if (grafo->pesos[grafo->numero_nos-1] == NULL) {
        printf("Não foi possível adicionar o nó!");
        return;
    }

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

        uint tamanho_maior_nome = maiorStringTamanho(g1->nos, g1->numero_nos);
        if (tamanho_maior_nome < 4) tamanho_maior_nome += 4;

        // Mostra o nome de cada coluna
        printf("%*s", tamanho_maior_nome + 3,  " | ");
        for (uint c = 0; c < g1->numero_nos; c++){
            printf("%-*s | ", tamanho_maior_nome,  g1->nos[c].nome_cidade);
        }

        // Imprime cada linha e o conteúdo
        for (uint c = 0; c < g1->numero_nos; c++){
            printf("\n%*s | ", tamanho_maior_nome, g1->nos[c].nome_cidade);
            for (uint i = 0; i < g1->numero_nos; i++) {
                printf("%-*.3f | ", tamanho_maior_nome,  g1->pesos[c][i]);
            }
        }

    }
}

uint maiorStringTamanho(Nodo *nos, int numero_nos) {
    uint maior = 0;

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

void mostrarMenorCaminho(Grafo *grafo) {
    if (!grafo) return;
    if (grafo->numero_nos == 0) {
        printf("O Grafo está vazio\n");
        return;
    }

    // Lê o vértice inicial para procura
    
    int indice_vertice_partida = -1;
    do {
        printf("\nVértice de início: ");
        Nodo vertice_partida = lerNodo();
        indice_vertice_partida = procuraNodo(grafo, vertice_partida);
        free(vertice_partida.nome_cidade);
    } while ( indice_vertice_partida == -1);

    // Lê o vértice de destino do menor caminho
    int indice_vertice_chegada = -1;
    do {
        printf("\nVérice de destino: ");
        Nodo vertice_chegada = lerNodo();
        indice_vertice_chegada = procuraNodo(grafo, vertice_chegada);
        free(vertice_chegada.nome_cidade);
    } while (indice_vertice_chegada == -1);


    AuxDijkstra *resultado = Dijkstra(grafo, indice_vertice_partida);
    if (!resultado) {
        printf("\nERRO AO PROCURAR MENOR CAMINHO\n");
        return;
    }

    if (resultado[indice_vertice_chegada].menor_caminho == INFINITY) {
        printf("\nNão existe um caminho entre os vértices selecionados\n");
    }
    else {
        printf("\nO menor caminho é : %.2f\n", resultado[indice_vertice_chegada].menor_caminho);

        int contador_vertice = indice_vertice_chegada;
        while (contador_vertice != -1) {    // contador_vertice != indice_vertice_partida
            printf("%s\n", grafo->nos[contador_vertice].nome_cidade);
            contador_vertice = resultado[contador_vertice].indice_vertice_anterior;
        }
    }
    free(resultado);
    
}

AuxDijkstra *inicializaVetoresDijkstra(Grafo *grafo) {
    if (!grafo) return NULL;
    AuxDijkstra *temp = (AuxDijkstra *) malloc(sizeof(AuxDijkstra) * grafo->numero_nos);
    for (uint c = 0; c < grafo->numero_nos; c++) {
        temp[c].indice_vertice_anterior = -1;
        temp[c].menor_caminho = (float)INFINITY;   // Inicializa com infinito
        temp[c].vertice_aberto = true;
    }
    return temp;
}

AuxDijkstra *Dijkstra(Grafo *grafo, int indice_vertice_inicial){
    if (!grafo || indice_vertice_inicial < 0) return NULL;
    
    // Inicialização 
    AuxDijkstra *aux_dijkstra = inicializaVetoresDijkstra(grafo);

    // Inicializa o vértice de partida com 0.0
    aux_dijkstra[indice_vertice_inicial].menor_caminho = 0.0;

    while (possuiVerticesAbertos(aux_dijkstra, grafo->numero_nos)) {
        // Escolhe dentre os vértices abertos o que tem a menor estimativa
        int indice_vertice_atual = indiceVerticeAbertoComMenorEstimativa(aux_dijkstra, grafo->numero_nos);
        VerticesAdjacentes v_adj = verticesAdjacentes(grafo, indice_vertice_atual);

        // Marca o vértice como fechado
        aux_dijkstra[indice_vertice_atual].vertice_aberto = false;

        // Atualiza os caminhos dos vértices adjacentes
        for (int c=0; c < v_adj.tamanho; c++) {
            int indice_vertice_adjacente = v_adj.verticesAdjacentes[c];
            // Só visita vértices marcados como aberto
            if (aux_dijkstra[indice_vertice_adjacente].vertice_aberto) {
                if (aux_dijkstra[indice_vertice_atual].menor_caminho + grafo->pesos[indice_vertice_atual][indice_vertice_adjacente] < aux_dijkstra[indice_vertice_adjacente].menor_caminho){
                    aux_dijkstra[indice_vertice_adjacente].menor_caminho = aux_dijkstra[indice_vertice_atual].menor_caminho + grafo->pesos[indice_vertice_atual][indice_vertice_adjacente];
                    aux_dijkstra[indice_vertice_adjacente].indice_vertice_anterior = indice_vertice_atual;
                }
            }
        }

        free(v_adj.verticesAdjacentes);
    }

    return aux_dijkstra;
}

int indiceVerticeAbertoComMenorEstimativa(AuxDijkstra *aux_dijkstra, int numero_nos) {
    int indice_vertice_menor_estimativa = -1;
    float menor_estimativa = INFINITY;
    for (int c = 0; c < numero_nos; c++){
        if (aux_dijkstra[c].vertice_aberto == true){
            if (indice_vertice_menor_estimativa == -1 || aux_dijkstra[c].menor_caminho < menor_estimativa){
                indice_vertice_menor_estimativa = c;
                menor_estimativa = aux_dijkstra[c].menor_caminho;
            }
        }
    }
    return indice_vertice_menor_estimativa;
}

bool possuiVerticesAbertos(AuxDijkstra *aux_dijkstra, int total_nos) {
    for (int c = 0; c < total_nos; c++){
        if (aux_dijkstra[c].vertice_aberto) return true;
    }

    return false;
}

VerticesAdjacentes verticesAdjacentes(Grafo *grafo, int posicao_no){
    VerticesAdjacentes temp_v;
    temp_v.tamanho = 0;
    
    // Aloca espaço para o pior caso, ser adjacente a todos os vértices
    temp_v.verticesAdjacentes = (int *) malloc(sizeof(int) * grafo->numero_nos);
    if (!temp_v.verticesAdjacentes) {
        printf("ERRO\n");
        return temp_v;
    }

    // Salva o índice dos vértices que são adjacentes (peso da conexão diferente de zero)
    for (uint c = 0; c < grafo->numero_nos; c++){
        if (grafo->pesos[posicao_no][c] != 0.0){
            temp_v.verticesAdjacentes[temp_v.tamanho++] = c;
        }
    }

    // Diminui o espaço que não está sendo utilizado
    temp_v.verticesAdjacentes = realloc(temp_v.verticesAdjacentes, sizeof(int) * temp_v.tamanho);
    if (!temp_v.verticesAdjacentes) {
        temp_v.tamanho = 0;
    }

    return temp_v;    
}

// Retorna o índice do nó, ou -1 caso não encontrar
int procuraNodo(Grafo *grafo, Nodo no){
    if (grafo) {
        for (uint c = 0; c < grafo->numero_nos; c++){
            if (strcmp(no.nome_cidade, grafo->nos[c].nome_cidade) == 0) return c;
        }
    }
    return -1;
}