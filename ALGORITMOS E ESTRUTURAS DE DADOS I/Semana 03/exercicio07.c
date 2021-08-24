#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 10
#define MAX_PESSOAS 10

//          int             int             int
//  |---------------+---------------+---------------+->
//  | Nro de Bytes  |  Nro Itens    | Opção Usuário |-> Alocado/liberado pelo resto das funções
//  |---------------+---------------+---------------+->

typedef struct {
    char nome[TAM_NOME];
    int idade;
    long telefone;
} Pessoa;

Pessoa pessoas[MAX_PESSOAS];

void inserePessoa(int *posicao, void **pBuffer);
void apagaPessoa(void **pBuffer);
void buscaPessoa(int *posicao_encontrada, void **pBuffer);
void imprimeBusca(void **pBuffer);
void exibePessoa(int *posicao);
void imprimePessoas(void **pBuffer);
void menu(int *escolha);

// Função principal
int main(){
    // O pBuffer tem as informações na ordem: 
    // int [0] total de espaços que está alocado
    // int [1] total de pessoas na estrutura
    // int [2] opcao que a pessoa escolheu

    void *pBuffer = (void *) calloc(3, sizeof(int));
    if (!pBuffer){
        printf("\nNao foi possivel alocar memoria.");
        exit(-1);
    }
    *((int *) (pBuffer)) = sizeof(int) * 3;

    while (1){

        menu(((int *) (pBuffer + sizeof(int) * 2)));
        switch ( ((int *)(pBuffer))[2] )
        {
        case 1:
            inserePessoa( (int *)(pBuffer + sizeof(int) * 1), &pBuffer);
            break;
        case 2:
            apagaPessoa(&pBuffer);
            break;
        case 3:
            imprimePessoas(&pBuffer);
            break;
        case 4:
            imprimeBusca(&pBuffer);
            break;
        case 5:
            free(pBuffer);
            exit(0);
            break;
        default:
            break;
        }
    }
}

// Exibe as opções do usuário e lê com o scanf e valida a entrada
void menu(int *escolha){
    if (!escolha){
        printf("Ponteiro NULO");
        exit(-1);
    }

    do {
        printf("\n\t---------");
        printf("\n1 - Incluir contato");
        printf("\n2 - Apagar contato");
        printf("\n3 - Listar contatos");
        printf("\n4 - Buscar contato");
        printf("\n5 - Sair");
        printf("\n\t---------\n");
        while (scanf("%d%*c", escolha) == 0);
    } while (*escolha > 5 || *escolha < 1);
}

// Lê as informações de uma pessoa
void inserePessoa(int *posicao, void **pBuffer){
    if (*posicao < 0 || *posicao > MAX_PESSOAS){
        printf("Posicao invalida.");
        return;
    }
    
    printf("\nDigite o nome: ");
    fgets(pessoas[*posicao].nome, TAM_NOME, stdin);

    // A função scanf retorna 1 se ela encontrar o formato especificado e 0 caso contrario
    do {
        printf("\nDigite a idade: ");
    } while (scanf("%d%*c", &(pessoas[*posicao].idade)) == 0);

    do {
        printf("\nDigite o telefone: ");
    } while (scanf("%ld%*c", &(pessoas[*posicao].telefone)) == 0);
    (*((int *)(*pBuffer + sizeof(int) * 1)))++;
}

// Imprime as informações de uma pessoa
void exibePessoa(int *posicao){
    printf("\nNOME: \t\t%s", pessoas[*posicao].nome);
    printf("IDADE: \t\t%d", pessoas[*posicao].idade);
    printf("\nTELEFONE: \t%ld\n", pessoas[*posicao].telefone);
}

void imprimePessoas(void **pBuffer){
    // Aumenta o buffer para salvar a variavel que será utilizada no contador
    // Para facilitar o entendimento:
    // *((int *) (*pBuffer)) = tamnho total do buffer (bytes alocados)
    // *((int *) (*pBuffer + *((int *) (*pBuffer)))) = equivalente a uma variavel contadora
    // *((int *) (*pBuffer + sizeof(int))) = número de pessoas na estrutura

    if ( *((int *) (*pBuffer + sizeof(int))) <= 0 ){
        printf("\nNenhuma pessoa na estrutura");
        return;
    }

    *pBuffer = (void *) realloc(*pBuffer, *((int *)(*pBuffer)) + sizeof(int));
    
    if (!(*pBuffer)){
        printf("\nNao foi possivel alocar mais memoria.\n");
        exit(-1);
    }
    
    for (*((int *) (*pBuffer + *((int *) (*pBuffer)))) = 0; *((int *) (*pBuffer + *((int *) (*pBuffer)))) < *((int *) (*pBuffer + sizeof(int))); (*((int *) (*pBuffer + *((int *) (*pBuffer)))))++){
        exibePessoa((int *) (*((int *) (*pBuffer)) + *pBuffer));
    }

    // Reduz o buffer, já que a o contador não é mais utilizado
    *pBuffer = (void *) realloc(*pBuffer, *((int *)(*pBuffer)));
    if (!*pBuffer){
        printf("\nNao foi possivel alocar mais memoria.");
        exit(-1);
    }

}


