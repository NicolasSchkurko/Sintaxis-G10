#include <stdio.h>

int esExpresionAritmetica (char []);
int obtenerColumna (char);

int main (){
    char cadenaIngresada[] = "x";
    if(esExpresionAritmetica(cadenaIngresada) == 1)
        printf("se trata de una expresion aritmetica");
    else
        printf("no es una expresion aritmetica");
}

int esExpresionAritmetica (char cadena[]){

    static int tablaTransiciones [4][4]= {{1,3,3,3}, //0
                                        {1,2,2,3},
                                        {1,3,3,3},
                                        {3,3,3,3}};
    int estadoActual;
    unsigned int i;
    for(estadoActual = 0, i = 0; cadena[i] != '\0' && estadoActual != 3; i++){
        estadoActual = tablaTransiciones [estadoActual][obtenerColumna(cadena[i])];
    }
    return estadoActual;
}

int obtenerColumna (char caracter){
    if(caracter == '+') return 1;
    if(caracter == '-') return 2;
    if(caracter >= '0' && caracter <= '9') return 0;
    else return 3; 
}
