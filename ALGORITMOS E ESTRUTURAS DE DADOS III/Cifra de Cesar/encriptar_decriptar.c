#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define CHAVES_TESTAR 26

char *encriptar(char *mensagem, int chave, const char inicio_tabela_ascii, const char final_tabela_ascii);
char *desencriptar(char *mensagem_encriptada, int chave, const char inicio_tabela_ascii, const char final_tabela_ascii);
char *lerDeArquivo(char *nome_arquivo);

int main(int argc, char **argv){
    if (argc < 2) {
        printf("\nA forma de utilizar o programa e:");
        printf("\n %s nome_arquivo.txt", argv[0]);
        printf("\n");
        exit(EXIT_FAILURE);
    }
    char *mensagem = lerDeArquivo("./teste.txt");
    printf("\nLido: \n%s", mensagem);
    for (int c = 0; c < CHAVES_TESTAR; c++){
        char *mensagem_desencriptada = desencriptar(mensagem, c, 'A', 'Z');
        printf("\n\nDesencriptada: chave %d \n%s", c, mensagem_desencriptada);
        printf("\n");
        if (mensagem_desencriptada) free(mensagem_desencriptada);
    }
    if (mensagem) free(mensagem);
    return 0;
}

char *encriptar(char *mensagem, int chave, const char inicio_tabela_ascii, const char final_tabela_ascii){
    if (mensagem == NULL) return NULL;
    int tamanho_mensagem_original = strlen(mensagem) + 1;
    char *mensagem_encriptada = (char *) calloc(sizeof(char), tamanho_mensagem_original); 
    if (mensagem_encriptada == NULL) return NULL;

    for (int c = 0; c < tamanho_mensagem_original; c++){
        if (mensagem[c] < inicio_tabela_ascii || mensagem[c] > final_tabela_ascii) {
            mensagem_encriptada[c] = mensagem[c];
            continue;
        }
        
        mensagem_encriptada[c] = (char) ((mensagem[c] - inicio_tabela_ascii + chave) % (final_tabela_ascii + 1 - inicio_tabela_ascii));
        mensagem_encriptada[c] += inicio_tabela_ascii;
    }

    return mensagem_encriptada;
}

char *desencriptar(char *mensagem_encriptada, int chave, const char inicio_tabela_ascii, const char final_tabela_ascii){
    if (mensagem_encriptada == NULL) return NULL;
    int tamanho_mensagem_original = strlen(mensagem_encriptada) + 1;
    char *mensagem_desencriptada = (char *) calloc(sizeof(char), tamanho_mensagem_original); 
    if (mensagem_desencriptada == NULL) return NULL;

    for (int c = 0; c < tamanho_mensagem_original; c++){
        if (mensagem_encriptada[c] < inicio_tabela_ascii || mensagem_encriptada[c] > final_tabela_ascii) {
            mensagem_desencriptada[c] = mensagem_encriptada[c];
            continue;
        }
        
        mensagem_desencriptada[c] = (char) ((mensagem_encriptada[c] - inicio_tabela_ascii - chave));
        if (mensagem_desencriptada[c] < 0){
            mensagem_desencriptada[c] += final_tabela_ascii + 1;
        }
        else {
            mensagem_desencriptada[c] += inicio_tabela_ascii;
        }
    }

    return mensagem_desencriptada;
}

char *lerDeArquivo(char *nome_arquivo){
    FILE *arquivo = (FILE *) fopen(nome_arquivo, "rt");
    if (arquivo == NULL) return NULL;

    char *lido = NULL;
    int tamanho_lido = 0;
    char *linha = NULL;
    size_t tamanho_buffer = 0;
    int caracteres_lidos = 0;
    while ((caracteres_lidos = getline(&linha, &tamanho_buffer, arquivo)) != EOF){
        tamanho_lido += caracteres_lidos;
        lido = realloc(lido, sizeof(char) * tamanho_lido + 1);
        if (lido == NULL) return NULL;
        memcpy((char *) (lido + tamanho_lido- caracteres_lidos), linha, caracteres_lidos);
    }
    if (linha) free(linha);
    lido[tamanho_lido] = '\0';

    fclose(arquivo);
    return lido;
}
