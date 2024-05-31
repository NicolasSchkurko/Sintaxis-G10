/*
1)	2)	Lenguajes formales y C
Escriba un programa comando que utilizando strtok y un autómata finito resuelva el siguiente problema 
“dado un archivo de texto con líneas de no más de 100 caracteres formadas por palabras separadas por blanco 
y dado además un carácter se debe crear otro archivo de texto con aquellas palabras en las que el carácter dado aparezca exactamente dos veces en la palabra. 
El archivo resultante debe tener una sola palabra por línea.

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