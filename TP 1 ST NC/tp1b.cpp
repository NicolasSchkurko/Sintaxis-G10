#include <stdio.h> // printf /
#include <ctype.h> // isdigit /

bool Verifica(char*);  // que los caracteres pertenezcan al alfabeto /
int Columna(int);  //dado un caracter, determina la columna  /
int EsPalabra(const char*);

int main()
{
    char s1[] = "(14-8)*(2+(5+6))";
    printf("Este programa prueba un AFD que determina\n");
    printf("si una cadena dada corresponde a una expresión matemática\n\n");
    if (!EsPalabra(s1))
        printf("La cadena s1 no pertenece al lenguaje\n");
    else
        printf("La cadena s1 pertenece al lenguaje\n");
    return 0;
} //* fin main /

int Columna(int c)
{
    switch (c)
    {
        case '+':
            return 1;
        case '-':
            return 1;
        case '*':
            return 1;
        case '/':
            return 1;
        case '(':
            return 2;
        case ')':
            return 3;
        default: /// es digito
            return 0;
    }
} // fin Columna /

int EsPalabra(const char *cadena)
{
    static int tt[6][5] ={
                {1,6,3,6,6},          //* Tabla de Transiciones /
                {1,2,3,6,6},          // 2 es el estado final /
                {1,6,3,6,6},          // 3 es el estado de rechazo /
                {4,6,6,2,6},
                {4,5,6,1,6},
                {4,6,3,6,6}};
    int e; // estado actual del automata /
    __UINT_FAST16_TYPE__ i; // recorre la cadena /
    for (e = 0, i = 0; cadena[i] != '\0' && e != 6; i++)
        e = tt[e][Columna(cadena[i])];
    return e == 1;    // estado final? retorna 1 /
}  //fin EsPalabra */