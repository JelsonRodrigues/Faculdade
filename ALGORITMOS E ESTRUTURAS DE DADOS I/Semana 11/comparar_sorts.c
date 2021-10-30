#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

typedef struct _resultado {
    struct timeval *quickSort;
    struct timeval *mergeSort;
    struct timeval *insertionSort;
    struct timeval *selectionSort; 
    struct timeval *bubbleSort;
    uint num_quick;
    uint num_merge;
    uint num_insertion;
    uint num_selection;
    uint num_bubble;
} Resultado;

typedef enum {
    QUICK_SORT = 1,
    MERGE_SORT = 2,
    INSERTION_SORT = 3,
    SELECTION_SORT = 4,
    BUBBLE_SORT = 5
} Algoritmo;

// Funções relativa aos vetores
int *criarVetorAleatorio(const uint numero_de_elementos);
int *criarVetorOrdenado(const uint numero_de_elementos);
int *criarVetorOrdemInversa(const uint numero_de_elementos);
void randomizaElementos(int *vetor, const uint numero_elementos);
void imprimirVetor(const int *vetor, const uint numero_elementos);
bool estaOrdenado(const int *vetor, const uint numero_elementos);

// Funções de ordenação
void ordenar(int* vetor, const uint numero_elementos, Algoritmo algoritmo);
void quickSort(int *vetor, const uint numero_elementos);
void mergeSort(int *vetor, const uint numero_elementos);
void merge(int *esquerda, int *direita, const uint tam_esquerda, const uint tam_direita);
void insertionSort(int *vetor, const uint numero_elementos);
void selectionSort(int *vetor, const uint numero_elementos);
void bubbleSort(int *vetor, const uint numero_elementos);

// Funções auxiliares
void salvarResultadosPassadas(Resultado *res, uint numero_de_elementos);
void imprimirTempo(struct timeval *inicio, struct timeval *fim, Algoritmo algoritmo);
Resultado criaResultado();
Resultado mediaResultados(Resultado *r);
void adicionaResultado(Resultado *res, struct timeval *inicio, struct timeval *fim, Algoritmo algoritmo);
void liberarResultados(Resultado *resultado);
void zerarResultado(Resultado *res);

/* ---------------- FUNÇÃO PRINCIPAL ----------------*/
int main(int argc, char **argv) {
    uint tamanho = 0;
    
    // Foi passado parâmetro?
    if (argc > 1) {
        if (argv[1][0] == '-') {
            tamanho = atoi(argv[1] + sizeof(char));
        }
        else {
            tamanho = atoi(argv[1]);
        }
    }
    
    if (tamanho == 0){
        printf("\nDigite o numero de elementos para sortear: ");
        do {
            while (scanf("%u", &tamanho) == 0) {
                stdin->_IO_read_ptr = stdin->_IO_read_end;
            }
        } while (tamanho == 0);
    }

    // Variáveis
    Resultado resultados;
    struct timeval inicio, fim;
    
    int *vetor = NULL;
    int *vetor_ordenar = NULL;
    uint passadas = 5;
    
    //for (uint c = 1; c <= 20; c++) {
        resultados = criaResultado();
        
        //tamanho += 10000;
        vetor = criarVetorAleatorio(tamanho);
        vetor_ordenar = criarVetorAleatorio(tamanho);
        printf("\nColetando dados para vetor com tamanho: %d", tamanho);
        for (uint i = 0; i < passadas; i++) {
            printf("\nPassada %d/%d\n", i+1, passadas);
            memcpy(vetor_ordenar, vetor, tamanho * sizeof(int));
            gettimeofday(&inicio, 0);
            ordenar(vetor_ordenar, tamanho, QUICK_SORT);
            gettimeofday(&fim, 0);
            imprimirTempo(&inicio, &fim, QUICK_SORT);
            adicionaResultado(&resultados, &inicio, &fim, QUICK_SORT);

            memcpy(vetor_ordenar, vetor, tamanho * sizeof(int));
            gettimeofday(&inicio, 0);
            ordenar(vetor_ordenar, tamanho, MERGE_SORT);
            gettimeofday(&fim, 0);
            imprimirTempo(&inicio, &fim, MERGE_SORT);
            adicionaResultado(&resultados, &inicio, &fim, MERGE_SORT);
            
            memcpy(vetor_ordenar, vetor, tamanho * sizeof(int));
            gettimeofday(&inicio, 0);
            ordenar(vetor_ordenar, tamanho, INSERTION_SORT);
            gettimeofday(&fim, 0);
            imprimirTempo(&inicio, &fim, INSERTION_SORT);
            adicionaResultado(&resultados, &inicio, &fim, INSERTION_SORT);

            memcpy(vetor_ordenar, vetor, tamanho * sizeof(int));
            gettimeofday(&inicio, 0);
            ordenar(vetor_ordenar, tamanho, SELECTION_SORT);
            gettimeofday(&fim, 0);
            imprimirTempo(&inicio, &fim, SELECTION_SORT);
            adicionaResultado(&resultados, &inicio, &fim, SELECTION_SORT);
            
            memcpy(vetor_ordenar, vetor, tamanho * sizeof(int));
            gettimeofday(&inicio, 0);
            ordenar(vetor_ordenar, tamanho, BUBBLE_SORT);
            gettimeofday(&fim, 0);
            imprimirTempo(&inicio, &fim, BUBBLE_SORT);
            adicionaResultado(&resultados, &inicio, &fim, BUBBLE_SORT);
        }

        free(vetor_ordenar);
        free(vetor);

        salvarResultadosPassadas(&resultados, tamanho);
        liberarResultados(&resultados);
    //}
    
    printf("\nFIM\n");
    return EXIT_SUCCESS;
}

