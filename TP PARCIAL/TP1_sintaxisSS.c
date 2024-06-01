#include <stdio.h>
#include <string.h>

int letraRepetida(char cadena[], char letra);
int obtenerColumna(int c, char letra);

int main (){
    char caracter_e = 'a';
    
    FILE* lectura = fopen ("Lectura.txt", "r");
    FILE* escritura = fopen ("Escritura.txt","w");
    char linea_a_leer[100];
    char *palabra;
    
    while(fread(linea_a_leer, sizeof(linea_a_leer), 1, lectura) != NULL){ //Lee cada linea
        printf ("%s",linea_a_leer);
        palabra = strtok(linea_a_leer, " "); //Separa la linea por espacio
        while(palabra != NULL){ //Pasa por cada palabra
            
           if(letraRepetida(palabra, caracter_e) == 2){
                printf("repite dos veces %c:",caracter_e);
                fprintf(escritura,"%s\n", palabra);
            }
            printf("%s\n",palabra);
            
            palabra = strtok(NULL, "  \n");
        } 
    }
    fclose(lectura);
    fclose(escritura);
    getchar ();
    return 0;
}

int letraRepetida(char cadena[], char letra){
    static int tablaTransiciones [4][2]= {{1,0}, 
                                        {2,1},
                                        {3,2},
                                        {3,3}};
    int estadoActual;
    unsigned int i;
    for(estadoActual = 0, i = 0; cadena[i] != '\0' && estadoActual != 3; i++){
        estadoActual = tablaTransiciones [estadoActual][obtenerColumna(cadena[i], letra)];
    }
    return estadoActual;
}

int obtenerColumna(int c, char letra)
{
    if(c == letra) return 0;
    else return 1;
} // fin Columna /


