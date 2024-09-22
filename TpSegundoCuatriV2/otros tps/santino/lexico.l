%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

void yyerror(char *s);
int yylex();
int yylineno;
int yylexerrs = 0;
%}
digito [0-9]

letra [a-zA-Z]

identificador {letra}({letra}|{digito})*

constEntera {digito}({digito})*
%%
":=" {return ASIGNACION;}
{constEntera} {yylval.num=atoi(yytext);return CONSTANTE;}
"inicio" {return INICIO;}
"fin" {return FIN;}
"leer" {return LEER;}
"escribir" {return ESCRIBIR;}
{identificador} {yylval.cadena = strdup(yytext); return ID;}
"," {return COMA;}
";" {return PYCOMA;}
"(" {return PARENIZQUIERDO;}
")" {return PARENDERECHO;}
"+" {return SUMA;}
"-" {return RESTA;}
\n  { yylineno++; }
[ \t\n] {/*ignora espacios en blanco*/}
. {yylexerrs++; printf("Error léxico: %s es un carácter inválido\n", yytext); return 0;}
%%

int yywrap(void)
{
   return 1;
}
