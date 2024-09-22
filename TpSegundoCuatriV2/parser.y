%{
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
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
%union{
   char* cadena;
   int num;
} 

%token ASIGNACION PUNTOYCOMA RESTA SUMA PARENIZQUIERDO PARENDERECHO INICIO FIN LEER ESCRIBIR COMA FDT
%token <cadena> ID
%token <num> CONSTANTE

%%

objetivo: programa FDT

programa: INICIO listaSentencias FIN

listaSentencias: listaSentencias sentencia 
|sentencia

sentencia: ID {if(yyleng>32){ printf("Error lexico: se excedio la longitud maxima para un identificador"); yylexerrs++;}} ASIGNACION expresion PUNTOYCOMA 
|LEER PARENIZQUIERDO listaIds PARENDERECHO PUNTOYCOMA 
|ESCRIBIR PARENIZQUIERDO listaExpresiones PARENDERECHO PUNTOYCOMA
;

listaIds: listaIds COMA ID  // es recursivo por izquierda
| ID
;

listaExpresiones: listaExpresiones COMA expresion
|expresion
;

expresion: primaria 
| expresion operadorAditivo primaria
;

operadorAditivo: SUMA
|RESTA
;

primaria: ID 
|CONSTANTE 
|PARENIZQUIERDO expresion PARENDERECHO
;


%%

int main(int argc, char** argv) 
{

   if ( argc == 1 )
   {
      printf("Debe ingresar el nombre del archivo fuente (en lenguaje Micro) en la linea de comandos\n");
      return -1;
   }
   else if ( argc != 2 )
   {
      printf("Numero incorrecto de argumentos\n");
      return -1;
   }
   char filename[50];
   sprintf(filename, "%s", argv[1]);
   int largo = strlen(filename);

   if (argv[1][largo-1] != 'm' || argv[1][largo-2] != '.') {
      printf("Extension incorrecta (debe ser .m)");
      return EXIT_FAILURE;
   }

   yyin = fopen(filename, "r");
   if (yyin == NULL) {
      perror("Error al abrir el archivo");
      return EXIT_FAILURE;
   }
   
   switch (yyparse()){
      case 0: printf("\nProceso de compilacion termino exitosamente");
      break;
      case 1: printf("\nErrores en la compilacion");
      break;
      case 2: printf("\nNo hay memoria suficiente");
      break;
   }
   printf("\nErrores sintacticos: %i\tErrores lexicos: %i\n", yynerrs, yylexerrs);
   fclose(yyin);
   return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "\nError sintáctico: %s en la línea %d\n", s, yylineno);
      if (yytext) {
        fprintf(stderr, "                  -> Provocado por el token: %s\n", yytext);
    }
}