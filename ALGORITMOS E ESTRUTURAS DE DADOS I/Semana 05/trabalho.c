/*
Na memória um nodo é armazenado da seguinte forma:
struct Nodo {
    void *anterior;
    char *nome;
    short int idade;
    long int telefone;
    void *proximo;
};

A fila de prioridade é armazenada:
struct Fila {
    void *primeiro;
    void *ultimo;
};

O pBuffer é armazenado da seguinte maneira:
void *pBuffer = [numero_bytes_alocados, int opcao; struct Fila, ... ];
*/

/* Inlcudes */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
/* Includes */

/* Defines */
#define OFFSET_NUMERO_BYTES (0)
#define OFFSET_OPCAO_USUARIO (sizeof(int))
#define OFFSET_FILA (sizeof(int) + sizeof(int))
// Lista encadeada
#define TAM_NODO (sizeof(void *) + sizeof(char *) + sizeof(short int) + sizeof(long int) + sizeof(void *))
#define TAM_PESSOA (sizeof(char *) + sizeof(short int) + sizeof(long int))
#define OFFSET_ANTERIOR_NODO (0)
#define OFFSET_NOME_PESSOA (sizeof(void *))
#define OFFSET_IDADE_PESSOA (sizeof(void *) + sizeof(char *))
#define OFFSET_TELEFONE_PESSOA (sizeof(void *) + sizeof(char *) + sizeof(short int))
#define OFFSET_PROXIMO_NODO (sizeof(void *) + sizeof(char *) + sizeof(short int) + sizeof(long int))
// Fila de prioridade
#define TAM_FILA (sizeof(void *) + sizeof(void *))
#define OFFSET_PRIMEIRO (0)
#define OFFSET_ULTIMO (sizeof(void *))
/* Defines */

/* Funções */
// Funções da fila ordenada
bool PUSH(void *fila, void *nodo);
bool POP(void *fila, void *pessoa);
bool EMPTY(void *fila);
bool FULL(void *fila);
void CLEAR(void **pBuffer);
int prioridadeNodo(void *nodo1, void *nodo2);
// Funções da lista duplamente encadeada
void *alocarNodo(void);
void liberarNodo(void *nodo);
void imprimeNodo(void *nodo);
void liberaLista(void *primeiroNodo, void *proximoNodo);
// Funções da agenda
void menu(int *opcao);
void imprimir(void **pBuffer);
void inserir(void **pBuffer);
void buscar(void **pBuffer);
void apagar(void **pBuffer);
void *criarBuffer();
void lerInformacoesPessoa(void *nodo);
// Outras
void limparStream(FILE *arquivo);
/* Funções */

// Função principal
int main(){
    void *pBuffer = criarBuffer();
    assert(pBuffer);
    while (true){
        menu((int *) (pBuffer + OFFSET_OPCAO_USUARIO));
        //printf("A opção escolhida e: %d\n", *((int *) (pBuffer + OFFSET_OPCAO_USUARIO)));
        switch (*((int *) (pBuffer + OFFSET_OPCAO_USUARIO)))
        {
        case 1:
            inserir(&pBuffer);
            break;
        case 2:
            apagar(&pBuffer);
            break;
        case 3:
            buscar(&pBuffer);
            break;
        case 4:
            imprimir(&pBuffer);
            break;
        case 5:
            CLEAR(&pBuffer);
            free(pBuffer);
            printf("\nSAINDO...\n");
            exit(0);
            break;        
        default:
            break;
        }
    }
    return -1;
}

// Exibe as opções do usuário e salva a escolha no endereço passado
void menu(int *opcao){
    assert(opcao);
    do {
        printf("\n-------------MENU-------------");
        printf("\n\t1 - ADICIONAR PESSOA");
        printf("\n\t2 - APAGAR PESSOA");
        printf("\n\t3 - BUSCAR PESSOA");
        printf("\n\t4 - MOSTRAR PESSOAS");
        printf("\n\t5 - SAIR");
        printf("\n------------------------------\n");
        while (scanf("%d%*c", opcao) == 0) {
            limparStream(stdin);   // Limpa o buffer de entrada
        }
    } while (*opcao > 5 || *opcao < 1);
}

void limparStream(FILE *arquivo){
    // O stdin é a entrada padrão, que na verdade é um ponteiro para o tipo FILE
    // O tipo file é uma estrutura que contém, entre outras coisas, um ponteiro que diz 
    // qual o caractere próximo a ser lido (stdin->_IO_read_ptr) e qual é o 
    // endereço do último caractere da stream (stdin->_IO_read_end),
    // a função fgetc(stdin), vai ler um caractere por vez e a cada leitura o valor do 
    // stdin->_IO_read_ptr vai ser atualizado, quando o proximo caractere é o ultimo
    // então significa que a stream de entrada está limpa
    
    if (!arquivo) return;
    // Big O -> O(1)
    arquivo->_IO_read_ptr = arquivo->_IO_read_end;
    
    // Big 0 -> O(n)
    /*
    while (stdin->_IO_read_end != stdin->_IO_read_ptr) {
    //  fgetc(stdin);
        getc(stdin);
    }
    */
}