// Inicializa uma struct Resultado
Resultado criaResultado(){
    Resultado resultado;
    zerarResultado(&resultado);
    return resultado;
}

void zerarResultado(Resultado *res){
    if (res == NULL) return;
    res->quickSort = NULL;
    res->mergeSort = NULL;
    res->insertionSort = NULL;
    res->selectionSort = NULL;
    res->bubbleSort = NULL;
    res->num_quick = 0;
    res->num_merge = 0;
    res->num_insertion = 0;
    res->num_selection = 0;
    res->num_bubble = 0;
}

void adicionaResultado(Resultado *res, struct timeval *inicio, struct timeval *fim, Algoritmo algoritmo) {
    switch (algoritmo)
    {
    case QUICK_SORT:
        res->quickSort = (struct timeval *) realloc(res->quickSort, sizeof(struct timeval) * (res->num_quick + 1));
        if (res->quickSort != NULL){
            res->quickSort[res->num_quick].tv_usec = fim->tv_usec - inicio->tv_usec;
            res->quickSort[res->num_quick].tv_sec = fim->tv_sec - inicio->tv_sec;

            if (res->quickSort[res->num_quick].tv_usec < 0){
                res->quickSort[res->num_quick].tv_sec--;
                res->quickSort[res->num_quick].tv_usec *= -1;
            }

            res->num_quick++;
        }
        else {
            res->num_quick = 0;
        }
        break;
    case MERGE_SORT:
        res->mergeSort = (struct timeval *) realloc(res->mergeSort, sizeof(struct timeval) * (res->num_merge + 1));
        if (res->mergeSort != NULL){
            res->mergeSort[res->num_merge].tv_usec = fim->tv_usec - inicio->tv_usec;
            res->mergeSort[res->num_merge].tv_sec = fim->tv_sec - inicio->tv_sec;

            if (res->mergeSort[res->num_merge].tv_usec < 0){
                res->mergeSort[res->num_merge].tv_sec--;
                res->mergeSort[res->num_merge].tv_usec *= -1;
            }

            res->num_merge++;
        }
        else {
            res->num_merge = 0;
        }
        break;
    case INSERTION_SORT:
        res->insertionSort = (struct timeval *) realloc(res->insertionSort, sizeof(struct timeval) * (res->num_insertion + 1));
        if (res->insertionSort != NULL){
            res->insertionSort[res->num_insertion].tv_usec = fim->tv_usec - inicio->tv_usec;
            res->insertionSort[res->num_insertion].tv_sec = fim->tv_sec - inicio->tv_sec;

            if (res->insertionSort[res->num_insertion].tv_usec < 0){
                res->insertionSort[res->num_insertion].tv_sec--;
                res->insertionSort[res->num_insertion].tv_usec *= -1;
            }
            
            res->num_insertion++;
        }
        else {
            res->num_insertion = 0;
        }
        break;
    case SELECTION_SORT:
        res->selectionSort = (struct timeval *) realloc(res->selectionSort, sizeof(struct timeval) * (res->num_selection + 1));
        if (res->selectionSort != NULL){
            res->selectionSort[res->num_selection].tv_usec = fim->tv_usec - inicio->tv_usec;
            res->selectionSort[res->num_selection].tv_sec = fim->tv_sec - inicio->tv_sec;

            if (res->selectionSort[res->num_selection].tv_usec < 0){
                res->selectionSort[res->num_selection].tv_sec--;
                res->selectionSort[res->num_selection].tv_usec *= -1;
            }

            res->num_selection++;
        }
        else {
            res->num_selection = 0;
        }
        break;
    case BUBBLE_SORT:
        res->bubbleSort = (struct timeval *) realloc(res->bubbleSort, sizeof(struct timeval) * (res->num_bubble + 1));
        if (res->bubbleSort != NULL){
            res->bubbleSort[res->num_bubble].tv_usec = fim->tv_usec - inicio->tv_usec;
            res->bubbleSort[res->num_bubble].tv_sec = fim->tv_sec - inicio->tv_sec;

            if (res->bubbleSort[res->num_bubble].tv_usec < 0){
                res->bubbleSort[res->num_bubble].tv_sec--;
                res->bubbleSort[res->num_bubble].tv_usec *= -1;
            }

            res->num_bubble++;
        }
        else {
            res->num_bubble = 0;
        }
        break;
    }
}

