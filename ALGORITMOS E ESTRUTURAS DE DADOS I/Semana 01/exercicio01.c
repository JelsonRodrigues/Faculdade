#include <stdio.h>

int main(void){
    const char mensagem[] = "Ola Mundo!\n";
    if (printf("%s", mensagem)) {
        return 0;
    } 
    else {
        return -1;
    } 
}