// Adiciona um nodo na fila
bool PUSH(void *fila, void *nodo){
    assert(fila && nodo);
    // Assegura que os ponteiros do nodo sejam NULL
    *(void **)(nodo + OFFSET_ANTERIOR_NODO) = NULL;
    *(void **)(nodo + OFFSET_PROXIMO_NODO) = NULL;
    if (!FULL(fila)){
        // Adicionar em fila Vazia
        if (EMPTY(fila)){
            *(void **) (fila + OFFSET_PRIMEIRO) = nodo;
            *(void **) (fila + OFFSET_ULTIMO) = nodo;
            return true;
        }

        // Seta o ultimo nodo até chegar na posição a ser inserido o nodo
        while(prioridadeNodo(nodo, *(void **) (fila + OFFSET_ULTIMO)) < 0 && * (void **) (*(void **) (fila + OFFSET_ULTIMO) + OFFSET_ANTERIOR_NODO) != NULL){
            *(void **) (fila + OFFSET_ULTIMO) = * (void **) (*(void **) (fila + OFFSET_ULTIMO) + OFFSET_ANTERIOR_NODO);
        }

        // Adicionar no início da fila
        if (*(void **) (fila + OFFSET_ULTIMO) == *(void **) (fila + OFFSET_PRIMEIRO) && prioridadeNodo(nodo, *(void **) (fila + OFFSET_ULTIMO)) < 0){
            // Atualiza o ponteiro do nodo que está sendo inserido
            *(void **) (nodo + OFFSET_PROXIMO_NODO) = *(void **) (fila + OFFSET_ULTIMO);
            // Atualiza o ponteiro do nodo vizinho do nodo
            *(void **) ((*(void **) (nodo + OFFSET_PROXIMO_NODO)) + OFFSET_ANTERIOR_NODO) = nodo;
            // Atualiza o primeiro elemento da lista
            *(void **) (fila + OFFSET_PRIMEIRO) = nodo;
        }
        // Adicionar no final da fila
        else if ( *(void **) (*(void **) (fila + OFFSET_ULTIMO) + OFFSET_PROXIMO_NODO) == NULL && prioridadeNodo(nodo, *(void **) (fila + OFFSET_ULTIMO)) > 0){
            // Adicionar depois do último nodo
            // Atualiza o ponteiro do nodo que está sendo inserido
            *(void **) (nodo + OFFSET_ANTERIOR_NODO) = *(void **) (fila + OFFSET_ULTIMO);
            // Atualiza o ponteiro do nodo vizinho do nodo
            *(void **) ((*(void **) (nodo + OFFSET_ANTERIOR_NODO)) + OFFSET_PROXIMO_NODO) = nodo;
            // Atualiza o primeiro elemento da lista
            *(void **) (fila + OFFSET_ULTIMO) = nodo;
        }
        // Adiciona no meio da fila antes do nodo
        else if (prioridadeNodo(nodo, *(void **) (fila + OFFSET_ULTIMO)) < 0){
            // Atualiza os ponteiros do nodo que está sendo inserido
            *(void **) (nodo + OFFSET_PROXIMO_NODO) = *(void **) (fila + OFFSET_ULTIMO);
            *(void **) (nodo + OFFSET_ANTERIOR_NODO) = *(void **) (*(void **) (fila + OFFSET_ULTIMO) + OFFSET_ANTERIOR_NODO);
            // Atualiza os ponteiros dos nodos vizinhos do nodo
            * (void **) ((*(void **) (nodo + OFFSET_PROXIMO_NODO)) + OFFSET_ANTERIOR_NODO) = nodo;
            * (void **) ((*(void **) (nodo + OFFSET_ANTERIOR_NODO)) + OFFSET_PROXIMO_NODO) = nodo;
        }
        // Adicionar no meio da fila depois do nodo
        else {
            // Atualiza os ponteiros do nodo que está sendo inserido
            *(void **) (nodo + OFFSET_ANTERIOR_NODO) = *(void **) (fila + OFFSET_ULTIMO);
            *(void **) (nodo + OFFSET_PROXIMO_NODO) = *(void **) (*(void **) (fila + OFFSET_ULTIMO) + OFFSET_PROXIMO_NODO);
            // Atualiza os ponteiros dos nodos vizinhos do nodo
            * (void **) ((*(void **) (nodo + OFFSET_PROXIMO_NODO)) + OFFSET_ANTERIOR_NODO) = nodo;
            * (void **) ((*(void **) (nodo + OFFSET_ANTERIOR_NODO)) + OFFSET_PROXIMO_NODO) = nodo;
        }
        // Atualiza o último elemento novamente
        for (*(void **) (fila + OFFSET_ULTIMO) = *(void **) (fila + OFFSET_PRIMEIRO);\
         * (void **) (*(void **) (fila + OFFSET_ULTIMO) + OFFSET_PROXIMO_NODO);\
         * (void **) (fila + OFFSET_ULTIMO) = *(void **) (*(void **) (fila + OFFSET_ULTIMO) + OFFSET_PROXIMO_NODO) );
        return true;
    }
    return false;
}

