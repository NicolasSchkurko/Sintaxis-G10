#define NUMESTADOS 17
#define NUMCOLS 15
#define TAMLEX 32 + 1
#define TAMNOM 20 + 1
#define MAX_OPERADOR 1

/******************Declaraciones Globales*************************/
FILE *in;
typedef enum
{
    INICIO,
    FIN,
    LEER,
    ESCRIBIR,
    ID,
    CONSTANTE,
    PARENIZQUIERDO,
    PARENDERECHO,
    PUNTOYCOMA,
    COMA,
    ASIGNACION,
    SUMA,
    RESTA,
    FDT,
    ERRORLEXICO,
    MULTIPLICACION,
    DIVISION,
} TOKEN;

typedef struct
{
    char identifi[TAMLEX];
    TOKEN t; /* t=0, 1, 2, 3 Palabra Reservada, t=ID=4 Identificador (ver enum) */
} RegTS;
RegTS TS[1000] = {{"inicio", INICIO}, {"fin", FIN}, {"leer", LEER}, {"escribir", ESCRIBIR}, {"$", 99}};

typedef struct
{
    TOKEN clase;
    char nombre[TAMLEX];
    int valor;
} REG_EXPRESION;

char buffer[TAMLEX];
TOKEN tokenActual;
int flagToken = 0;

/**********************Prototipos de Funciones************************/
TOKEN scanner(); // el scanner
int columna(int c);
int estadoFinal(int e);
void Objetivo(void); // del PAS
void Programa(void);
void ListaSentencias(void);
void Sentencia(void);
void ListaIdentificadores(void);
void Identificador(REG_EXPRESION *presul);
void ListaExpresiones(void);
void Expresion(REG_EXPRESION *presul);
void Primaria(REG_EXPRESION *presul);
void Operador(char *presul);
void Termino(REG_EXPRESION *presul);


REG_EXPRESION ProcesarCte(void);
REG_EXPRESION ProcesarId(void);
char *ProcesarOp(void);
void Leer(REG_EXPRESION in);
void Escribir(REG_EXPRESION out);
REG_EXPRESION GenInfijo(REG_EXPRESION e1, char *op, REG_EXPRESION e2);

void Match(TOKEN t);
TOKEN ProximoToken();
void ErrorLexico();
void ErrorSintactico();
void Generar(char *co, char *a, char *b, char *c);
char *Extraer(REG_EXPRESION *preg);
int Buscar(char *id, RegTS *TS, TOKEN *t);
void Colocar(char *id, RegTS *TS);
void RevisaEnTS(char *s);
void Comenzar(void);
void Terminar(void);
void Asignar(REG_EXPRESION izq, REG_EXPRESION der);