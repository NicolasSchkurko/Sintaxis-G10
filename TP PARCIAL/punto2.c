#include <stdio.h>
#include <string.h>

// Función que determina si la letra aparece exactamente dos veces en la cadena
int letraRepetida(char cadena[], char letra);

// Función que obtiene la columna para la tabla de transiciones del autómata finito
int obtenerColumna(int c, char letra);

int main() {
    char caracter_e = 'a'; // Carácter a buscar
    
    FILE* lectura = fopen("Lectura.txt", "r");
    FILE* escritura = fopen("Escritura.txt", "w");
    char linea_a_leer[100];
    char *palabra;
    
    if (lectura == NULL) {
        printf("Error al abrir el archivo de lectura.\n");
        return 1;
    }

    if (escritura == NULL) {
        printf("Error al abrir el archivo de escritura.\n");
        fclose(lectura);
        return 1;
    }
    
    while (fgets(linea_a_leer, sizeof(linea_a_leer), lectura) != NULL) { // Lee cada línea
        palabra = strtok(linea_a_leer, " \n"); // Separa la línea por espacios y nuevas líneas
        while (palabra != NULL) { // Pasa por cada palabra
            printf("%s\n", palabra); // Imprime la palabra leída
            if (letraRepetida(palabra, caracter_e) == 2) {
                printf("cumple: %s\n", palabra); // Imprime que la palabra cumple la condición
                fprintf(escritura, "%s\n", palabra); // Escribe la palabra en el archivo de salida
            }
            palabra = strtok(NULL, " \n");
        }
    }
    
    fclose(lectura);
    fclose(escritura);
    return 0;
}

int letraRepetida(char cadena[], char letra) {
    static int tablaTransiciones[4][2] = {
        {1, 0}, 
        {2, 1},
        {3, 2},
        {3, 3}
    };
    int estadoActual;
    unsigned int i;
    for (estadoActual = 0, i = 0; cadena[i] != '\0' && estadoActual != 3; i++) {
        estadoActual = tablaTransiciones[estadoActual][obtenerColumna(cadena[i], letra)];
    }
    return estadoActual;
}

int obtenerColumna(int c, char letra) {
    if (c == letra) return 0;
    else return 1;
}