bool POP(void *fila, void *nodo){
    assert(fila && nodo);
    if (!EMPTY(fila)){
        // Salva as informações no nodo passado
        * (char **) (nodo + OFFSET_NOME_PESSOA) = * (char **) (*(void **) (fila + OFFSET_PRIMEIRO) + OFFSET_NOME_PESSOA);
        * (short int *) (nodo + OFFSET_IDADE_PESSOA) = * (short int *) (*(void **) (fila + OFFSET_PRIMEIRO) + OFFSET_IDADE_PESSOA);
        * (long int *) (nodo + OFFSET_TELEFONE_PESSOA) = * (long int *) (*(void **) (fila + OFFSET_PRIMEIRO) + OFFSET_TELEFONE_PESSOA);
        
        // Atualiza os ponteiros da fila
        // Retirando o último?
        if (*(void **) (fila + OFFSET_PRIMEIRO) == *(void **) (fila + OFFSET_ULTIMO)){
            free(*(void **) (fila + OFFSET_PRIMEIRO));
            *(void **) (fila + OFFSET_PRIMEIRO) = NULL;
            *(void **) (fila + OFFSET_ULTIMO) = NULL;
        }
        else {
            *(void **) (fila + OFFSET_PRIMEIRO) = *(void **) (*(void **) (fila + OFFSET_PRIMEIRO) + OFFSET_PROXIMO_NODO);
            free(*(void **) (*(void **) (fila + OFFSET_PRIMEIRO) + OFFSET_ANTERIOR_NODO));
            * (void **) (*(void **) (fila + OFFSET_PRIMEIRO) + OFFSET_ANTERIOR_NODO) = NULL;
        }
        return true;
    }
    return false;
}
bool EMPTY(void *fila){
    // Verifica se o primeiro elemento da fila ou o último é nulo
    return (*(void **) (fila + OFFSET_PRIMEIRO) == NULL && *(void **) (fila + OFFSET_ULTIMO) == NULL);
}
bool FULL(void *fila){
    // Como se trata de uma lista ligada sempre é possível adicionar mais elementos
    return false;
}

void CLEAR(void **pBuffer){
    // Basicamente aloca espaço para um nodo e vai dando POP até a fila esvaziar
    assert(*pBuffer);
    *pBuffer = realloc(*pBuffer, *(int*) *pBuffer + sizeof(void *));
    assert(*pBuffer);
    *(void **)(*pBuffer + *(int *) *pBuffer) = alocarNodo();
    assert(*(void **)(*pBuffer + *(int *) *pBuffer));
    while (!EMPTY((*pBuffer + OFFSET_FILA))){
        POP((*pBuffer + OFFSET_FILA), *(void **)(*pBuffer + *(int *) *pBuffer));
        // libera o espaço alocado dinamicamente para o nome
        free(*(char **) (*(void **)(*pBuffer + *(int *) *pBuffer) + OFFSET_NOME_PESSOA));
    }
    free(*(void **)(*pBuffer + *(int *) *pBuffer));
    *pBuffer = realloc(*pBuffer, *(int*) *pBuffer - sizeof(void *));
    assert(*pBuffer);
}

// Funciona de forma invertida: se a prioridade do n1 é maior que n2, então retorna um número negativo
int prioridadeNodo(void *nodo1, void *nodo2){
    assert(nodo1 && nodo2);
    // Nome igual
    if (strcmp(*(char **) (nodo1 + OFFSET_NOME_PESSOA), *(char **) (nodo2 + OFFSET_NOME_PESSOA)) == 0){
        // Retorna o a diferença da idade
        return *(short int *)(nodo2 + OFFSET_IDADE_PESSOA) - *(short int *)(nodo1 + OFFSET_IDADE_PESSOA);
    }
    return strcmp(*(char **) (nodo1 + OFFSET_NOME_PESSOA), *(char **) (nodo2 + OFFSET_NOME_PESSOA));
}

