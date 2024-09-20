
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "y.tab.h"
	#include <string.h>
%}

DIGITO      [0-9]
LETRA       [a-zA-Z]
CONSTANTE   {DIGITO}({DIGITO})*
ID          {LETRA}({LETRA}|{DIGITO})*


%%
[ \t\n]             ;  // Ignorar espacios en blanco y tabulaciones
EOF         {return FDT;}
":="        {return ASIGNACION;}
";"         {return PUNTOYCOMA;}
"inicio"    {return INICIO;}
"leer"      {return LEER;}
"escribir"  {return ESCRIBIR;}
"fin"       {return FIN;}
"+"         {return SUMA;}
"-"         {return RESTA;} 
")"         {return PARENDERECHO;}
"("         {return PARENIZQUIERDO;}

{ID}        {yylval.cadena = strdup(yytext); return ID;}
{CONSTANTE} {yylval.num = atoi(yytext); return CONSTANTE; }
%%

int yywrap() {
    return 1;
}