void busca(int *posicao_encontrada, void **pBuffer){
    // Essa função vai alocar memória no pBuffer para:
    // a opção que a pesoa quer buscar
    // o campo que a pessoa vai buscar
    if ( *((int *) (*pBuffer + sizeof(int))) <= 0 ){
        printf("\nNenhuma pessoa na estrutura");
        return;
    }

    // Aloca memória para a escolha da pessoa
    *pBuffer = (void *) realloc(*pBuffer, *((int *) (*pBuffer)) + sizeof(int));
    if (!*pBuffer){
        printf("\nNao foi possivel realocar memoria.");
        exit(-1);
    }

    *((int *)(*pBuffer)) += sizeof(int);

    do {
        printf("\n1 - Buscar por NOME ");
        printf("\n2 - Buscar por IDADE ");
        printf("\n3 - Buscar por TELEFONE\n");
        while( scanf("%d%*c", (int *) (*pBuffer + *((int *) (*pBuffer)) - sizeof(int))) == 0);   // Fica repetindo até digitar algo numérico
    } while (*((int *) (*pBuffer + *((int *) (*pBuffer)) - sizeof(int))) < 0 || *((int *) (*pBuffer + *((int *) (*pBuffer)) - sizeof(int))) > 3);

    switch (*((int *) (*pBuffer + *((int *) (*pBuffer)) - sizeof(int))))
    {
    case 1:
        // O tamanho que o buffer fica após realocar com os char não é contabilizado
        *pBuffer = (void *) realloc(*pBuffer, *((int *)(*pBuffer)) + sizeof(char) * TAM_NOME);
        if (!*pBuffer){
            printf("\nNao foi possivel realocar a memoria.\n");
            exit(-1);
        }
        printf("\nDigite o NOME: ");
        fgets((char *) (*pBuffer + *((int *) (*pBuffer))), TAM_NOME, stdin);

        // Reutiliza o mesmo espaço alocado para a opção do usuário como contador
        for (*((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int))) = 0; *((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int))) < *((int *) (*pBuffer + sizeof(int))); (*((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int))))++ ){
            if (strcmp(pessoas[*((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int)))].nome, (char *) (*pBuffer + *((int *) *pBuffer))) == 0){
                *posicao_encontrada = *((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int)));
                break;
            }
        }

        *pBuffer = (void *) realloc(*pBuffer, *((int *) *pBuffer) - sizeof(int));
        if (!*pBuffer){
            printf("\nNao foi possivel realocar memória.\n");
            exit(-1);
        }
        *((int *) (*pBuffer)) -= sizeof(int); // Desconta o que foi desalocado 
        break;
    case 2:
        // O tamanho que o buffer fica após realocar com int não é contabilizado
        *pBuffer = (void *) realloc(*pBuffer, *((int *)(*pBuffer)) + sizeof(int));
        if (!*pBuffer){
            printf("\nNao foi possivel realocar a memoria.\n");
            exit(-1);
        }
        
        do {
            printf("\nDigite a IDADE: ");
        } while (scanf("%d%*c", (int *) (*pBuffer + *((int *) (*pBuffer)))) == 0);

        // Reutiliza o mesmo espaço alocado para a opção do usuário como contador
        for (*((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int))) = 0; *((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int))) < *((int *) (*pBuffer + sizeof(int))); (*((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int))))++ ){
            if (pessoas[*((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int)))].idade == *((int *) (*pBuffer + *((int *) *pBuffer))) ){
                *posicao_encontrada = *((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int)));
                break;
            }
        }

        *pBuffer = (void *) realloc(*pBuffer, *((int *) *pBuffer) - sizeof(int));
        if (!*pBuffer){
            printf("\nNao foi possivel realocar memória.\n");
            exit(-1);
        }
        *((int *) (*pBuffer)) -= sizeof(int); // Desconta o que foi desalocado 
        break;

    case 3:
        // O tamanho que o buffer fica após realocar com long não é contabilizado
        *pBuffer = (void *) realloc(*pBuffer, *((int *)(*pBuffer)) + sizeof(long));
        if (!*pBuffer){
            printf("\nNao foi possivel realocar a memoria.\n");
            exit(-1);
        }
        
        do {
            printf("\nDigite o TELEFONE: ");
        } while (scanf("%ld%*c", (long *) (*pBuffer + *((int *) (*pBuffer)))) == 0);

        // Reutiliza o mesmo espaço alocado para a opção do usuário como contador
        for (*((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int))) = 0; *((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int))) < *((int *) (*pBuffer + sizeof(int))); (*((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int))))++ ){
            if (pessoas[*((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int)))].telefone == *((long *) (*pBuffer + *((int *) *pBuffer))) ){
                *posicao_encontrada = *((int *) (*pBuffer + *((int *) *pBuffer) - sizeof(int)));
                break;
            }
        }

        *pBuffer = (void *) realloc(*pBuffer, *((int *) *pBuffer) - sizeof(int));
        if (!*pBuffer){
            printf("\nNao foi possivel realocar memória.\n");
            exit(-1);
        }
        *((int *) (*pBuffer)) -= sizeof(int); // Desconta o que foi desalocado 
        break;
    }
}

