#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int procura(const char *oQueProcurar, const char *emQueProcurar);
char *lerString(void);
char *adicionaNome(char *lista);
int opcaoMenu(void);
char *excluirEntrada(char *string);

int main(void){
    int escolha = 0;
    char *nomes = NULL;
    while (1) {
        escolha = opcaoMenu();
        switch (escolha)
        {
        case 1:
            printf("Digite o nome que voce quer adicionar: ");
            if (!nomes){
                nomes = lerString();
            }
            else {
                nomes = adicionaNome(nomes);
            }
            break;
        case 2:
            if (nomes){
                nomes = excluirEntrada(nomes);
            }
            else {
                printf("Nao existem entradas na lista.");
            }
            break;
        case 3:
            if (nomes){
                printf("%s", nomes);
            }
            else {
                printf("A lista esta vazia!\n");
            }
            break;
        case 4:
            if (nomes) free(nomes);
            exit(0);
            break;
        default:
            break;
        }
    }
    return 0;
}

int opcaoMenu(void){
    int opcao = 0;
    do {
        printf("\n-----------------\n");
        printf("1) Adicionar nome\n");
        printf("2) Remover nome\n");
        printf("3) Listar\n");
        printf("4) Sair\n");
        printf("------------------\n");
        char *linha = lerString();
        if (linha) {
            opcao = atoi(linha);
            free(linha);
        }
    } while (opcao > 4 || opcao <= 0);
    return opcao;
}

char *lerString(void){
    char *string = (char *) calloc(1, sizeof(char));
    if (!string) {
        printf("Nao foi possivel alocar memoria.\n");
        exit(-1);
    }
    int indice = 0;
    do {
        string[indice] = getchar();
        indice++;
        string = (char *)realloc(string, sizeof(char) * (indice +1));
        if (!string){
            printf("\nNao foi possivel alocar mais memoria.\n");
            exit(-1);
        } 
    } while (string[indice -1] != '\n');
    string[indice] = '\0';
    return string;
}

int procura(const char *oQueProcurar, const char *emQueProcurar){
    int indice = 0;
    int tamanho_oQueProcurar = strlen(oQueProcurar);
    int tamanho_emQueProcurar = strlen(emQueProcurar);
    
    if (tamanho_oQueProcurar > tamanho_emQueProcurar){
        return -1;
    }
    for (indice = 0; indice + tamanho_oQueProcurar <= tamanho_emQueProcurar; indice++){
        int caracteresIguais = 0;
        for (int c = 0; c < tamanho_oQueProcurar; c++){
            caracteresIguais += (oQueProcurar[c] == emQueProcurar[c + indice]);
        }
        if (caracteresIguais == tamanho_oQueProcurar){
            return indice;
        }
    }
    return -1;
}

char *excluirEntrada(char *string) {
    printf("Digite o nome a excluir: ");
    char *nomeExcluir = lerString();
    int tamanho_nomeExcluir = strlen(nomeExcluir);
    int tamanho_stringOriginal = strlen(string);
    int posicaoEncontrada = procura(nomeExcluir, string);
    if (posicaoEncontrada != -1){
        for (int c = posicaoEncontrada; c + tamanho_nomeExcluir <= tamanho_stringOriginal; c++){
            string[c] = string[c + tamanho_nomeExcluir];
        }
        string = (char *) realloc(string, sizeof(char) * (tamanho_stringOriginal - tamanho_nomeExcluir));
        if (!string && tamanho_stringOriginal - tamanho_nomeExcluir != 0){
            printf("Erro ao realocar a memoria.");
            exit(-1);
        }
        if (tamanho_stringOriginal - tamanho_nomeExcluir == 0){
            string = NULL;
        }
        printf("Nome excluido: %s\n", nomeExcluir);
    }
    else {
        printf("Nao foi encontrado o nome: %s\n", nomeExcluir);
    }
    free(nomeExcluir);
    return string;
}

char *adicionaNome(char *lista){
    int tamanho = strlen(lista);
    do {
        lista[tamanho] = getchar();
        tamanho++;
        lista = (char *) realloc(lista, sizeof(char) * (tamanho + 1));
        if (!lista){
            printf("Nao foi possivel alocar mais memoria");
            exit(-1);
        }
    } while (lista[tamanho - 1] != '\n');
    lista[tamanho] = '\0';
    return lista;
}