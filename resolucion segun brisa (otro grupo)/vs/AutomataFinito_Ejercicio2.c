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

//DETERMINA SI UN ELEMENTO ES UN DIGITO DE 0 A 9
bool esDigito (char caracter){
    if(caracter >= '0' && caracter <= '9'){
        return true;
    }else {
        return false;
    }
}

//DETERMINA SI ES UN OPERADOR -> + - x /
bool esOperador (char caracter){
    if(caracter == '+' || caracter == '-' || caracter == 'x' || caracter == '/'){
        return true;
    }else {
        return false;
    }
}

//ACUMULA LOS DIGITOS
int acumular (int acumulador, char operador, int entero){
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
    while(cadena[i] != '\0'){
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

//GRAFICA LA TABLA DE TRANSICIONES
void graficarTabla(int tabla[10][5]){
    //10 filas: 0 (inicial), 1(final), 2, 3 (final), 4, 5, 6, 7, 8 (final), 9(rechazo)
    //5 columnas: 0.Digito (de 0 a 9), 1.Operando (+,-, x, /), 2.Abierto el parentesis , 3.Cerrado el parentesis, 4.Ninguno

    //Tabla de Transiciones inicializada, no realizada
    int i=0;
    printf("\nTabla de transiciones: \n");
    printf("  | D | O | A | C | N\n");
    while(i<10){
        printf("%d |%d  |%d  |%d  |%d  |%d\n", i, tabla[i][0],tabla[i][1],tabla[i][2],tabla[i][3],tabla[i][4]);
        i++;
    }
}

//PERMITE IDENTIFICAR EN QUE COLUMNA ESTA POSICIONADO
int posicionarEnColumna(char caracter){
    if(esDigito(caracter)){
        return 0;
    } else if (esOperador(caracter)){
        return 1;
    } else if (caracter == '('){
        return 2;
    } else if (caracter == ')'){
        return 3;
    } else {
        return 4;
    }
}

//DETERMINA SI ES O NO PALABRA
bool describirActividad (char cadena[], int tabla[10][5]){
    printf("\nDescripcion de la ACTIVIDAD: \n");
    int i = 0, estado = 0, columna;
    int estados[80];

    columna = posicionarEnColumna(cadena[i]);
        estados[i] = 0;
        estado = tabla[estado][columna];
        printf("%d -> '%c' -> %d", estados[i], cadena[i], estado);
        i++;
    while(cadena[i] != '\0' && estado != 9){
        columna = posicionarEnColumna(cadena[i]);
        estados[i] = estado;
        estado = tabla[estado][columna];
        printf(" -> '%c' -> %d", cadena[i], estado);
        i++;
        
    }

    if(estado == 9){
        printf(" RECHAZA\nNo es palabra.\n");
        return false;
    } else {
        printf(" RECONOCE\nEs es palabra.\n");
        return true;
    }
}

//MAIN
int main()
{
    //Tabla de Transiciones
    int tabla[10][5] = {   //D|O|A|C|N
                    /*0*/   {1,9,4,9,9},
                    /*1*/   {9,2,9,9,9},
                    /*2*/   {3,9,4,9,9},
                    /*3*/   {9,2,9,9,9},
                    /*4*/   {5,9,9,9,9},
                    /*5*/   {9,6,9,8,9},
                    /*6*/   {7,9,9,9,9},
                    /*7*/   {9,6,9,8,9},
                    /*8*/   {9,2,9,9,9},
                    /*9*/   {9,9,9,9,9}};

    //ESTE AUTOMATA SOLO RECONOCE EXPRESIONES ARITMETICAS DE +, -, X, /, CON NUMEROS DE UN DIGITO, APERTURA DE UN SOLO PARENTESIS NO (())")
    printf("Ingrese una expresion aritmetica simple: ");
    char operacion[80];
    scanf("%s",&operacion);

    if(describirActividad(operacion, tabla)){
        //DECLARACION DE VARIABLES
        struct Nodo *pila = NULL; //Divido la pila
        struct Nodo *pila2 = NULL;

    //AGREGAR OPERACIONES A LA PILA
        int i=0, j = 0, cambio = 0,aux;
        bool abierto;
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
        free(array);

    //DA VUELTA LA PILA
        char operacion2[100], s;
        int r;
        while(pila != NULL){
            s = pila->siguiente_signo;
            r = pila->resultado;
            char d[100];
            strcpy(d,pila->operacion);
            push2(&pila2, r, s);
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
            char operador = pila->siguiente_signo;
            pila = pila->siguiente;
        }
    
        printf("\nResultado de la operacion: %d\n", resultado);

    //LIBERA LA MEMORIA ASIGNADA
        while (pila != NULL) {
            struct Nodo *temp = pila;
            pila = pila->siguiente;
            free(temp);
        }
        }
        
    graficarTabla(tabla);
    return 0;
}
