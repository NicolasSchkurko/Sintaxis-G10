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
extern int yylexerrs;
extern FILE* yyin;

struct Identificador{
   char* nombre;
   int valor;
}; 

struct Identificador listaIds[80];

int cantIdentificadores = 0;
int lineaActual = 0;
int erroresLexicos = 0;
int erroresSintacticos = 0;
int errorID = 0;
int errorTotal= 0;

int erroreslol = 0;

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

%token ASIGNACION PUNTOYCOMA RESTA SUMA PARENIZQUIERDO PARENDERECHO INICIO FIN LEER ESCRIBIR COMA MULTIPLICACION DIVISION
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
    // Salta al siguiente punto y coma
        yyclearin;
        yyerrok;
}
;

instruccion: ID ASIGNACION expresion {
    char* nombre = $<cadena>1;
    int valor = $<num>3;
    asignarIds(nombre, valor);
    printf("declara %s, entera\n", nombre);
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

expresion: termino
| expresion SUMA termino 
{$<num>$ = $<num>1 + $<num>3;}
| expresion RESTA termino
{$<num>$ = $<num>1 - $<num>3;}
;

termino: termino MULTIPLICACION primaria
{$<num>$ = $<num>1 * $<num>3;}
|termino DIVISION primaria
{   
    if($<num>3!=0) $<num>$ = $<num>1 / $<num>3;
    else 
        {
            char mensajeDeError[100];
            sprintf(mensajeDeError, "Es imposible dividir por 0");
            yyerror(mensajeDeError);
            errorID++;
        }
}
| primaria
;

primaria: ID 
{
    char* nombre = $<cadena>1;
    int i;
    for (i = 0; i < cantIdentificadores; i++) {
        if (strcmp(listaIds[i].nombre, nombre) == 0) {
            $<num>$ = listaIds[i].valor;
            break;
        }
    }
    if (i == cantIdentificadores) {
	char mensajeDeError[100];
        sprintf(mensajeDeError, "La variable %s no ha sido definida con ningun valor", nombre);
	    yyerror(mensajeDeError);
        errorID++;
    }
}
| CONSTANTE 
| PARENIZQUIERDO expresion PARENDERECHO
{ $<num>$ = $<num>2; } 
;


%%

int main(int argc, char** argv) {
    // Validación de argumentos
    if (argc == 1) {
        printf("Debe ingresar el nombre del archivo fuente (en lenguaje Micro) en la linea de comandos\n");
        return -1;
    } else if (argc != 2) {
        printf("Numero incorrecto de argumentos\n");
        return -1;
    }

    // Carga del nombre del archivo
    char filename[50];
    sprintf(filename, "%s", argv[1]);
    int longitudArchivo = strlen(filename);

    // Verificación de la extensión del archivo
    if (argv[1][longitudArchivo - 1] != 'm' || argv[1][longitudArchivo - 2] != '.') {
        printf("Extension incorrecta (debe ser .m)\n");
        return EXIT_FAILURE;
    }

    // Apertura del archivo
    yyin = fopen(filename, "r");
    if (yyin == NULL) {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    // Análisis sintáctico
    switch (yyparse()) {
        case 0: estadoActual = CORRECTO; break;
        case 1: estadoActual = ERROR; break;
        case 2: estadoActual = ERROR_MEMORIA; break;
    }

    // Verificación de errores
    if ((errorTotal || yylexerrs || errorID) && estadoActual != ERROR_MEMORIA) 
        estadoActual = ERROR;

    // Mensajes según el estado de compilación
    switch (estadoActual) {
        case CORRECTO: 
            printf("\nProceso de compilacion termino exitosamente, codigo correcto sintacticamente\n"); 
            break;
        case ERROR: 
            printf("\nErrores en la compilacion\n"); 
            break;
        case ERROR_MEMORIA: 
            printf("\nNo hay memoria suficiente\n"); 
            break;
    }

    // Cálculo de errores
    erroresSintacticos = errorTotal - erroresLexicos - errorID;
    printf("\nErrores sintacticos: %i\tErrores lexicos: %i\tErrores totales: %i\n", erroresSintacticos, erroresLexicos, errorTotal);

    // Cierre del archivo
    fclose(yyin);
    return 0;
}

// Manejo de errores
void yyerror(char *s) {
    if (lineaActual != yylineno) {
        lineaActual = yylineno;
        fprintf(stderr, "ERROR: %s en la linea %d\n", s, yylineno);
        erroresLexicos = yylexerrs;
        errorTotal++;
    } else if (erroresLexicos != yylexerrs) {
        fprintf(stderr, "ERROR: %s en la linea %d\n", s, yylineno);
        erroresLexicos = yylexerrs;
        errorTotal++;
    }
}

// Asignación de identificadores
void asignarIds(char* nombre, int valor) {
    int i;
    // Busca si el identificador ya existe
    for (i = 0; i < cantIdentificadores; i++) {
        if (strcmp(listaIds[i].nombre, nombre) == 0) {
            listaIds[i].valor = valor;
            break;
        }
    }
    
    // Si no existe, agrega uno nuevo
    if (i == cantIdentificadores) {
        listaIds[cantIdentificadores].nombre = nombre; 
        listaIds[cantIdentificadores].valor = valor;
        cantIdentificadores++;
    }
}
