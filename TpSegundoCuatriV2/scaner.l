
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
CONST   		{DIGITO}({DIGITO})*
IDENTIFICADOR 	{LETRA}({LETRA}|{DIGITO})* 


%%
EOF        {return FDT;}
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
{IDENTIFICADOR} {yylval.cadena = strdup(yytext); return ID;}
{CONST} {yylval.num = atoi(yytext); return CONSTANTE; }

\n  { yylineno++; }
[ \t\n]             ;  // Ignorar espacios en blanco y tabulaciones
. {yylexerrs++; printf("Error léxico: %s es un carácter inválido\n", yytext); return 0;}
%%

int yywrap() {
    return 1;
}

