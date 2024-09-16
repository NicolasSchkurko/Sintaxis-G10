
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

{ID}    printf( "Un identificador: %s de longitud %d\n", yytext, yyleng);
.                   printf("SIMBOLO DESCONOCIDO: %s\n", yytext);

%%
int main(int argc, char * argv[]){
    if (argc > 1) { //Version resumida del verificador de archivo
        // Abrir el archivo .m
        FILE *archivo = fopen(argv[1], "r");
        if (!archivo) {
            perror("Error al abrir el archivo");
            return 1;
        }

        // Establecer el archivo como entrada para Flex
        yyin = archivo;
    }


    int token;

    while((token = yylex())){
        switch (token) {
        case ASIGNACION:
            printf("ASIGNACION\n");
            break;
        case PUNTOYCOMA:
            printf("PUNTOYCOMA\n");
            break;
        case INICIO:
            printf("Detecta Inicio\n");
            break;
        case FIN:
            printf("Detecta FIN\n");
            break;
        case LEER:
            printf("Lee \n");
            break;
        case ESCRIBIR:
            printf(" ESCRIBIR \n");
            break;
        case SUMA:
            printf("SUMA\n");
            break;
        case RESTA:
            printf("RESTA\n");
            break;  
        case ID:
            printf("ID\n");
            break;
        case CONSTANTE:
            printf("CONSTANTE\n");
            break;
        case PARENDERECHO:
            printf("PARENTESIS DERECHO\n");
            break;
        case PARENIZQUIERDO:
            printf("PARENTESIS IZQUIERDO\n"); 
            break;
        case FDT
            printf("termino el programa en micro\n");
        default:
            printf("TOKEN_DESCONOCIDO: %s\n");
            break;
    }
    }

    return 0;
}