void liberarResultados(Resultado *resultado){
    if (resultado == NULL) return;
    free(resultado->quickSort);
    free(resultado->mergeSort);
    free(resultado->insertionSort);
    free(resultado->selectionSort);
    free(resultado->bubbleSort);
    zerarResultado(resultado);
}

void imprimirTempo(struct timeval *inicio, struct timeval *fim, Algoritmo algoritmo){
    if (inicio == NULL || fim == NULL) {
        printf("\nTempos inválidos\n");
        return;
    }
    char nome_algoritmo[] = "-------------";
    switch (algoritmo)
    {
    case QUICK_SORT:
        strcpy(nome_algoritmo, "QuickSort");
        break;
    case MERGE_SORT:
        strcpy(nome_algoritmo, "MergeSort");
        break;
    case INSERTION_SORT:
        strcpy(nome_algoritmo, "InsertionSort");
        break;
    case SELECTION_SORT:
        strcpy(nome_algoritmo, "SelectionSort");
        break;
    case BUBBLE_SORT:
        strcpy(nome_algoritmo, "BubbleSort");
        break;
    }
    time_t diferenca_segundos = (fim->tv_sec - inicio->tv_sec);
    suseconds_t diferenca_micro_segundos = (fim->tv_usec - inicio->tv_usec);
    if (diferenca_micro_segundos < 0) {
        diferenca_micro_segundos *= -1;
        diferenca_segundos--;
    }
    uint minutos = diferenca_segundos / 60;
    uint segundos = diferenca_segundos % 60;
    uint milisegundos = diferenca_micro_segundos / 1000;
    uint microssegundos = diferenca_micro_segundos % 1000;
    
    printf("Tempo de execucao do algoritmo %13s:\t %02um%02us%03ums%03uus\n", nome_algoritmo,
                                            minutos, 
                                            segundos,
                                            milisegundos,
                                            microssegundos);
}