void imprimeBusca(void **pBuffer){
    //          int             int             int           int *
    //  |---------------+---------------+---------------+----------------+->
    //  | Nro de Bytes  |  Nro Itens    | Opção Usuário | Pos encontrada |->
    //  |---------------+---------------+---------------+----------------+->
    //  |               Já vem da função main           |
    
    // Aloca espaço para o local onde será salvo o indice do resultado da busca
    *pBuffer = (void *) realloc(*pBuffer, *((int *) (*pBuffer)) + sizeof(int *));
    if (!*pBuffer){
        printf("\nNao foi possivel realocar a memoria.\n");
        exit(-1);
    }

    // Incrementa o local com o valor do tamanho do buffer
    *((int *) (*pBuffer)) += sizeof(int *);
    // Inicializa o local de onde será salvo o resultado da função de busca
    *((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *))) = (int *) malloc(sizeof(int));
    if (!*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *)))){
        printf("\nNao foi possivel alocar memoria.\n");
        exit(-1);
    }
    *(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *)))) = -1;


    busca(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *))), pBuffer);

    if (*(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *)))) >= 0){
        exibePessoa(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *))));
    }
    else {
        printf("\nNao encontrado!!\n");
    }

    free(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *))));

    *pBuffer = (void *) realloc(*pBuffer, *((int *) (*pBuffer)) - sizeof(int *));
    if (!*pBuffer){
        printf("\nNao foi possivel realocar a memoria.\n");
        exit(-1);
    }
    // Decrementa o local com o valor do tamanho do buffer
    *((int *) (*pBuffer)) -= sizeof(int *);
}

void apagaPessoa(void **pBuffer){
    //          int             int             int           int *
    //  |---------------+---------------+---------------+----------------+->
    //  | Nro de Bytes  |  Nro Itens    | Opção Usuário | Pos encontrada |->
    //  |---------------+---------------+---------------+----------------+->
    //  |               Já vem da função main           |
    
    // Aloca espaço para o local onde será salvo o indice do resultado da busca
    *pBuffer = (void *) realloc(*pBuffer, *((int *) (*pBuffer)) + sizeof(int *));
    if (!*pBuffer){
        printf("\nNao foi possivel realocar a memoria.\n");
        exit(-1);
    }

    // Incrementa o local com o valor do tamanho do buffer
    *((int *) (*pBuffer)) += sizeof(int *);
    // Inicializa o local de onde será salvo o resultado da função de busca
    *((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *))) = (int *) malloc(sizeof(int));
    if (!*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *)))){
        printf("\nNao foi possivel alocar memoria.\n");
        exit(-1);
    }
    *(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *)))) = -1;

    busca(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *))), pBuffer);

    if (*(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *)))) >= 0){
        while (*(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *)))) < MAX_PESSOAS -1){
            memcpy(&pessoas[*(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *))))],  &pessoas[*(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *)))) +1], sizeof(Pessoa));
            (*(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *)))))++;
        }
        (*((int *) (*pBuffer + sizeof(int))))--;
        printf("\nRegistro excluido");
    }
    else {
        printf("\nNao encontrado!!\n");
    }

    free(*((int **)(*pBuffer +  *((int *) (*pBuffer)) - sizeof(int *))));

    *pBuffer = (void *) realloc(*pBuffer, *((int *) (*pBuffer)) - sizeof(int *));
    if (!*pBuffer){
        printf("\nNao foi possivel realocar a memoria.\n");
        exit(-1);
    }
    // Decrementa o local com o valor do tamanho do buffer
    *((int *) (*pBuffer)) -= sizeof(int *);
}