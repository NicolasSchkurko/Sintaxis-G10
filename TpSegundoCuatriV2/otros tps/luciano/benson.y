%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
extern char *yytext;
extern int yyleng;
extern int yylex(void);
extern void yyerror(char*);
int variable=0;
%}
%union{
  char* cadena;
  int num;
}
%token INICIO FIN LEER ESCRIBIR PARENIZQUIERDO PARENDERECHO PYCOMA COMA ASIGNACION SUMA RESTA FDT ERRORLEXICO MULTIPLICACION COCIENTE MODULO_O_RESTO
%token <cadena> ID
%token <num> CONSTANTE
%%
programa: INICIO listaDeSentencias FIN
;
listaDeSentencias: listaDeSentencias sentencia
|sentencia
;
sentencia: ID {if(yyleng>4) yyerror("semantico, debido a que supero el limite de 32 bits");} ASIGNACION expresion PYCOMA
| LEER PARENIZQUIERDO listaDeIDs PARENDERECHO PYCOMA
| ESCRIBIR PARENIZQUIERDO listaDeExpresiones PARENDERECHO PYCOMA
;
listaDeIDs: listaDeIDs COMA ID
|ID
;
listaDeExpresiones: listaDeExpresiones COMA expresion
|expresion
;
expresion: primaria
|expresion operadorAditivo primaria
|expresion operadorMultiplicativo primaria
;
primaria: ID
|CONSTANTE {printf("valores %d \n",atoi(yytext)); }
|PARENIZQUIERDO expresion PARENDERECHO
;
operadorAditivo: SUMA
|RESTA
;
operadorMultiplicativo: MULTIPLICACION
|COCIENTE
|MODULO_O_RESTO
;
%%
int main() {
yyparse();
}
void yyerror (char *s){
printf("Se ha producido un error de tipo %s\n",s);
}
int yywrap()  {
 return 1;
}