void salvarResultadosPassadas(Resultado *res, uint numero_de_elementos) {
    
    /*
    // O logaritmo de um número na base que ele está representado arredondado para cima é o número de dígitos necessários para representá-lo
    // ex.: 236 -> log10(236) = 2,37... -> ceil(2.37...) = 3
    // então soma-se 1 para a string guardar o caractere terminador \0
    // Abaixo eu arredondo para baixo e somo com 2
    uint digitos_necessarios = ((int) log10(numero_de_elementos) + 2);
    char *numero_como_string = (char *) malloc(sizeof(char) * digitos_necessarios );
    snprintf(numero_como_string, digitos_necessarios, "%u", numero_de_elementos);
    */

    bool arquivo_ja_existe = false;

    FILE *arquivo_final;

    if ((arquivo_final = fopen("resultados.csv", "rt")) != NULL){
        arquivo_ja_existe = true;
    }
    else {
        fclose(arquivo_final);
    }
  
    if (arquivo_ja_existe == false){
        arquivo_final = fopen("resultados.csv", "wt");
        if (arquivo_final == NULL) {
            printf("\nNao foi possivel salvar as informacoes!!\n");
            return;
        }
        // Cabeçalho
        fprintf(arquivo_final, "passes,QuickSort,MergeSort,InsertionSort,SelectionSort,BubbleSort\n");
    }
    else {
        arquivo_final = fopen("resultados.csv", "at");
        if (arquivo_final == NULL) {
            printf("\nNao foi possivel salvar as informacoes!!\n");
            return;
        }
    }

    fprintf(arquivo_final, "%d,", numero_de_elementos);

    time_t media_segundos = 0;
    suseconds_t media_micro_segundos = 0;

    for (uint c = 0; c < res->num_quick; c++){
        media_segundos += res->quickSort[c].tv_sec;
        media_micro_segundos += res->quickSort[c].tv_usec;
    }
    media_segundos /= res->num_quick;
    media_micro_segundos /= res->num_quick;
    fprintf(arquivo_final, "%02ld:%02ld:%06ld,", (media_segundos / 60), (media_segundos % 60), media_micro_segundos);

    media_segundos = 0;
    media_micro_segundos = 0;
    for (uint c = 0; c < res->num_merge; c++){
        media_segundos += res->mergeSort[c].tv_sec;
        media_micro_segundos += res->mergeSort[c].tv_usec;
    }
    media_segundos /= res->num_merge;
    media_micro_segundos /= res->num_merge;
    fprintf(arquivo_final, "%02ld:%02ld:%06ld,", (media_segundos / 60), (media_segundos % 60), media_micro_segundos);
    
    media_segundos = 0;
    media_micro_segundos = 0;
    for (uint c = 0; c < res->num_insertion; c++){
        media_segundos += res->insertionSort[c].tv_sec;
        media_micro_segundos += res->insertionSort[c].tv_usec;
    }
    media_segundos /= res->num_insertion;
    media_micro_segundos /= res->num_insertion;
    fprintf(arquivo_final, "%02ld:%02ld:%06ld,", (media_segundos / 60), (media_segundos % 60), media_micro_segundos);

    media_segundos = 0;
    media_micro_segundos = 0;
    for (uint c = 0; c < res->num_selection; c++){
        media_segundos += res->selectionSort[c].tv_sec;
        media_micro_segundos += res->selectionSort[c].tv_usec;
    }
    media_segundos /= res->num_selection;
    media_micro_segundos /= res->num_selection;
    fprintf(arquivo_final, "%02ld:%02ld:%06d,", (media_segundos / 60), (media_segundos % 60), media_micro_segundos);

    media_segundos = 0;
    media_micro_segundos = 0;
    for (uint c = 0; c < res->num_bubble; c++){
        media_segundos += res->bubbleSort[c].tv_sec;
        media_micro_segundos += res->bubbleSort[c].tv_usec;
    }
    media_segundos /= res->num_bubble;
    media_micro_segundos /= res->num_bubble;
    fprintf(arquivo_final, "%02ld:%02ld:%06ld", (media_segundos / 60), (media_segundos % 60), media_micro_segundos);

    fprintf(arquivo_final, "\n");
    
    fclose(arquivo_final);
    arquivo_final = NULL;
}

