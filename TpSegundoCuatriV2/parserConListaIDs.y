%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *yytext;
extern int yyleng;
extern int yylex(void);
extern void yyerror(char*);
void asignarIds(char* nombre, int valor);

extern int yylineno;
extern int yynerrs;
extern int yylexerrs;
extern FILE* yyin;

struct Identificador{
   char* nombre;
   int valor;
}; 
struct Identificador listaIdentificadores[80];
int cantidadIdentificadores = 0;
%}


%union {
   char* cadena;
   int num;
}

%token ASIGNACION PUNTOYCOMA RESTA SUMA PARENIZQUIERDO PARENDERECHO INICIO FIN LEER ESCRIBIR COMA FDT
%token <cadena> ID
%token <num> CONSTANTE

%%  
// Programa principal: arranca con INICIO y termina con FIN
programa: INICIO listaSentencias FIN {
    if (yynerrs || yylexerrs) {
        YYABORT;
    }
}
;

listaSentencias: listaSentencias sentencia 
| sentencia
;

sentencia: ID ASIGNACION expresion PUNTOYCOMA {
    char* nombre = $<cadena>1;
    int valor = $<num>3;
    asignarIds(nombre, valor);
}
| LEER PARENIZQUIERDO listaIds PARENDERECHO PUNTOYCOMA 
| ESCRIBIR PARENIZQUIERDO listaExpresiones PARENDERECHO PUNTOYCOMA
;

listaIds: listaIds COMA ID
{
	strcat($<cadena>1, ",");
	strcat($<cadena>1, $<cadena>3);
}
| ID
;

listaExpresiones: listaExpresiones COMA expresion
{
	printf("%d\n", $<num>3);
}
| expresion
{
	printf("%d\n", $<num>$);
}

;

expresion: primaria 
| expresion SUMA primaria 
{$<num>$ = $<num>1 + $<num>3;}
| expresion RESTA primaria
{$<num>$ = $<num>1 - $<num>3;}
;

primaria: ID 
{
    char* nombre = $<cadena>1;
    int i;
    for (i = 0; i < cantidadIdentificadores; i++) {
        if (strcmp(listaIdentificadores[i].nombre, nombre) == 0) {
            $<num>$ = listaIdentificadores[i].valor;
            break;
        }
    }
    if (i == cantidadIdentificadores) {
	char mensajeDeError[100];
        sprintf(mensajeDeError, "ERROR: La variable %s no ha sido definida con ningun valor \n", nombre);
	yyerror(mensajeDeError);
    }
}
| CONSTANTE 
| PARENIZQUIERDO expresion PARENDERECHO
{ $<num>$ = $<num>2; }
;

%%

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Debe ingresar el nombre del archivo fuente (en lenguaje Micro) en la linea de comandos\n");
        return -1;
    } else if (argc != 2) {
        printf("Numero incorrecto de argumentos\n");
        return -1;
    }
    
    char filename[50];
    sprintf(filename, "%s", argv[1]);
    int largo = strlen(filename);

    if (argv[1][largo - 1] != 'm' || argv[1][largo - 2] != '.') {
        printf("Extension incorrecta (debe ser .m)\n");
        return EXIT_FAILURE;
    }

    yyin = fopen(filename, "r");
    if (yyin == NULL) {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    switch (yyparse()) {
        case 0: printf("\nProceso de compilacion termino exitosamente, codigo correcto sintacticamente\n"); break;
        case 1: printf("\nErrores en la compilacion\n"); break;
        case 2: printf("\nNo hay memoria suficiente\n"); break;
    }

    printf("\nErrores sintacticos: %i\tErrores lexicos: %i\n", yynerrs, yylexerrs);
    fclose(yyin);
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "%s en la linea %d\n", s, yylineno);
}

void asignarIds(char* nombre, int valor) {
    int i;
    for (i = 0; i < cantidadIdentificadores; i++) {
        if (strcmp(listaIdentificadores[i].nombre, nombre) == 0) {
            listaIdentificadores[i].valor = valor;
            break;
        }
    }
    
    if (i == cantidadIdentificadores) {
        listaIdentificadores[cantidadIdentificadores].nombre = nombre; 
        listaIdentificadores[cantidadIdentificadores].valor = valor;
        cantidadIdentificadores++;
    }
}