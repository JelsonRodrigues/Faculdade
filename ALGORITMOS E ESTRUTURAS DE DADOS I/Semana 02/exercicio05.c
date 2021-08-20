#include <stdio.h>
#include <stdlib.h>

#define TAM_NOME 30

typedef struct{
    char nome[TAM_NOME];
    int idade;
    int altura;
}Pessoa;

void imprime(Pessoa *lista, int totalItens);
Pessoa *adicionaPessoa(Pessoa *lista, int local);

int main(){
    Pessoa *lista = NULL;
    char sair = '\0';
    int numeroPessoas = 0;
    do {
        lista = adicionaPessoa(lista, numeroPessoas);
        numeroPessoas++;
        printf("Voce deseja sair? s/n ");
        sair = getchar();
        while(getchar() != '\n');
    } while (sair != 's' && sair != 'S');
    
    imprime(lista, numeroPessoas);
    free(lista);
    return 0;
}

Pessoa *adicionaPessoa(Pessoa *lista, int local){
    if (!lista){
        lista = (Pessoa *) calloc(1, sizeof(Pessoa));
        local = 0;
    } else{
        lista = realloc(lista, sizeof(Pessoa) * (local +1));
    }
    if (!lista){
        printf("Nao foi possivel alocar memoria.\n");
        exit(-1);
    }
    
    printf("Digite o nome da pessoa: ");
    fgets(lista[local].nome, TAM_NOME, stdin);
    printf("Digite a idade: ");
    scanf("%d%*c", &(lista[local].idade));
    printf("Digite a altura: ");
    scanf("%d%*c", &(lista[local].altura));
    
    return lista;
}

void imprime(Pessoa *lista, int totalItens){
    for (int c = 0; c < totalItens; c++){
        printf("\n-------|Pessoa %02d|-------\n", c + 1);
        printf("\nNome: \t%s", lista[c].nome);
        printf("Idade: \t%02d", lista[c].idade);
        printf("\nAltura:\t%02d\n", lista[c].altura);
    }
}