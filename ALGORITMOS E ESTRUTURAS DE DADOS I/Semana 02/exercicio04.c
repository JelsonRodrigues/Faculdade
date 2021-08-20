#include <stdio.h>
#include <stdlib.h>

void *meuRealloc(void *ponteiroParaRealocar, size_t novoTamanho);
void *meuRealloc2(void *ponteiroParaRealocar, size_t novoTamanho, size_t tamanhoAnterior);
void copiaMemoria(void *destino, void *origem, size_t numeroBytes);
char *lerString();
char *lerString2();

int main(){
    printf("Digite algo: ");
    char *teste = lerString2();
    printf("Voce digitou: %s\n", teste);

    meuRealloc(teste, 0);
    return 0;
}

void *meuRealloc(void *ponteiroParaRealocar, size_t novoTamanho){
    if (!ponteiroParaRealocar){
        if (novoTamanho){
            ponteiroParaRealocar = (void *) malloc(novoTamanho);
            if (!ponteiroParaRealocar){
                ponteiroParaRealocar = NULL;
            }
        }
    }
    else{
        if (novoTamanho){
            void *bufferNovo = malloc(novoTamanho);
            if (!bufferNovo){
                free(ponteiroParaRealocar);
                return NULL;
            }
            copiaMemoria(bufferNovo, ponteiroParaRealocar, novoTamanho);
            free(ponteiroParaRealocar);
            ponteiroParaRealocar = bufferNovo;
        }
        else {
            free(ponteiroParaRealocar);
            ponteiroParaRealocar = NULL;
        }
    }
    return ponteiroParaRealocar;
}

void *meuRealloc2(void *ponteiroParaRealocar, size_t novoTamanho, size_t tamanhoAnterior){
    if (!ponteiroParaRealocar){
        if (novoTamanho){
            ponteiroParaRealocar = (void *) malloc(novoTamanho);
            if (!ponteiroParaRealocar){
                ponteiroParaRealocar = NULL;
            }
        }
    }
    else{
        if (novoTamanho){
            void *bufferNovo = malloc(novoTamanho);
            if (!bufferNovo){
                free(ponteiroParaRealocar);
                return NULL;
            }
            copiaMemoria(bufferNovo, ponteiroParaRealocar, tamanhoAnterior);
            free(ponteiroParaRealocar);
            ponteiroParaRealocar = bufferNovo;
        }
        else {
            free(ponteiroParaRealocar);
            ponteiroParaRealocar = NULL;
        }
    }
    return ponteiroParaRealocar;
}

void copiaMemoria(void *destino, void *origem, size_t numeroBytes){
    if (destino && origem){
        for (size_t c = 0; c < numeroBytes; c++){
            *((char *) (destino + c * sizeof(char))) = *((char *) (origem + c * sizeof(char)));
        }
    }
}

char *lerString(){
    int numeroCaracteres = 0;
    char *string = (char *) malloc(sizeof(char));
    if (!string){
        printf("Nao foi possivel alocar memoria;\n");
        exit(-1);
    }

    do {
        string[numeroCaracteres] = getchar();
        numeroCaracteres++;
        string = (char *) meuRealloc(string, sizeof(char) * (numeroCaracteres + 1));
        if (!string){
            printf("Nao foi possivel alocar memoria;\n");
            exit(-1);
        }
    } while (string[numeroCaracteres -1] != '\n');
    string[numeroCaracteres] = '\0';
    
    return string;
}

char *lerString2(){
    int numeroCaracteres = 0;
    char *string = (char *) malloc(sizeof(char));
    if (!string){
        printf("Nao foi possivel alocar memoria;\n");
        exit(-1);
    }

    do {
        string[numeroCaracteres] = getchar();
        numeroCaracteres++;
        string = (char *) meuRealloc2(string, sizeof(char) * (numeroCaracteres + 1), sizeof(char) * (numeroCaracteres));
        if (!string){
            printf("Nao foi possivel alocar memoria;\n");
            exit(-1);
        }
    } while (string[numeroCaracteres -1] != '\n');
    string[numeroCaracteres] = '\0';
    
    return string;
}