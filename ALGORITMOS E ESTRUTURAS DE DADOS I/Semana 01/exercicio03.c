#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 10
#define ERRO_ALOCAR_MEMORIA -1
#define True 1
#define False 0

typedef enum {ENUM_INICIO = 0, ENUM_FINAL = 1} extremidade;

typedef struct {
    short unsigned int DDD;
    long unsigned int numero;
} numeroTelefonico;

typedef struct {
    char nome[TAM_NOME];
    unsigned int idade;
    numeroTelefonico telefone;
} pessoa;

typedef struct lista_telefonica {
    struct lista_telefonica *anterior;
    pessoa contato;
    struct lista_telefonica *proximo;
} agenda;

void incluir(agenda *lista);
pessoa lerPessoa();
long int lerNumero();
agenda *criarCelula();
agenda *apagar(agenda *lista);
void listar(const agenda *lista);
agenda *buscar(agenda *lista);
int compararPessoas(pessoa p1, pessoa p2);
agenda *itemExtremolista(const agenda *lista, extremidade sentido);
void liberarMemoria(agenda *lista);
void liberaCelula(agenda *celula);
void exibirInformacoes(const agenda *celula);
int menu(void);

int main(){
    agenda *pBuffer = NULL;
    int escolha = 0;
    while (True){
        escolha = menu();
        switch (escolha)
        {
        case 1:
            if (!pBuffer){
                pBuffer = criarCelula();
            }
            else {
                incluir(pBuffer);
            }
            break;
        case 2:
            if (pBuffer){
                pBuffer = apagar(pBuffer);
            }
            else {
                printf("Nao existem contatos na lista.\n");
            }
            break;
        case 3:
            if (pBuffer){
                listar(pBuffer);
            }
            else {
                printf("Nao existem contatos na lista.\n");
            }
            break;
        case 4:
            if (pBuffer){
                exibirInformacoes(buscar(pBuffer));
            }
            else {
                printf("Nao existem contatos na lista.\n");
            }
            break;
        case 5:
            printf("Saindo\n");
            liberarMemoria(pBuffer);
            exit(0);
        default:
            printf("Opcao invalida.\n");
            break;
        }
    }
    return 0;
}

int menu(void){
    int opcao = 0;
    do {
        printf("\n\t---------");
        printf("\n1 - Incluir contato");
        printf("\n2 - Apagar contato");
        printf("\n3 - Listar contatos");
        printf("\n4 - Buscar contato");
        printf("\n5 - Sair");
        printf("\n\t---------\n");
        opcao = lerNumero();
    } while (opcao > 5 || opcao < 1);
    return opcao;
}

// Aloca espaço para a primeira célula
agenda *criarCelula(){
    agenda *celula = calloc(1, sizeof(agenda));
    if (!celula){
        printf("Nao foi possivel alocar memoria.");
        exit(ERRO_ALOCAR_MEMORIA);
    }

    celula->anterior = NULL;
    celula->proximo = NULL;
    celula->contato = lerPessoa();
    return celula;
}

// Lê todos os dados da estrutura pessa e retorna
pessoa lerPessoa(){
    pessoa p1;
    printf("Digite o nome: ");
    fgets(p1.nome, TAM_NOME, stdin);
    
    do {
        printf("Digite a idade: ");
        p1.idade = (unsigned int) lerNumero();
    } while(p1.idade == 0);
    
    do {
        printf("Digite o DDD: ");
        p1.telefone.DDD = (unsigned short) lerNumero();
    } while (p1.telefone.DDD == 0);

    do {
        printf("Digite o numero: ");
        p1.telefone.numero = (unsigned long) lerNumero();
    } while (p1.telefone.numero == 0);

    return p1;
}

// Lê um número como uma string e depois converte para um long int
long int lerNumero() {
    size_t caracteres_lidos;
    char *entrada = NULL;
    long int saida = 0;
    while (True) {
        caracteres_lidos = 0;
        getline(&entrada, &caracteres_lidos, stdin);
        if (caracteres_lidos){
            saida = atol(entrada);
            if (entrada) {
                free(entrada);
                entrada = NULL;
            }
            break;
        }
        if (entrada) {
            free(entrada);
            entrada = NULL;
        }
    }
    return saida;
}

void incluir(agenda *lista){
    agenda *ultimo_item = itemExtremolista(lista, ENUM_FINAL);
    ultimo_item->proximo = criarCelula();
    ultimo_item->proximo->anterior = ultimo_item;
}

agenda *itemExtremolista(const agenda *lista, extremidade sentido){
    if (lista) {
        switch (sentido)
        {
        case ENUM_FINAL:
            while (lista->proximo != NULL){
                lista = lista->proximo;
            }
            break;
        case ENUM_INICIO:
            while (lista->anterior != NULL){
                lista = lista->anterior;
            }
            break;
        }
    }
    return lista;
}

agenda *apagar(agenda *lista) {
    agenda *encontrado = buscar(lista);
    if (encontrado){
        if (encontrado->anterior == NULL){
            lista = lista->proximo;
        }
        liberaCelula(encontrado);
        printf("Excluido\n");
    }
    return lista;
}

void listar(const agenda *lista) {
    agenda *item = itemExtremolista(lista, ENUM_INICIO);
    while (item){
        exibirInformacoes(item);
        item = item->proximo;
    }
}

void exibirInformacoes(const agenda *celula){
    if (celula){
        printf("\nNome: \t\t%s", celula->contato.nome);
        printf("Idade: \t\t%d\n", celula->contato.idade);
        printf("Numero: \t%hu %lu\n", celula->contato.telefone.DDD, celula->contato.telefone.numero);
    }
}

agenda *buscar(agenda *lista){
    printf("Insira as informacoes da Pessoa: \n");
    pessoa item = lerPessoa();
    agenda *inicio = itemExtremolista(lista, ENUM_INICIO);

    while (inicio){
        if (compararPessoas(item, inicio->contato)){
            return inicio;
        }
        inicio = inicio->proximo;
    }
    return NULL;
}

int compararPessoas(pessoa p1, pessoa p2){
    return (p1.idade == p2.idade && !strcmp(p1.nome, p2.nome) && p1.telefone.DDD == p2.telefone.DDD && p1.telefone.numero == p2.telefone.numero);
}

void liberarMemoria(agenda *lista){
    agenda *atual = itemExtremolista(lista, ENUM_INICIO);
    agenda *proximo = NULL;

    while (atual) {
        proximo = atual->proximo;
        liberaCelula(atual);
        atual = proximo;
    }
}

void liberaCelula(agenda *celula){
    if (celula) {
        if (celula->anterior){
            celula->anterior->proximo = celula->proximo;
            celula->anterior = NULL;
        }
        if (celula->proximo){
            celula->proximo->anterior = celula->anterior;
            celula->proximo = NULL;
        }
        free(celula);
        celula = NULL;
    }
}