// Funções da lista duplamente encadeada
void *alocarNodo(void){
    return memset(malloc(TAM_NODO), 0, TAM_NODO);
    //return calloc(1, TAM_NODO);
}
void liberarNodo(void *nodo){
    assert(nodo);
    // Libera o espaço do Nome
    free(*(char **) (nodo + OFFSET_NOME_PESSOA));
    free(nodo);
}
void imprimeNodo(void *nodo){
    printf("\nNOME:\t\t%s", *(char **) (nodo + OFFSET_NOME_PESSOA));
    printf("\nIDADE:\t\t%02hd", * (short int *) (nodo + OFFSET_IDADE_PESSOA));
    printf("\nTELEFONE: \t%011ld\n", * (long int *) (nodo + OFFSET_TELEFONE_PESSOA));
}
// Exclui todos os nodos de uma lista ligada de forma recursiva
void liberaLista(void *primeiroNodo, void *proximoNodo){
    if (primeiroNodo){
        liberarNodo(primeiroNodo);
        if (proximoNodo){
            liberaLista(proximoNodo, *(void **) (proximoNodo + OFFSET_PROXIMO_NODO));
        }
    }
}
// Funções da agenda
void imprimir(void **pBuffer){
    assert(*pBuffer);
    // Cria outra fila
    *pBuffer = realloc(*pBuffer, *(int *) *pBuffer + TAM_FILA + sizeof(void *));
    *(int *) *pBuffer += TAM_FILA + sizeof(void *);
    // Zera os espacos de memória que a realloc alocou
    memset((*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *)), 0, (TAM_FILA + sizeof(void *)));
    assert(*pBuffer);
    while (!EMPTY(*pBuffer + OFFSET_FILA)){
        * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)) = alocarNodo();
        assert(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)));
        POP(*pBuffer + OFFSET_FILA, * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)));
        imprimeNodo(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)));
        // Adiciona o nodo retirado da fila principal e coloca na outra fila
        PUSH((*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *)), * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)));
    }
    // Faz com que a fila principal aponte para a fila secundária onde foram salvos os dados ao extrair da fila principal
    *(void **) (*pBuffer + OFFSET_FILA + OFFSET_PRIMEIRO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) + OFFSET_PRIMEIRO);
    *(void **) (*pBuffer + OFFSET_FILA + OFFSET_ULTIMO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) + OFFSET_ULTIMO);
    // Diminui o buffer 
    *pBuffer = realloc(*pBuffer, *(int *) *pBuffer - TAM_FILA - sizeof(void *));
    assert(*pBuffer);
    *(int *) *pBuffer -= (TAM_FILA + sizeof(void *));
}

void inserir(void **pBuffer){
    assert(*pBuffer);
    *pBuffer = realloc(*pBuffer, *(int *) *pBuffer + sizeof(void *));
    assert(*pBuffer);
    * (void **) (*pBuffer + *(int *) *pBuffer) = alocarNodo();
    assert(* (void **) (*pBuffer + *(int *) *pBuffer));
    lerInformacoesPessoa(* (void **) (*pBuffer + *(int *) *pBuffer));
    PUSH((void *)(*pBuffer + OFFSET_FILA), * (void **) (*pBuffer + *(int *) *pBuffer));
    *pBuffer = realloc(*pBuffer, *(int *) *pBuffer);
    assert(*pBuffer);
}

