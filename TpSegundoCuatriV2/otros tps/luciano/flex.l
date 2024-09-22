%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "y.tab.h"
%}
INICIO "INICIO"|"inicio"

FIN "FIN"|"fin"

LEER "LEER"|"leer"

ESCRIBIR "ESCRIBIR"|"escribir"

DIGITO [0-9]

LETRA [a-zA-Z]

IDENTIFICADOR {LETRA}({LETRA}|{DIGITO})*

constEntera {DIGITO}({DIGITO})*
%%
{INICIO} {return INICIO;}
{LEER} {return LEER;}
{ESCRIBIR} {return ESCRIBIR;}
{FIN} {return FIN;}
":=" {return ASIGNACION;}
{constEntera} {yylval.num=atoi (yytext);return CONSTANTE;}
{IDENTIFICADOR} {return ID;}
";" {return PYCOMA;}
"," {return COMA;}
"(" {return PARENIZQUIERDO;}
")" {return PARENDERECHO;}
"+" {return SUMA;}
"-" {return RESTA;}
"*" {return MULTIPLICACION;}
"/" {return COCIENTE;}
"%" {return MODULO_O_RESTO;}
%%