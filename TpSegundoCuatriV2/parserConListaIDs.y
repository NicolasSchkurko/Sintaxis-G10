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


int lineaActual = 0;
int erroresSintacticos = 0;
int erroresSemanticos = 0;

typedef enum {
    CORRECTO,
    ERROR,
    ERROR_MEMORIA
} Estado;

Estado estadoActual = CORRECTO;
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
programa: INICIO listaSentencias FIN
;

listaSentencias: listaSentencias sentencia 
| sentencia
;


sentencia: instruccion PUNTOYCOMA
| error{
    //  Nota de nico: Aparentemente si este error lo vuelo a la mierda el compi no lee el codigo completo (solo si hay error un de sentencia sin punto y coma)
    yyerror("Falta punto y coma, saltando al siguiente punto y coma...");
        yyclearin;
        yyerrok;
}
;

instruccion: ID ASIGNACION expresion {
    char* nombre = $<cadena>1;
    int valor = $<num>3;
    asignarIds(nombre, valor);
}
| LEER PARENIZQUIERDO listaIds PARENDERECHO {
    printf("Lee %s\n", $<cadena>3);
}
| ESCRIBIR PARENIZQUIERDO listaExpresiones PARENDERECHO{
    printf("Escribe %d\n", $<num>3);
}
;


listaIds: listaIds COMA ID
{
	strcat($<cadena>1, ",");
	strcat($<cadena>1, $<cadena>3);
}
| ID
;

listaExpresiones: listaExpresiones COMA expresion
| expresion
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
        sprintf(mensajeDeError, "Error semantico (A REVISAR): La variable %s no ha sido definida con ningun valor", nombre);
	    yyerror(mensajeDeError);
        erroresSemanticos++;
    }
}
| CONSTANTE 
| PARENIZQUIERDO expresion PARENDERECHO
{ $<num>$ = $<num>2; }
| error {
    // Salta al siguiente punto y coma
    yyerror("Error en la sentencia, saltando al siguiente punto y coma...");
        yyclearin;
        yyerrok;
}
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
        case 0: estadoActual = CORRECTO; break;
        case 1: estadoActual = ERROR; break;
        case 2: estadoActual = ERROR_MEMORIA; break;
    }

    if (erroresSintacticos || yylexerrs || erroresSemanticos) estadoActual = ERROR;

    switch (estadoActual) {
        case CORRECTO: printf("\nProceso de compilacion termino exitosamente, codigo correcto sintacticamente\n"); break;
        case ERROR: printf( "\x1b[31m" "\nErrores en la compilacion\n" "\x1b[0m"); break;
        case ERROR_MEMORIA: printf("\nNo hay memoria suficiente\n"); break;
    }
    erroresSintacticos = erroresSintacticos - yylexerrs - erroresSemanticos;
    printf("\nErrores sintacticos: %i\tErrores lexicos: %i\tErrores semanticos: %i\n", erroresSintacticos, yylexerrs, erroresSemanticos);
    fclose(yyin);
    return 0;
}

void yyerror(char *s) {
    if(lineaActual != yylineno){
        lineaActual = yylineno;
        fprintf(stderr, "\x1b[31m" "ERROR %s en la linea %d\n"  "\x1b[0m", s, yylineno);
        erroresSintacticos++;
    }
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