%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);

typedef struct {
    char nombre[32];
    int valor;
} Identificador;

Identificador identificadores[100];
int cantidadIdentificadores = 0;

int buscarIdentificador(const char* nombre);
void agregarIdentificador(const char* nombre, int valor);

%}

%tipos{
    int num;
    char* cadena;
}

%token <cadena> ID
%token <num> CONSTANTE
%token ASIGNACION
%token LEER ESCRIBIR
%token SUMA RESTA
%token PUNTOCOMA 
%token INICIO FIN
%token EOF

%type <num> expresion primaria //tipo de los no terminales

%%

programa:
    INICIO sentencias FIN EOF { printf("Análisis sintáctico completo\n"); }
;

sentencias:
    sentencias sentencia //recursivo por izquierda
    | /* vacío */
;

sentencia:
    LEER '(' lista_identificadores ')' PUNTOCOMA { //el $3 hace referencia al tercer elemento de la produccion de la regla (lista_identificadores)
        printf("Leer variables: %s\n", $3); 
    }
    | ESCRIBIR '(' lista_expresiones ')' PUNTOCOMA {
        printf("Escribir valores: %s\n", $3);
    }
    | ID ASIGNACION expresion PUNTOCOMA {
        int idx = buscarIdentificador($1);
        if (idx == -1) {
            agregarIdentificador($1, $3);
        } else {
            identificadores[idx].valor = $3;
        }
        printf("Asignación: %s := %d\n", $1, $3);
    }
;

lista_identificadores:
    lista_identificadores COMA ID { $$ = strcat($1, ", "); strcat($$, $3); } 
    | ID { $$ = $1; } // si hay un id solo en la lista
;

lista_expresiones:
    lista_expresiones COMA expresion { $$ = strcat($1, ", "); strcat($$, "valor"); }
    | expresion { $$ = "valor"; }
;

expresion:
    expresion MAS primaria { $$ = $1 + $3; }
    | expresion MENOS primaria { $$ = $1 - $3; }
    | expresion MULT primaria { $$ = $1 * $3; }
    | expresion DIV primaria { $$ = $1 / $3; }
    | primaria { $$ = $1; }
;

primaria:
    NUM { $$ = $1; }
    | ID {
        int idx = buscarIdentificador($1);
        if (idx != -1) {
            $$ = identificadores[idx].valor;
        } else {
            char mensaje[100];
            sprintf(mensaje, "La variable %s no ha sido definida.\n", $1);
            yyerror(mensaje);
        }
    }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}

int buscarIdentificador(const char* nombre) {
    for (int i = 0; i < cantidadIdentificadores; i++) {
        if (strcmp(identificadores[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

void agregarIdentificador(const char* nombre, int valor) {
    strcpy(identificadores[cantidadIdentificadores].nombre, nombre);
    identificadores[cantidadIdentificadores].valor = valor;
    cantidadIdentificadores++;
}

int main() {
    return yyparse();
}
