
%{
#include <stdio.h>
#include <stdlib.h>
#ifndef yywrap
static int yywrap(void) { return 1;}
#include "./tokens.h"
#include <string.h>
#endif

%}
DIGITO      [0-9]
LETRA       [a-zA-Z]
CONSTANTE   {DIGITO}({DIGITO})*
ID          {LETRA}({LETRA}|{DIGITO})*

%%

":="        {return ASIGNACION;}
";"         {return PUNTOYCOMA;}
"inicio"    {return INICIO;}
"leer"      {return LEER;}
"escribir"  {return ESCRIBIR;}
"fin"       {return FIN;}
"+"         {return SUMA;}
"-"         {return RESTA;} 

{ID}    printf( "Un identificador: %s de longitud %d\n", yytext, yyleng);
// hay que ver si aca se imprime todos los posibles tokens o lo hacemos en el codigo de c
%%
int main() {
yylex();
}

