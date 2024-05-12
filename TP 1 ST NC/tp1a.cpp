#include <stdio.h> // printf /
#include <ctype.h> // isdigit /

bool Verifica(char*);  // que los caracteres pertenezcan al alfabeto /
int Columna(int);  //dado un caracter, determina la columna  /
int EsPalabra(const char*);

int main()
{
    char s1[] = "14-8";
    printf("Este programa prueba un AFD que determina\n");
    printf("si una cadena dada corresponde a un numero entero\n\n");
    if (!Verifica(s1))
        printf("La cadena s1 tiene caracteres invalidos\n");
    else
        if (EsPalabra(s1))
            printf("La cadena s1 pertenece al lenguaje\n");
        else
            printf("La cadena s1 no pertenece al lenguaje\n");
    return 0;
} //* fin main /

bool Verifica(char *s)
{
    unsigned i;
    for (i = 0; s[i] != '\0'; i++)
        if (!isdigit(s[i]) && s[i] != '+' && s[i] != '-')
            return false;
    return true;
} //* fin Verifica /

int Columna(int c)
{
    switch (c)
    {
        case '+':
            return 1;
        case '-':
            return 2;
        default: /// es digito
            return 0;
    }
} // fin Columna /

int EsPalabra(const char *cadena)
{
    static int tt[4][4] ={
                {1,3,3,3},          //* Tabla de Transiciones /
                {1,2,2,3},          // 2 es el estado final /
                {1,3,3,3},          // 3 es el estado de rechazo /
                {3,3,3,3}};
    int e; // estado actual del automata /
    int i; // recorre la cadena /
    for (e = 0, i = 0; cadena[i] != '\0' && e != 3; i++)
        e = tt[e][Columna(cadena[i])];
    return e == 1;    // estado final? retorna 1 /
}  //fin EsPalabra */