int *criarVetorAleatorio(const uint numero_de_elementos){
    int *vetor = (int *) malloc(sizeof(int) * numero_de_elementos);
    if (vetor == NULL) return NULL;
    randomizaElementos(vetor, numero_de_elementos);
    return vetor;
}

void randomizaElementos(int *vetor, const uint numero_elementos){
    if (vetor){
        srand(time(NULL));
        for (uint i = 0; i < numero_elementos; i++)
        {
            vetor[i] = (rand() % numero_elementos);
        }
    }
}

int *criarVetorOrdenado(const uint numero_de_elementos){
    int *vetor = (int *) malloc(sizeof(int) * numero_de_elementos);
    if (vetor == NULL) return NULL;
    for (uint c = 0; c < numero_de_elementos; c++) {
        vetor[c] = c;
    }
    return vetor;    
}

int *criarVetorOrdemInversa(const uint numero_de_elementos){
    int *vetor = (int *) malloc(sizeof(int) * numero_de_elementos);
    if (vetor == NULL) return NULL;
    for (uint c = 0; c < numero_de_elementos; c++) {
        vetor[c] = numero_de_elementos - c;
    }
    return vetor;
}

void imprimirVetor(const int *vetor, const uint numero_elementos) {
    if (vetor == NULL) return;
    printf("\n");
    for (uint c = 0; c < numero_elementos; c++){
        printf("% 2d ", vetor[c]);
    }
    printf("\n");
}

bool estaOrdenado(const int *vetor, const uint numero_elementos){
    if (vetor == NULL) return true;
    for (uint c = 0; c < numero_elementos - 1; c++) {
        if (vetor[c] > vetor [c+1]){
            return false;
        }
    }
    return true;
}

void ordenar(int* vetor, const uint numero_elementos, Algoritmo algoritmo){
    if (vetor == NULL) return;
    switch (algoritmo)
    {
    case QUICK_SORT:
        quickSort(vetor, numero_elementos);
        break;
    case MERGE_SORT:
        mergeSort(vetor, numero_elementos);
        break;
    case INSERTION_SORT:
        insertionSort(vetor, numero_elementos);
        break;
    case SELECTION_SORT:
        selectionSort(vetor, numero_elementos);
        break;
    case BUBBLE_SORT:
        bubbleSort(vetor, numero_elementos);
        break;
    }
}

void quickSort(int *vetor, const uint numero_elementos) {
    if (vetor == NULL || numero_elementos <= 1) return;
    
    uint pos_pivo = (rand() % numero_elementos);
    int pivo = vetor[pos_pivo];
    int i = 0;
    int j = numero_elementos - 1;
    int temp = 0;
    
    while (i <= j) {
        while (vetor[i] < pivo) i++;
        while (vetor[j] > pivo) j--;
        
        // Swap
        if (i <= j) {
            temp = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temp;
            i++;
            j--;
        }
    }
    // Recursão
    if (j > 0) quickSort(vetor, j+1); // Parte Esquerda
    if (i < (int) numero_elementos) quickSort((vetor + i), (numero_elementos - i)); // Parte Direita
}

void mergeSort(int *vetor, const uint numero_elementos){
    if (vetor == NULL) return;
    if (numero_elementos <= 1) return;
    
    // Divide o vetor
    uint tam_esquerda = (uint) (numero_elementos / 2);
    uint tam_direita = numero_elementos - tam_esquerda;
    int *esquerda = vetor;
    int *direita = (vetor + tam_esquerda);

    mergeSort(esquerda, tam_esquerda);
    mergeSort(direita, tam_direita);

    // Junta os valores
    merge(esquerda, direita, tam_esquerda, tam_direita);
}