void buscar(void **pBuffer){
    assert(*pBuffer);
    
    printf("\nDeseja BUSCAR por:");
    printf("\n1 - NOME");
    printf("\n2 - IDADE");
    printf("\n3 - TELEFONE");
    printf("\n4 - TODOS OS CAMPOS\n");
    do {
        while (scanf("%d", (int *) (*pBuffer + OFFSET_OPCAO_USUARIO)) == 0){
            limparStream(stdin);
        }
    } while (*(int *) (*pBuffer + OFFSET_OPCAO_USUARIO) > 4 || *(int *) (*pBuffer + OFFSET_OPCAO_USUARIO) < 1);
    
    

    switch (*(int *) (*pBuffer + OFFSET_OPCAO_USUARIO))
    {
    case 1:
        // Aloca espaço no pBuffer para mais uma fila, para um nodo e para um ponteiro para char
        *pBuffer = realloc(*pBuffer, *(int *) *pBuffer + TAM_FILA + sizeof(void *) + sizeof(char *));
        assert(*pBuffer);
        *(int *) *pBuffer += TAM_FILA + sizeof(void *) + sizeof(char *);
        memset((*pBuffer + *(int *) *pBuffer - (TAM_FILA + sizeof(void *) + sizeof(char *))), 0, (TAM_FILA + sizeof(void *) + sizeof(char *)));
        
        printf("\nDigite o NOME que quer buscar: ");
        while (scanf("%ms", (char **) (*pBuffer + *(int *) *pBuffer - sizeof(char*))) == 0){
            limparStream(stdin);
        }

        printf("\n%s procurando...", *(char **) (*pBuffer + *(int *) *pBuffer - sizeof(char*)));
        
        while (!EMPTY((*pBuffer + OFFSET_FILA))){
            * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(char *)) = alocarNodo();
            assert(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(char *)));
            POP(*pBuffer + OFFSET_FILA, *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(char *) - sizeof(void *)));
            if ( strcmp(*(char **)(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(char*) - sizeof(void *)) + OFFSET_NOME_PESSOA), *(char **) (*pBuffer + *(int *) *pBuffer - sizeof(char*))) == 0){
                printf("\n\tENCONTRADO!!!");
                imprimeNodo(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(char*) - sizeof(void *)));
            }
            PUSH((*pBuffer + *(int *) *pBuffer - sizeof(char *) - sizeof(void *) - TAM_FILA), *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(char*) - sizeof(void *)));
        }
        free(*(char **) (*pBuffer + *(int *) *pBuffer - sizeof(char*)));
        // Faz com que a fila principal aponte para a fila secundária onde foram salvos os dados ao extrair da fila principal
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_PRIMEIRO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(char *) + OFFSET_PRIMEIRO);
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_ULTIMO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(char *) + OFFSET_ULTIMO);
        *(int *) *pBuffer -= (TAM_FILA + sizeof(void *) + sizeof(char *));
        break;
    case 2:
        // Aloca espaço no pBuffer para mais uma fila, para um nodo e para um short int
        *pBuffer = realloc(*pBuffer, *(int *) *pBuffer + TAM_FILA + sizeof(void *) + sizeof(short int));
        assert(*pBuffer);
        *(int *) *pBuffer += TAM_FILA + sizeof(void *) + sizeof(short int);
        memset((*pBuffer + *(int *) *pBuffer - (TAM_FILA + sizeof(void *) + sizeof(short int))), 0, (TAM_FILA + sizeof(void *) + sizeof(short int)));
        
        printf("\nDigite a IDADE que quer buscar: ");
        do {
            while (scanf("%hd", (short int *) (*pBuffer + *(int *) *pBuffer - sizeof(short int))) == 0){
                limparStream(stdin);
            }
        } while ( *(short int *) (*pBuffer + *(int *) *pBuffer - sizeof(short int)) <= 0);
        
        while (!EMPTY(*pBuffer + OFFSET_FILA)){
            * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(short int)) = alocarNodo();
            assert(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(short int)));
            POP(*pBuffer + OFFSET_FILA, *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(short int) - sizeof(void *)));
            if ( *(short int *) (*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(short int) - sizeof(void *)) + OFFSET_IDADE_PESSOA) == *(short int *) (*pBuffer + *(int *) *pBuffer - sizeof(short int))){
                printf("\n\tENCONTRADO!!!");
                imprimeNodo(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(short int) - sizeof(void *)));
            }
            PUSH((*pBuffer + *(int *) *pBuffer - sizeof(short int) - sizeof(void *) - TAM_FILA), *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(short int) - sizeof(void *)));
        }
        // Faz com que a fila principal aponte para a fila secundária onde foram salvos os dados ao extrair da fila principal
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_PRIMEIRO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(short int) + OFFSET_PRIMEIRO);
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_ULTIMO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(short int) + OFFSET_ULTIMO);
        *(int *) *pBuffer -= (TAM_FILA + sizeof(void *) + sizeof(short int));
        break;
    case 3:
        // Aloca espaço no pBuffer para mais uma fila, para um nodo e para um long int
        *pBuffer = realloc(*pBuffer, *(int *) *pBuffer + TAM_FILA + sizeof(void *) + sizeof(long int));
        assert(*pBuffer);
        *(int *) *pBuffer += TAM_FILA + sizeof(void *) + sizeof(long int);
        memset((*pBuffer + *(int *) *pBuffer - (TAM_FILA + sizeof(void *) + sizeof(long int))), 0, (TAM_FILA + sizeof(void *) + sizeof(long int)));
        
        printf("\nDigite o TELEFONE que quer buscar: ");
        do {
            while (scanf("%ld", (long int *) (*pBuffer + *(int *) *pBuffer - sizeof(long int))) == 0){
                limparStream(stdin);
            }
        } while ( *(long int *) (*pBuffer + *(int *) *pBuffer - sizeof(long int)) <= 0);
        
        while (!EMPTY(*pBuffer + OFFSET_FILA)){
            * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(long int)) = alocarNodo();
            assert(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(long int)));
            POP(*pBuffer + OFFSET_FILA, *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(long int) - sizeof(void *)));
            if ( *(long int *) (*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(long int) - sizeof(void *)) + OFFSET_TELEFONE_PESSOA) == *(long int *) (*pBuffer + *(int *) *pBuffer - sizeof(long int))){
                printf("\n\tENCONTRADO!!!");
                imprimeNodo(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(long int) - sizeof(void *)));
            }
            PUSH((*pBuffer + *(int *) *pBuffer - sizeof(long int) - sizeof(void *) - TAM_FILA), *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(long int) - sizeof(void *)));
        }
        // Faz com que a fila principal aponte para a fila secundária onde foram salvos os dados ao extrair da fila principal
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_PRIMEIRO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(long int) + OFFSET_PRIMEIRO);
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_ULTIMO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(long int) + OFFSET_ULTIMO);
        *(int *) *pBuffer -= (TAM_FILA + sizeof(void *) + sizeof(long int));
        break;
    case 4:
        // Aloca espaço no pBuffer para mais uma fila, para dois nodo 
        *pBuffer = realloc(*pBuffer, *(int *) *pBuffer + TAM_FILA + sizeof(void *) + sizeof(void *));
         assert(*pBuffer);
        *(int *) *pBuffer += TAM_FILA + sizeof(void *) + sizeof(void *);
        memset((*pBuffer + *(int *) *pBuffer - (TAM_FILA + sizeof(void *) + sizeof(void *))), 0, (TAM_FILA + sizeof(void *) + sizeof(void *)));
        
        * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)) = alocarNodo();
        assert(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)));
        lerInformacoesPessoa(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)));
        
        while (!EMPTY(*pBuffer + OFFSET_FILA)){
            * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)) = alocarNodo();
            assert(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)));
            POP(*pBuffer + OFFSET_FILA, *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)));
            // Compara o telefone 
            if ( *(long int *) (*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)) + OFFSET_TELEFONE_PESSOA) == *(long int *) (*(void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)) + OFFSET_TELEFONE_PESSOA)){
                if ( *(short int *) (*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)) + OFFSET_IDADE_PESSOA) == *(short int *) (*(void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)) + OFFSET_IDADE_PESSOA)){
                    if ( strcmp( *(char **) (*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)) + OFFSET_NOME_PESSOA), *(char **) (*(void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)) + OFFSET_NOME_PESSOA)) == 0){
                        printf("\n\tENCONTRADO!!!");
                        imprimeNodo(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)));
                    }
                }
            }
            PUSH((*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *) - TAM_FILA), *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)));
        }
        liberarNodo(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)));
        // Faz com que a fila principal aponte para a fila secundária onde foram salvos os dados ao extrair da fila principal
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_PRIMEIRO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(void *) + OFFSET_PRIMEIRO);
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_ULTIMO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(void *) + OFFSET_ULTIMO);
        
        *(int *) *pBuffer -= (TAM_FILA + sizeof(void *) + sizeof(void *));
        break;
    }
    
    // Diminui o buffer 
    *pBuffer = realloc(*pBuffer, *(int *) *pBuffer);
    assert(*pBuffer);
}

