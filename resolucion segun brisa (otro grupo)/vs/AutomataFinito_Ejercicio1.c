#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>

bool esDigito (char caracter);
bool esOperador (char caracter);
void graficarTabla(int tabla[5][3]);
int acumular (int acumulador, char operador, int entero);
int posicionarEnColumna(char caracter);
bool describirActividad(char cadena[], int tabla[5][3]);
int calcular(char cadena[]);

int main()
{
    //Tabla de Transiciones
    int tabla[5][3] = {    //D|O|N
                    /*0*/   {1,4,4},
                    /*1*/   {4,2,4},
                    /*2*/   {3,4,4},
                    /*3*/   {4,0,4},
                    /*4*/   {4,4,4}};
    //ESTE AUTOMATA SOLO RECONOCE EXPRESIONES ARITMETICAS DE RESTA Y/O SUMA, CON NUMEROS de 0 a 9")
    printf("Ingrese una expresion aritmetica simple: ");
    char cadena[80];
    scanf("%s",&cadena);
    if(describirActividad(cadena, tabla)){
        printf("\nEl resultado de %s es: %d", cadena, calcular(cadena));
    }
        

    graficarTabla(tabla);
    return 0;
}

int calcular(char cadena[]){
    int i = 0;
    int resultado = (cadena[0]-'0');
    while(cadena[i] != '\0'){
        if(esDigito(cadena[i])){
                printf("->%d\n", resultado);
                resultado = acumular(resultado, cadena[i-1], (cadena[i]-'0'));
            }
        i++;
    }
    return resultado;
}

bool esDigito (char caracter){
    if(caracter >= '0' && caracter <= '9'){
        return true;
    }else {
        return false;
    }
}

bool esOperador (char caracter){
    if(caracter == '+' || caracter == '-'){
        return true;
    }else {
        return false;
    }
}

int acumular (int acumulador, char operador, int entero){
    if(operador == '+'){
        return acumulador += entero;
    }else if(operador == '-'){
        return acumulador -= entero;
    } else {
        return acumulador;
    }
}

void graficarTabla(int tabla[5][3]){
    //5 filas: 0 (inicial/final), 1, 2, 3 , 4(rechazo)
    //3 columnas: 0.Digito (de 0 a 9), 1.Operando (+ o -), 2.Ni digito ni operando

    //Tabla de Transiciones inicializada, no realizada
    int i=0;
    printf("\nTabla de transiciones: \n");
    printf("  | D | O | N\n");
    while(i<5){
        printf("%d |%d  |%d  |%d\n", i, tabla[i][0],tabla[i][1],tabla[i][2]);
        i++;
    }
}

int posicionarEnColumna(char caracter){
    if(esDigito(caracter)){
        return 0;
    } else if (esOperador(caracter)){
        return 1;
    } else {
        return 2;
    }
}

bool describirActividad (char cadena[], int tabla[5][3]){
    printf("\nDescripcion de la ACTIVIDAD: \n");
    int i = 0, estado = 0, columna;
    int estados[80];

    columna = posicionarEnColumna(cadena[i]);
        estados[i] = 0;
        estado = tabla[estado][columna];
        printf("%d -> '%c' -> %d", estados[i], cadena[i], estado);
        i++;
    while(cadena[i] != '\0' && estado != 4){
        columna = posicionarEnColumna(cadena[i]);
        estados[i] = estado;
        estado = tabla[estado][columna];
        printf(" -> '%c' -> %d", cadena[i], estado);
        i++;
        
    }

    if(estado == 4){
        printf(" RECHAZA\nNo es palabra.\n");
        return false;
    } else {
        printf(" RECONOCE\nEs es palabra.\n");
        return true;
    }
}