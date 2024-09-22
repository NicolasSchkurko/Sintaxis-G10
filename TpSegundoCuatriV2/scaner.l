
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "y.tab.h"
	#include <string.h>

	void yyerror(char *s);
	int yylex();
	int yylineno;
	int yylexerrs = 0;
%}

DIGITO      	[0-9]
LETRA       	[a-zA-Z]
CONST   	{DIGITO}({DIGITO})*
IDENTIFICADOR {LETRA}({LETRA}|{DIGITO})* 
ID_ERROR      {DIGITO}({DIGITO}|{LETRA})*


%%
":="        {return ASIGNACION;}
";"         {return PUNTOYCOMA;}
","			{return COMA;}
"inicio"    {return INICIO;}
"leer"      {return LEER;}
"escribir"  {return ESCRIBIR;}
"fin"       {return FIN;}
"+"         {return SUMA;}
"-"         {return RESTA;} 
")"         {return PARENDERECHO;}
"("         {return PARENIZQUIERDO;}
{IDENTIFICADOR} {yylval.cadena = strdup(yytext); return ID;}
{CONST} {yylval.num = atoi(yytext); return CONSTANTE; }
{ID_ERROR}    { yylexerrs++; printf("Error lexico: %s es un identificador invalido\n", yytext); return 0; }

\n  { yylineno++; }
[ \t\nEOF]             ;  // Ignorar espacios en blanco y tabulaciones, EOF ingorado debido a la presencia de "fin"
. {yylexerrs++; printf("Error lexico: %s es un caracter invalido\n", yytext); return 0;}
%%

int yywrap() {
    return 1;
}

