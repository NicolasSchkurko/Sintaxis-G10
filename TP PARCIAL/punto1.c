/*
1)	Parte de C
Escriba un programa que determine e informe la cantidad de lines que tiene un archivo de texto, 
que se pasa como argumento desde la linea de comandos. 
Debe resolverlo utilizando fgetc 
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int ch; // es int pq el tipo char no es capaz de representar EOF (-1)
    int cant_lineas = 0;

    // Lee desde la entrada estándar y cuenta las líneas
    while ((ch = fgetc(stdin)) != EOF) {
        if (ch == '\n') {
            cant_lineas++;
        }
    }
    cant_lineas++; // Asi cuenta la ultima linea (la de EOF)
    // Informa la cantidad de líneas
    printf("El archivo tiene %d lineas.\n", cant_lineas);

    return 0;
}