#include <stdio.h>
#include <stdlib.h>

int menu(void);
int lerNumero(void);

int **criaMatriz(int m, int n);
int **redimensionaMatriz(int **mat, int m, int n, int m_antigo, int n_antigo);
void leiaMatriz(int **mat, int m, int n);
int somaMatriz(int **mat, int m, int n);
int *colunaMatriz(int ** mat, int m, int ncoluna);
void liberaMatriz(int **mat, int nlinhas);
void imprimeMatriz(int **mat, int m, int n);
void imprimeVetor (int *vet, int n);

int main(){
    int **matriz = NULL;
    int m = 0;
    int n = 0;
    int ncoluna = 0;
    int opcao = 0;

    int *vetor = NULL;

    int n_antigo = 0;
    int m_antigo = 0;
    char redimensionar = '\0';

    while (1){
        opcao = menu();
        switch (opcao)
        {
        case 1:
            if (!matriz){
                printf("\nMatriz ainda não foi criada.");
                do {
                    printf("\nDigite o numero de Linhas: ");
                    m = lerNumero();
                } while (m <= 0);
                do {
                    printf("\nDigite o numero de Colunas: ");
                    n = lerNumero();
                } while (n <= 0);
                matriz = criaMatriz(m, n);
                printf("\nMatriz de %02d linhas e %02d colunas criada!!!", m, n);
            }
            else {
                printf("\nUma matriz ja existe.");
                printf("\nVoce deseja redimensionar ela ou criar outra do zero? r/n ");
                redimensionar = getchar();
                while (getchar() != '\n');
                if (redimensionar == 'r' || redimensionar == 'R') {
                    n_antigo = n;
                    m_antigo = m;
                    printf("\nA matriz atualmente tem %02d linhas por %02d colunas", m, n);
                    do {
                        printf("\nDigite o novo numero de Linhas: ");
                        m = lerNumero();
                    } while (m <= 0);
                    do {
                        printf("\nDigite o novo numero de Colunas: ");
                        n = lerNumero();
                    } while (n <= 0);
                    matriz = redimensionaMatriz(matriz, m, n, m_antigo, n_antigo);
                    printf("\nA matriz foi redimensionada com sucesso!!!");
                    printf("\nAgora possui ordem de %02dX%02d", m, n);
                }
                else if (redimensionar == 'n' || redimensionar == 'N'){
                    liberaMatriz(matriz, m);
                    do {
                        printf("\nDigite o numero de linhas: ");
                        m = lerNumero();
                    } while (m <= 0);
                    do {
                        printf("\nDigite o numero de Colunas: ");
                        n = lerNumero();
                    } while (n <= 0);
                    matriz = criaMatriz(m, n);
                    printf("\nMatriz de %02d linhas e %02d colunas criada!!!", m, n);
                }
                else {
                    printf("\nOpcao invalida! Nenhuma alteracao efetuada.");
                }
            }
            break;
        case 2:
            leiaMatriz(matriz, m, n);
            break;
        case 3:
            printf("\nA soma dos elementos da matriz e %02d", somaMatriz(matriz, m, n));
            break;
        case 4:
            do {
                printf("\nDigite a coluna que voce deseja: ");
                ncoluna = lerNumero();
            } while (ncoluna > n || ncoluna <= 0);
            vetor = colunaMatriz(matriz, m, ncoluna-1);
            imprimeVetor(vetor, m);
            free(vetor);
            break;
        case 5:
            imprimeMatriz(matriz, m, n);
            break;
        case 6:
            liberaMatriz(matriz, m);
            return 0;
            break;
        }

    }
    return 0;
}

int menu(void){
    int opcao = 0;
    do {
        printf("\n-------------------------------------------------------------------------\n");
        printf("\n1) Criar e redimensionar a matriz");
        printf("\n2) Realizar a leitura dos elementos da matriz");
        printf("\n3) Fornecer a soma dos elementos da matriz");
        printf("\n4) Retornar em um vetor os elementos de uma determinada coluna da matriz");
        printf("\n5) Imprimir a matriz");
        printf("\n6) Sair do programa");
        printf("\n-------------------------------------------------------------------------\n");
        opcao = lerNumero();
    } while ( opcao < 1 || opcao > 6 );
    return opcao;
}

int lerNumero(void){
    char *entrada_do_usuario = NULL;
    size_t caracteres_lidos = 0;
    int numero = 0;

    while (1){
        caracteres_lidos = 0;
        getline(&entrada_do_usuario, &caracteres_lidos, stdin);
        if (caracteres_lidos){
            numero = atoi(entrada_do_usuario);
            free(entrada_do_usuario);
            break;
        }
        if (entrada_do_usuario){
            free(entrada_do_usuario);
            entrada_do_usuario = NULL;
        }
    }

    return numero;
}

int **criaMatriz(int m, int n){
    int **matriz = (int **) malloc(m * sizeof(int *));
    if (!matriz){
        printf("\nNao foi possivel alocar memoria.");
        exit(-1);
    }
    for (int c = 0; c < m; c++){
        matriz[c] = (int *) malloc( n * sizeof(int));
        if (!matriz[c]){
            printf("\nNao foi possivel alocar memoria.");
            exit(-1);
        }
        for (int i = 0; i < n; i++){
            matriz[c][i] = 0;
        }
    }
    return matriz;
}

int **redimensionaMatriz(int **mat, int m, int n, int m_antigo, int n_antigo){
    if (!mat){
        mat = criaMatriz(m, n);
        return mat;
    }
    for (int c = m; c < m_antigo; c++){
        free(mat[c]);
    }
    mat = (int **) realloc(mat, sizeof(int *) * m);
    if (!mat){
        printf("\nNao foi possivel realocar memoria.");
        exit(-1);
    }
    for (int c = 0; c < m; c++){
        mat[c] = (int *) realloc(mat[c], sizeof(int) * n);
        if (!mat[c]){
            printf("\nNao foi possivel realocar memoria.");
            exit(-1);
        }
        for (int i = n_antigo; i < n; i++){
            mat[c][i] = 0;
        }
    }
    return mat;
}

void leiaMatriz(int **mat, int m, int n){
    for (int c = 0; c < m; c++){
        for (int i = 0 ; i < n; i++){
            printf("Matriz[%d][%d]: ", c+1, i+1);
            mat[c][i] = lerNumero();
        }
    }
}

int somaMatriz(int **mat, int m, int n){
    int soma = 0;
    for (int c = 0; c < m; c++){
        for (int i = 0 ; i < n; i++){
            soma += mat[c][i];
        }
    }
    return soma;
}

int *colunaMatriz(int **mat, int m, int ncoluna){
    int *vetor = (int *) malloc(m * sizeof(int));
    if (!vetor){
        printf("\nNao foi possivel alocar memoria.");
        exit(-1);
    }

    for (int c = 0; c < m; c++){
        vetor[c] = mat[c][ncoluna];
    }

    return vetor;
}

void liberaMatriz(int **mat, int nlinhas){
    if (mat){
        for (int c = 0; c < nlinhas; c++){
            if (mat[c]){
                free(mat[c]);
            }
        }
        free(mat);
    }
}

void imprimeMatriz(int **mat, int m, int n){
    for (int c = 0; c < n; c++){
        printf("\t| %02d", c+1);
    }
    for (int c = 0; c < m; c++){
        printf("\n%02d\t", c+1);
        imprimeVetor(mat[c], n);
    }
}

void imprimeVetor (int *vet, int n){
    if (vet){
        for (int i = 0 ; i < n; i++){
            printf("| %d\t", vet[i]);
        }
    }
}