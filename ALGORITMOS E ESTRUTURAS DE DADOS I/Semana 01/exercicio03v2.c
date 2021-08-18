#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAM_NOME 10
#define ERRO_ALOCAR_MEMORIA -1
#define True 1

typedef unsigned int uint;

// Variáveis globais
int TAM_PESSOA = (sizeof(char) * TAM_NOME + sizeof(uint) + sizeof(long int));
int NOME_PESSOA = (0);
int IDADE_PESSOA = (sizeof(char) * TAM_NOME);
int TELEFONE_PESSOA = (sizeof(char) * TAM_NOME + sizeof(uint));

// Protoripação das funções
void *incluir(void *pBuffer);
void lerPessoa(void *pBuffer);
long int lerNumero();
void *apagar(void *pBuffer);
void listar(const void *pBuffer);
void **buscar(void *lista, uint *numero_encontrados);
void exibeBusca(void *lista);
void exibirInformacoes(const void *celula);
int menu(void);

int comparaStrings(const char *str1, const char *str2);
int tamanhoString(const char *string);

int main(){
    void *pBuffer = (void *) calloc(1, sizeof(uint));
    if (!pBuffer){
        printf("Nao foi possivel alocar memoria.");
        exit(ERRO_ALOCAR_MEMORIA);
    }
    *((uint *) pBuffer) = 0;    // Redundante
    int escolha = 0;
    while (True){
        escolha = menu();
        switch (escolha)
        {
        // Adicionar Contato
        case 1:
            pBuffer = incluir(pBuffer);
            break;
        // Apagar Contato
        case 2:
            pBuffer = apagar(pBuffer);
            break;
        // Listar Contatos
        case 3:
            listar(pBuffer);
            break;
        // Buscar Contato
        case 4:
            exibeBusca(pBuffer);
            break;
        // Sair do programa
        case 5:
            if (pBuffer) free(pBuffer);
            exit(0);
            break;
        // Opção inválida
        default:
            printf("Opcao invalida!!");
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
        opcao = (int) lerNumero();
    } while (opcao > 5 || opcao < 1);
    return opcao;
}

// Lê um número como uma string e depois converte para um long int
long int lerNumero() {
    size_t caracteres_lidos;
    char *entrada = NULL;
    long int saida = 0;
    while (True) {
        caracteres_lidos = 0;
        getline(&entrada, &caracteres_lidos, stdin);
        if (caracteres_lidos && entrada){
            saida = atol(entrada);
            free(entrada);
            entrada = NULL;
            break;
        }
        if (entrada) {
            free(entrada);
            entrada = NULL;
        }
    }
    return saida;
}

void *incluir(void *pBuffer){
    if (!pBuffer){
        return pBuffer;
    }
    // Incrementa o número de contatos
    *((uint *) pBuffer) += 1;
    pBuffer = (void *) realloc(pBuffer,  (*((uint *) pBuffer)) * TAM_PESSOA + sizeof(uint));
    if (!pBuffer){
        printf("Nao foi possivel alocar mais memoria.\n");
        exit(ERRO_ALOCAR_MEMORIA);
    }
    lerPessoa(pBuffer + ((*((uint *) pBuffer) - 1) * TAM_PESSOA) + sizeof(uint));
    return pBuffer;
}

void lerPessoa(void *pBuffer){
    if (pBuffer){
        printf("\nDigite o Nome: ");
        fgets((char *) pBuffer, TAM_NOME, stdin);
        do {
            printf("Digite a idade: ");
            *((uint *)(pBuffer + IDADE_PESSOA)) = (uint) lerNumero();
        } while (*((uint *)(pBuffer + IDADE_PESSOA)) == 0);
        do {
            printf("Digite o Numero: ");
            *((long int *)(pBuffer + TELEFONE_PESSOA)) = (long int) lerNumero();
        } while (*((long int *)(pBuffer + TELEFONE_PESSOA)) == 0);
    }
}

void listar(const void *pBuffer){
    if (pBuffer){
        printf("Existem %u registros.\n", *((uint *) pBuffer));
        for (int c = 0; c < *((uint *) pBuffer); c++){
            exibirInformacoes(pBuffer + ( TAM_PESSOA * c ) + sizeof(uint));
        }
    }
}

void exibirInformacoes(const void *celula){
    if (celula){
        printf("\nNome: \t\t%s", (char *) (celula + NOME_PESSOA));
        printf("Idade: \t\t%u\n", *((uint *) (celula + IDADE_PESSOA)));
        printf("Telefone: \t%ld\n", *((long int *) (celula + TELEFONE_PESSOA)));
    }
}

void *apagar(void *pBuffer){
    uint encontrados = 0;
    printf("\nEscolha uma forma para apagar um registro.\n");
    void **locais = buscar(pBuffer, &encontrados);

    if (encontrados) {
        for (uint c = 0; c < encontrados; c++){
           for ( void *i = locais[c]; i < (pBuffer + sizeof(uint) + (*((uint *) pBuffer) - 1) * TAM_PESSOA); i += TAM_PESSOA){
                memcpy(i, i + TAM_PESSOA, TAM_PESSOA);
                //strcpy((char *) (i), (char *) (i + TAM_PESSOA));
                //*((uint *) (i + sizeof(char) * TAM_NOME)) = *((uint *) (i + TAM_PESSOA + sizeof(char) * TAM_NOME));
                //*((long int *) (i + sizeof(uint) + sizeof(char) * TAM_NOME)) = *((long int *) (i + TAM_PESSOA + sizeof(uint) + sizeof(char) * TAM_NOME));
            }
        }
        free(locais);
        locais = NULL;
        pBuffer = (void *) realloc(pBuffer, (*((uint *) pBuffer) - encontrados) * TAM_PESSOA + sizeof(uint));
        if (!pBuffer){
            printf("Nao foi possivel realocar a memoria.");
            exit(ERRO_ALOCAR_MEMORIA);
        }
        *((uint *) pBuffer) -= encontrados;
        printf("\n%d registros excuidos.\n", encontrados);

    }
    else {
        printf("Nao encontrado\n");
    }
    return pBuffer;
}

void **buscar(void *lista, uint *numero_encontrados){
    *numero_encontrados = 0;
    if (!lista){
        return NULL;
    }
    if (*((uint *) lista) == 0){
        printf("Nao existema elementos na lista\n");
        return NULL;
    }

    int opcao = 0;
    
    do {
        printf("\n1 - Buscar por nome");
        printf("\n2 - Buscar por idade");
        printf("\n3 - Buscar por telefone");
        printf("\n4 - Buscar por todos campos");
        printf("\n:> ");
        opcao = lerNumero();
    } while(opcao < 1 || opcao > 4);
    
    char nome[TAM_NOME];
    uint idade = 0;
    long int telefone = 0;

    switch (opcao)
    {
    case 1:
        printf("\nDigite o Nome: ");
        fgets(nome, TAM_NOME, stdin);
        break;
    case 2:
        printf("\nDigite a Idade: ");
        idade = (uint) lerNumero();
        break;
    case 3:
        printf("\nDigite o Telefone: ");
        telefone = (long int) lerNumero();
        break;
    case 4:
        printf("\nDigite o Nome: ");
        fgets(nome, TAM_NOME, stdin);
        printf("Digite a Idade: ");
        idade = (uint) lerNumero();
        printf("Digite o Telefone: ");
        telefone = (long int) lerNumero();
        break;
    }

    uint adicionar = 0;
    void *endereco = NULL;
    void **lista_enderecos_encontrados = (void **) calloc(1, sizeof(void *));
    if (!lista_enderecos_encontrados){
        printf("Nao foi possivel alocar memoria.");
        exit(ERRO_ALOCAR_MEMORIA);
    }

    for (int c = 0; c < *((uint *) lista); c++){
        endereco = (lista + c * TAM_PESSOA + sizeof(uint));
        adicionar = 0;
        switch (opcao)
        {
        case 1:
            //if ( strcmp(nome, (char *) endereco) == 0 ){
            if ( comparaStrings(nome, (char *) (endereco + NOME_PESSOA)) ){
                adicionar++;
            }
            break;
        case 2:
            if ( idade == *((uint *) (endereco + IDADE_PESSOA)) ){
                adicionar++;
            }
            break;
        case 3:
            if ( telefone == *((long int *) (endereco + TELEFONE_PESSOA)) ){
                adicionar++;
            }
            break;
        case 4:
            //if ( strcmp(nome, (char *) endereco) == 0 ){
            if ( comparaStrings(nome, (char *) (endereco + NOME_PESSOA)) ){
                if ( idade == *((uint *) (endereco + IDADE_PESSOA)) ){
                    if ( telefone == *((long int *) (endereco + TELEFONE_PESSOA)) ){
                        adicionar++;
                    }
                }
            }
            break;
        }

        if (adicionar){
            lista_enderecos_encontrados = (void **) realloc(lista_enderecos_encontrados, (*numero_encontrados + 1) * sizeof(void *));
            if (!lista_enderecos_encontrados){
                printf("Nao foi possivel realocar memoria.");
                exit(ERRO_ALOCAR_MEMORIA);
            }
            lista_enderecos_encontrados[*numero_encontrados] = endereco;
            (*numero_encontrados)++;
        }
    }
    
    if ( *numero_encontrados == 0 ){
        free(lista_enderecos_encontrados);
        lista_enderecos_encontrados = NULL;
    }
    return lista_enderecos_encontrados;
}


void exibeBusca(void *lista){
    uint encontrados = 0;
    void **locais = buscar(lista, &encontrados);
    printf("%u registros encontrados.", encontrados);
    if (encontrados){
        for ( int c = 0; c < encontrados; c++){
            exibirInformacoes(locais[c]);
        }
        free(locais);
        locais = (void **) NULL;
    }
}

int comparaStrings(const char *str1, const char *str2){
    if (!str1 || !str2){
        printf("Strings invalidas.\n");
        return -1;
    }
    int tamanho_str1 = tamanhoString(str1);
    int tamanho_str2 = tamanhoString(str2);

    if (tamanho_str1 == tamanho_str2){
        for (int c = 0; c < tamanho_str1; c++){
            if ( str1[c] != str2[c] ){
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int tamanhoString(const char *string){
    int contador = 0;
    while (string[contador++] != '\0');
    return --contador;
}