void merge(int *esquerda, int *direita, const uint tam_esquerda, const uint tam_direita){
    if (esquerda == NULL || direita == NULL) return;
    
    uint tam_vetor_resultado = (tam_esquerda + tam_direita);
    // Cria um vetor temporário
    int *vetor_resultado = (int *) malloc(sizeof(int) * tam_vetor_resultado);
    if (vetor_resultado == NULL){
        return;
    }

    uint indice_vet1 = 0;
    uint indice_vet2 = 0;
    uint c = 0;
    while (indice_vet1 < tam_esquerda && indice_vet2 < tam_direita){
        if (esquerda[indice_vet1] < direita[indice_vet2]) {
            vetor_resultado[c++] = esquerda[indice_vet1++];
        }
        else {
            vetor_resultado[c++] = direita[indice_vet2++];
        }
    }
    
    // Neste ponto um dos vetores estão vazios
    while (indice_vet1 < tam_esquerda){
        vetor_resultado[c++] = esquerda[indice_vet1++];
    } 
    while (indice_vet2 < tam_direita){
        vetor_resultado[c++] = direita[indice_vet2++];
    }

    // Copia os valores de volta para o vetor principal

    c = 0;
    while (c < tam_esquerda){
        esquerda[c] = vetor_resultado[c];
        c++;
    }
    c = 0;
    while (c < tam_direita){
        direita[c] = vetor_resultado[c + tam_esquerda];
        c++;
    }

    free(vetor_resultado); 
    vetor_resultado = NULL;

}
void insertionSort(int *vetor, const uint numero_elementos){
    if (vetor != NULL && numero_elementos > 1){
        int dado = 0;
        int i = 0;
        for (uint c = 1; c < numero_elementos; c++) {
            dado = vetor[c];
            i = c-1;
            while ((vetor[i] > dado) && (i > 0)){
                vetor[i+1] = vetor[i];
                i--;
            }
            vetor[i] = dado;
        }
    }
}
void selectionSort(int *vetor, const uint numero_elementos){
    if (vetor != NULL && numero_elementos > 1) {
        uint ind_maior = 0;
        uint ind_menor = 0;

        int maior_valor = 0;
        int menor_valor = 0;
        for (uint c = 0; c < (numero_elementos - c - 1); c++){
            ind_menor = c;
            ind_maior = c;

            for (uint i = c+1; i < (numero_elementos - c); i++) {
                if (vetor[i] < vetor[ind_menor]) ind_menor = i;
                if (vetor[i] > vetor[ind_maior]) ind_maior = i;
            }
            
            menor_valor = vetor[ind_menor];
            maior_valor = vetor[ind_maior];
            
            if (ind_maior == c && ind_menor == numero_elementos - c - 1) {
                vetor[c] = menor_valor;
                vetor[numero_elementos - c - 1] = maior_valor;
            }
            else if (ind_maior == c){
                vetor[ind_menor] = vetor[numero_elementos - c - 1];
                vetor[numero_elementos - c -1] = maior_valor;
                vetor[c] = menor_valor;
            }
            else if (ind_menor == numero_elementos - c - 1){
                vetor[ind_maior] = vetor[c];
                vetor[c] = menor_valor;
                vetor[numero_elementos - c - 1] = maior_valor;
            }
            else { 
                vetor[ind_maior] = vetor[numero_elementos - c - 1];
                vetor[numero_elementos - c - 1] = maior_valor;
                vetor[ind_menor] = vetor[c];
                vetor[c] = menor_valor;
            }
        }
    }
}
void bubbleSort(int *vetor, const uint numero_elementos){
    if (vetor != NULL && numero_elementos > 1){
        bool modificacoes = true;
        int ordenados_no_final = 0;
        int aux;
        while (modificacoes){
            modificacoes = false;
            for (uint c = 0; c < numero_elementos - ordenados_no_final - 1; c++){
                if (vetor[c] > vetor[c + 1]) {
                    aux = vetor[c + 1];
                    vetor[c + 1] = vetor[c];
                    vetor[c] = aux;
                    modificacoes = true;
                }
            }
            ordenados_no_final++;
        }

    }
}