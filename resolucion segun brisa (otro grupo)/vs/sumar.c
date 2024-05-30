#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//DEFINO LOS DATOS DE LA PILA
struct Nodo {
    char operacion[50];
    char siguiente_signo;
    int resultado;
    struct Nodo *siguiente;
};

//INDICA SI EL CARACTER ES DIGITO -> 0 A 9
bool esDigito (char caracter){
    if(caracter >= '0' && caracter <= '9'){
        return true;
    }else {
        return false;
    }
}

//INDICA SI EL CARACTER ES OPERANDO -> '+' '-' 'x' '/'
bool esOperador (char caracter){
    if(caracter == 'x' || caracter == '/'|| caracter == '+'|| caracter == '-'){
        return true;
    }else {
        return false;
    }
}

//ACUMULA LOS DIGITOS
int acumular (int acumulador, char operador, int entero){
    printf("\nACUMULAR:\n");
    printf("Acum: %d| Op: %c| Ent: %d",acumulador,operador,entero);
    if(operador == 'x'){
        return acumulador *= entero;
    }else if(operador == '/'){
        return acumulador = acumulador/entero;
    } else if(operador == '+'){
        return acumulador += entero;
    } else if(operador == '-'){
        return acumulador -= entero;
    } else {
        return acumulador;
    }
}

//CALCULA LA OPERACION DE LA CADENA
int calcular(char cadena[]){
    int i = 0;
    int resultado = (cadena[0]-'0'), resultado2;
    char signo;
    bool abierto;
    printf("\nCALCULAR\n");
    while(cadena[i] != '\0'){
        printf("\ni: %d| c: %c\n", i, cadena[i]);
        if(esDigito(cadena[i])){
            if(esOperador(cadena[i-1])){
                resultado = acumular(resultado, cadena[i-1], (cadena[i]-'0'));
            } else {
                resultado = (cadena[i]-'0');
            }
        }
        if(cadena[i]=='('){
            if(esOperador(cadena[i-1])){
                abierto = true;
                signo = cadena[i-1];
                resultado2 = resultado;
            }
        }
        if(cadena[i]==')'&&abierto){
            resultado = acumular(resultado, signo, resultado2);
            abierto = false;
        }
        i++;

    }
    return resultado;
}

//AGREGAR ELEMENTOS A LA PILA
void push(struct Nodo **pila, char operacion[], char siguiente_signo) {
    struct Nodo *nuevoNodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    strcpy(nuevoNodo->operacion, operacion);
    nuevoNodo->siguiente_signo=siguiente_signo;
    nuevoNodo->resultado = calcular(operacion);
    printf("\nResultado almacenado: %d\n", nuevoNodo->resultado);
    nuevoNodo->siguiente = *pila;
    *pila = nuevoNodo;
}

//DAR VUELTA LA PILA
void push2(struct Nodo **pila, int resultado, char siguiente_signo) {
    struct Nodo *nuevoNodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuevoNodo->siguiente_signo=siguiente_signo;
    nuevoNodo->resultado = resultado; 
    nuevoNodo->siguiente = *pila;
    *pila = nuevoNodo;
}

int main() {
//DECLARACION DE VARIABLES
    struct Nodo *pila = NULL; //Divido la pila
    struct Nodo *pila2 = NULL;
    char operacion[50];

//INGRESO DE LA OPERACION
    printf("ingrese la operacion: ");
    scanf("%s", &operacion);

//AGREGAR OPERACIONES A LA PILA
    int i=0, j = 0, cambio = 0,aux;
    bool abierto;
    printf("GUARDANDO EN LA PILA");
    while(operacion[i] != '\0'){
        if(operacion[i]=='('){
            abierto = true;
        }
        if(operacion[i]==')'){
            abierto = false;
        }
        if(operacion[i]!='+' && operacion[i]!='-'){
            j++;
        } else {
            if(abierto){
                j++;
            } else {
               char *array = (char *)malloc(j * sizeof(char));
                cambio=i;
                aux=0;
                for(int x = cambio-j; x<cambio; x++){
                    array[aux] = operacion[x];
                    aux++;
                }
                array[aux] = '\0';
                push(&pila, array, operacion[i]);
                printf("\nOperacion almacenada: %s\n", array);
                j=0;
                free(array);
            } 
        }
        i++;
    }
    char *array = (char *)malloc(j * sizeof(char));
    cambio=i;
    aux=0;
    for(int x = cambio-j; x<cambio; x++){
        array[aux] = operacion[x];
        aux++;
    }
    array[aux] = '\0';
    push(&pila, array, '=');
    printf("\nOperacion almacenada: %s\n", array);
    free(array);

//DA VUELTA LA PILA
    char operacion2[100], s;
    int r;
    while(pila != NULL){
        char d[100];
        strcpy(d,pila->operacion);
        push2(&pila2, pila->resultado, pila->siguiente_signo);
        pila = pila->siguiente;
    }
    pila = pila2;
    free(pila2);
//CALCULAR VALOR    
    int resultado=pila->resultado;
    char operador = pila->siguiente_signo;
    pila = pila->siguiente;
    while(pila != NULL){
        resultado = acumular(resultado, operador, pila->resultado);
        printf("\nResultado de la operacion por ahora: %d\n", resultado);
        char operador = pila->siguiente_signo;
        pila = pila->siguiente;
    }
    
    printf("\nResultado de la operacion: %d", resultado);

//LIBERA LA MEMORIA ASIGNADA
    while (pila != NULL) {
        struct Nodo *temp = pila;
        pila = pila->siguiente;
        free(temp);
    }

    return 0;
}