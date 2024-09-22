%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "y.tab.h"
%}
CARACTER_DE_PUNTUACION (( | ) | , | ;)

ASIGNACION (:=)

operadorAditivo (+ | -)

PALABRA_RESERVADA ({INICIO} | {FIN} | {LEER} | {ESCRIBIR})

DIGITO [0-9]

LETRA [a-zA-Z]

IDENTIFICADOR {LETRA}({LETRA}|{DIGITO})*

constEntera {DIGITO}({DIGITO})*

TOKEN ({IDENTIFICADOR} | {CONSTANTE} | {PALABRA_RESERVADA} | {operadorAditivo} | {ASIGNACION} | {CARACTER_DE_PUNTUACION}) 
%%
{}
{PALABRA_RESERVADA} {return XXXXXXXXX;}
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







%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "y.tab.h"
%}
CARACTER_DE_PUNTUACION ["("] | [")"] | [","] | [";"]

ASIGNACION ":="

operadorAditivo ["+"] | ["-"]

PALABRA_RESERVADA "INICIO" | "FIN" | "LEER" | "ESCRIBIR"

DIGITO [0-9]

LETRA [a-zA-Z]

IDENTIFICADOR {LETRA}({LETRA}|{DIGITO})*

constEntera {DIGITO}({DIGITO})*

TOKEN "IDENTIFICADOR" | "CONSTANTE" | "PALABRA_RESERVADA" | "operadorAditivo" | "ASIGNACION" | "CARACTER_DE_PUNTUACION" 
%%
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