void apagar(void **pBuffer){
    assert(*pBuffer);
    
    printf("\nDeseja APAGAR por:");
    printf("\n1 - NOME");
    printf("\n2 - IDADE");
    printf("\n3 - TELEFONE");
    printf("\n4 - TODOS OS CAMPOS\n");
    do {
        while (scanf("%d", (int *) (*pBuffer + OFFSET_OPCAO_USUARIO)) == 0){
            limparStream(stdin);
        }
    } while (*(int *) (*pBuffer + OFFSET_OPCAO_USUARIO) > 4 || *(int *) (*pBuffer + OFFSET_OPCAO_USUARIO) < 1);
    
    
    switch (*(int *) (*pBuffer + OFFSET_OPCAO_USUARIO))
    {
    case 1:
        // Aloca espaço no pBuffer para mais uma fila, para um nodo e para um ponteiro para char
        *pBuffer = realloc(*pBuffer, *(int *) *pBuffer + TAM_FILA + sizeof(void *) + sizeof(char *));
        assert(*pBuffer);
        *(int *) *pBuffer += TAM_FILA + sizeof(void *) + sizeof(char *);
        memset((*pBuffer + *(int *) *pBuffer - (TAM_FILA + sizeof(void *) + sizeof(char *))), 0, (TAM_FILA + sizeof(void *) + sizeof(char *)));
        
        printf("\nDigite o NOME que quer buscar: ");
        while (scanf("%ms", (char **) (*pBuffer + *(int *) *pBuffer - sizeof(char*))) == 0){
            limparStream(stdin);
        }

        printf("\n%s procurando...", *(char **) (*pBuffer + *(int *) *pBuffer - sizeof(char*)));
        
        while (!EMPTY((*pBuffer + OFFSET_FILA))){
            * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(char *)) = alocarNodo();
            assert(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(char *)));
            POP(*pBuffer + OFFSET_FILA, *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(char *) - sizeof(void *)));
            if (strcmp(*(char **)(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(char*) - sizeof(void *)) + OFFSET_NOME_PESSOA), *(char **) (*pBuffer + *(int *) *pBuffer - sizeof(char*))) == 0){
                printf("\n\tENCONTRADO!!!");
                imprimeNodo(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(char*) - sizeof(void *)));
                liberarNodo(*(void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(char *)));
                printf("\n\tEXCLUIDO");
                continue;
            }
            PUSH((*pBuffer + *(int *) *pBuffer - sizeof(char *) - sizeof(void *) - TAM_FILA), *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(char*) - sizeof(void *)));
        }
        free(*(char **) (*pBuffer + *(int *) *pBuffer - sizeof(char *)));
        // Faz com que a fila principal aponte para a fila secundária onde foram salvos os dados ao extrair da fila principal
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_PRIMEIRO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(char *) + OFFSET_PRIMEIRO);
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_ULTIMO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(char *) + OFFSET_ULTIMO);
        *(int *) *pBuffer -= (TAM_FILA + sizeof(void *) + sizeof(char *));
        break;
    case 2:
        // Aloca espaço no pBuffer para mais uma fila, para um nodo e para um short int
        *pBuffer = realloc(*pBuffer, *(int *) *pBuffer + TAM_FILA + sizeof(void *) + sizeof(short int));
        assert(*pBuffer);
        *(int *) *pBuffer += TAM_FILA + sizeof(void *) + sizeof(short int);
        memset((*pBuffer + *(int *) *pBuffer - (TAM_FILA + sizeof(void *) + sizeof(short int))), 0, (TAM_FILA + sizeof(void *) + sizeof(short int)));
        
        printf("\nDigite a IDADE que quer buscar: ");
        do {
            while (scanf("%hd", (short int *) (*pBuffer + *(int *) *pBuffer - sizeof(short int))) == 0){
                limparStream(stdin);
            }
        } while ( *(short int *) (*pBuffer + *(int *) *pBuffer - sizeof(short int)) <= 0);
        
        while (!EMPTY(*pBuffer + OFFSET_FILA)){
            * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(short int)) = alocarNodo();
            assert(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(short int)));
            POP(*pBuffer + OFFSET_FILA, *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(short int) - sizeof(void *)));
            if ( *(short int *) (*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(short int) - sizeof(void *)) + OFFSET_IDADE_PESSOA) == *(short int *) (*pBuffer + *(int *) *pBuffer - sizeof(short int))){
                printf("\n\tENCONTRADO!!!");
                imprimeNodo(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(short int) - sizeof(void *)));
                liberarNodo(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(short int) - sizeof(void *)));
                printf("\n\tEXCLUIDO");
                continue;
            }
            PUSH((*pBuffer + *(int *) *pBuffer - sizeof(short int) - sizeof(void *) - TAM_FILA), *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(short int) - sizeof(void *)));
        }
        // Faz com que a fila principal aponte para a fila secundária onde foram salvos os dados ao extrair da fila principal
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_PRIMEIRO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(short int) + OFFSET_PRIMEIRO);
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_ULTIMO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(short int) + OFFSET_ULTIMO);
        *(int *) *pBuffer -= (TAM_FILA + sizeof(void *) + sizeof(short int));
        break;
    case 3:
        // Aloca espaço no pBuffer para mais uma fila, para um nodo e para um long int
        *pBuffer = realloc(*pBuffer, *(int *) *pBuffer + TAM_FILA + sizeof(void *) + sizeof(long int));
        assert(*pBuffer);
        *(int *) *pBuffer += TAM_FILA + sizeof(void *) + sizeof(long int);
        memset((*pBuffer + *(int *) *pBuffer - (TAM_FILA + sizeof(void *) + sizeof(long int))), 0, (TAM_FILA + sizeof(void *) + sizeof(long int)));
        
        
        printf("\nDigite o TELEFONE que quer buscar: ");
        do {
            while (scanf("%ld", (long int *) (*pBuffer + *(int *) *pBuffer - sizeof(long int))) == 0){
                limparStream(stdin);
            }
        } while ( *(long int *) (*pBuffer + *(int *) *pBuffer - sizeof(long int)) <= 0);
        
        while (!EMPTY(*pBuffer + OFFSET_FILA)){
            * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(long int)) = alocarNodo();
            assert(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(long int)));
            POP(*pBuffer + OFFSET_FILA, *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(long int) - sizeof(void *)));
            if ( *(long int *) (*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(long int) - sizeof(void *)) + OFFSET_TELEFONE_PESSOA) == *(long int *) (*pBuffer + *(int *) *pBuffer - sizeof(long int))){
                printf("\n\tENCONTRADO!!!");
                imprimeNodo(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(long int) - sizeof(void *)));
                liberarNodo(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(long int) - sizeof(void *)));
                printf("\n\tEXLUIDO");
                continue;
            }
            PUSH((*pBuffer + *(int *) *pBuffer - sizeof(long int) - sizeof(void *) - TAM_FILA), *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(long int) - sizeof(void *)));
        }
        // Faz com que a fila principal aponte para a fila secundária onde foram salvos os dados ao extrair da fila principal
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_PRIMEIRO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(long int) + OFFSET_PRIMEIRO);
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_ULTIMO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(long int) + OFFSET_ULTIMO);
        *(int *) *pBuffer -= (TAM_FILA + sizeof(void *) + sizeof(long int));
        break;
    case 4:
        // Aloca espaço no pBuffer para mais uma fila, para dois nodo 
        *pBuffer = realloc(*pBuffer, *(int *) *pBuffer + TAM_FILA + sizeof(void *) + sizeof(void *));
        assert(*pBuffer);
        *(int *) *pBuffer += TAM_FILA + sizeof(void *) + sizeof(void *);
        memset((*pBuffer + *(int *) *pBuffer - (TAM_FILA + sizeof(void *) + sizeof(void *))), 0, (TAM_FILA + sizeof(void *) + sizeof(void *)));
        
        * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)) = alocarNodo();
        assert(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)));
        lerInformacoesPessoa(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)));
        
        while (!EMPTY(*pBuffer + OFFSET_FILA)){
            * (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)) = alocarNodo();
            assert(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)));
            POP(*pBuffer + OFFSET_FILA, *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)));
            // Compara o telefone 
            if ( *(long int *) (*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)) + OFFSET_TELEFONE_PESSOA) == *(long int *) (*(void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)) + OFFSET_TELEFONE_PESSOA)){
                if ( *(short int *) (*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)) + OFFSET_IDADE_PESSOA) == *(short int *) (*(void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)) + OFFSET_IDADE_PESSOA)){
                    if ( strcmp( *(char **) (*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)) + OFFSET_NOME_PESSOA), *(char **) (*(void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)) + OFFSET_NOME_PESSOA)) == 0){
                        printf("\n\tENCONTRADO!!!");
                        imprimeNodo(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)));
                        liberarNodo(*(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)));
                        printf("\n\tEXCLUIDO");
                        continue;
                    }
                }
            }
            PUSH((*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *) - TAM_FILA), *(void **)(*pBuffer + *(int *) *pBuffer - sizeof(void *) - sizeof(void *)));
        }
        liberarNodo(* (void **) (*pBuffer + *(int *) *pBuffer - sizeof(void *)));
        // Faz com que a fila principal aponte para a fila secundária onde foram salvos os dados ao extrair da fila principal
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_PRIMEIRO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(void *) + OFFSET_PRIMEIRO);
        *(void **) (*pBuffer + OFFSET_FILA + OFFSET_ULTIMO) = *(void **) (*pBuffer + *(int *) *pBuffer - TAM_FILA - sizeof(void *) - sizeof(void *) + OFFSET_ULTIMO);
        
        *(int *) *pBuffer -= (TAM_FILA + sizeof(void *) + sizeof(void *));
        break;
    }
    
    // Diminui o buffer 
    *pBuffer = realloc(*pBuffer, *(int *) *pBuffer);
    assert(*pBuffer);
}

// Cria um buffer e todo inicializado com zero e salva o numero de bytes alocados
void *criarBuffer(){
    return memset(memset(malloc(sizeof(int) * 2 + TAM_FILA), 0, sizeof(int) * 2 + TAM_FILA), sizeof(int) * 2 + TAM_FILA, 1);
}

void lerInformacoesPessoa(void *nodo){
    // Leitura do nome
    printf("\nNOME: ");
    while (scanf("%ms", (char **)(nodo + OFFSET_NOME_PESSOA)) == 0){ // A opção %ms aloca memória dinamicamente
        limparStream(stdin);
    }
    // Leitura da idade
    printf("IDADE: ");
    do {
        while (scanf("%hd", (short int *)(nodo + OFFSET_IDADE_PESSOA)) == 0){
            limparStream(stdin);
        }
    } while (*(short int *)(nodo + OFFSET_IDADE_PESSOA)<=0);
    // Leitura do telefone
    printf("TELEFONE: ");
    do {
        while (scanf("%ld", (long int *)(nodo + OFFSET_TELEFONE_PESSOA)) == 0){
            limparStream(stdin);
        }
    } while(*(long int *)(nodo + OFFSET_TELEFONE_PESSOA)<=0);
}
