%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *yytext;
extern int yyleng;
extern int yylex(void);
extern void yyerror(char*);

extern int yylineno;
extern int yynerrs;
extern int yylexerrs;
extern FILE* yyin;
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

sentencia: identificador ASIGNACION expresion PUNTOYCOMA 
| LEER PARENIZQUIERDO listaIds PARENDERECHO PUNTOYCOMA 
| ESCRIBIR PARENIZQUIERDO listaExpresiones PARENDERECHO PUNTOYCOMA
;

listaIds: listaIds COMA identificador 
| identificador
;

listaExpresiones: listaExpresiones COMA expresion
| expresion
;

expresion: primaria 
| expresion operadorAditivo primaria
;

operadorAditivo: SUMA
| RESTA
;

primaria: identificador 
| CONSTANTE 
| PARENIZQUIERDO expresion PARENDERECHO
;

identificador: ID {
    if (yyleng > 32) {
        printf("Error lexico en la línea %d: se excedió la longitud máxima de 32 caracteres para un identificador -> '%s'\n", yylineno, yytext);
        yylexerrs++;
    }
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
        case 0: printf("\nProceso de compilación termino exitosamente\n"); break;
        case 1: printf("\nErrores en la compilacion\n"); break;
        case 2: printf("\nNo hay memoria suficiente\n"); break;
    }

    printf("\nErrores sintacticos: %i\tErrores lexicos: %i\n", yynerrs, yylexerrs);
    fclose(yyin);
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "\nError sintactico: %s en la linea %d\n", s, yylineno);
    if (yytext) {
        fprintf(stderr, "                  -> Provocado por el token: %s\n", yytext);
    }
}
