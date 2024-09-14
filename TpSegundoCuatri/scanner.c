/*

link al docs: https://docs.google.com/document/d/1qp0yhD5ABOVlFwAY9jzOJOZ1I4TllnRc3X-ho10-s78/edit

Voy a necesitar al menos ocho líneas para que el trabajo sea aceptable. Para obtener un 8, deberán realizar tanto el análisis sintáctico descendente recursivo 
como el análisis sintáctico ascendente. El análisis sintáctico ascendente es muy fácil, ya sea la semana que viene o la siguiente, 
mientras que el análisis sintáctico es más complicado. Para obtener un 9, deberán incluir algo que no está en el enunciado, 
como una lista de sentencias o de identificadores, ya que la coma no está incorporada. 
Finalmente, para obtener un 10, deberán agregar un operador multiplicativo. ¿Está claro?
*/


/* Compilador del Lenguaje Micro (Fischer)	aa*/ 
#include "scanner.h"


/******************Declaraciones Globales*************************/

FILE * in;

char buffer[TAMLEX];
TOKEN tokenActual;
int flagToken = 0;

/***************************Programa Principal************************/
/*ingresamos por la linea de comando el nombre del archivo ("codigo fuente" argv[1]) y el ejecutable (./scanner.c) y argc 
cuenta cuantos parametros ingresamos (el ejecutable cuenta siendo argv[0])*/
int main(int argc, char * argv[])
{
 TOKEN tok;
 char nombreArchivo[TAMNOM];
 int largoNombreArchivo;


/***************************Se abre el Archivo Fuente******************/
//./sanner.exe ESO.m
 if ( argc == 1 )
 {
  printf("Debe ingresar el nombre del archivo fuente (en lenguaje Micro) en la linea de comandos\n");
  return -1;
 }
 if ( argc != 2 )
 {
  printf("Numero incorrecto de argumentos\n");
  return -1;
 }
 strcpy(nombreArchivo, argv[1]);
 largoNombreArchivo = strlen(nombreArchivo);
 if ( largoNombreArchivo > TAMNOM )
 {
  printf("Nombre incorrecto del Archivo Fuente\n");
  return -1;
 }
 /*Nos asegura que el archivo que mandemos sea "archivo.m" (para micro)*/
 if ( nombreArchivo[largoNombreArchivo-1] != 'm' || nombreArchivo[largoNombreArchivo-2] != '.' )
 {
  printf("Nombre incorrecto del Archivo Fuente\n");
  return -1;
 }

 if ( (in = fopen(nombreArchivo, "r") ) == NULL )
 {
  printf("No se pudo abrir archivo fuente\n");
  return -1;
 }

/*************************Inicio Compilacion***************************/

 Objetivo();

/**************************Se cierra el Archivo Fuente******************/

 fclose(in);
  
 return 0;
}

/**********Procedimientos de Analisis Sintactico (PAS) *****************/

void Objetivo(void)
{
 /* <objetivo> -> <programa> FDT #terminar */

 Programa(); /* Inicializaciones Semanticas */
 Match(FDT);
 Terminar();
}

void Programa(void)
{
 /* <programa> -> #comenzar INICIO <listaSentencias> FIN */

 Comenzar(); /* Inicializaciones Semanticas */
 Match(INICIO);
 ListaSentencias();

 Match(FIN);
}

void ListaSentencias(void)
{
 /* <listaSentencias> -> <sentencia> {<sentencia>} */

 Sentencia();

 while ( 1 )
 {
  switch ( ProximoToken() )
  {
   case ID : case LEER : case ESCRIBIR :
   Sentencia();
   break;
   default : return;
  }
 }
}

void Sentencia(void)
{
 TOKEN tok = ProximoToken();
 REG_EXPRESION izq, der;

 switch ( tok )
 {
  case ID :	/* <sentencia> -> ID := <expresion> #asignar ; */
   Identificador(&izq);
   Match(ASIGNACION);
   Expresion(&der);
   Asignar(izq, der);
   Match(PUNTOYCOMA);
   break;
  case LEER :	/* <sentencia> -> LEER ( <listaIdentificadores> ) */
   Match(LEER);
   Match(PARENIZQUIERDO);
   ListaIdentificadores();
   Match(PARENDERECHO);
   Match(PUNTOYCOMA);
   break;
  case ESCRIBIR :	/* <sentencia> -> ESCRIBIR ( <listaExpresiones> ) */
   Match(ESCRIBIR);
   Match(PARENIZQUIERDO);
   ListaExpresiones();
   Match(PARENDERECHO);
   Match(PUNTOYCOMA);
   break;
  default : return;
 }
}

void ListaIdentificadores(void)
{
 /* <listaIdentificadores> -> <identificador> #leer_id {COMA <identificador> #leer_id} */

 TOKEN t;
 REG_EXPRESION reg;

 Identificador(&reg);
 Leer(reg);

 for ( t = ProximoToken(); t == COMA; t = ProximoToken() )
 {
  Match(COMA);
  Identificador(&reg);
  Leer(reg);
 }
}

void Identificador(REG_EXPRESION * presul)
{
 /* <identificador> -> ID #procesar_id */

 Match(ID);
 *presul = ProcesarId();
}

void ListaExpresiones(void)
{
 /* <listaExpresiones> -> <expresion> #escribir_exp {COMA <expresion> #escribir_exp} */

 TOKEN t;
 REG_EXPRESION reg;

 Expresion(&reg);
 Escribir(reg);

 for ( t = ProximoToken(); t == COMA; t = ProximoToken() )
 {
  Match(COMA);
  Expresion(&reg);
  Escribir(reg);
 }
}

void Expresion(REG_EXPRESION * presul)
{
 /* <expresion> -> <primaria> { <operadorAditivo> <primaria> #gen_infijo } */

 REG_EXPRESION operandoIzq, operandoDer;
 char op[TAMLEX];
 TOKEN t;

 Primaria(&operandoIzq);

 for ( t = ProximoToken(); t == SUMA || t == RESTA; t = ProximoToken() )
 {
  OperadorAditivo(op);
  Primaria(&operandoDer);
  operandoIzq = GenInfijo(operandoIzq, op, operandoDer);
 }
 *presul = operandoIzq;
}

void Primaria(REG_EXPRESION * presul)
{
 TOKEN tok = ProximoToken();

 switch ( tok )
 {
  case ID :	/* <primaria> -> <identificador> */
   Identificador(presul);
   break;
  case CONSTANTE :	/* <primaria> -> CONSTANTE #procesar_cte */
   Match(CONSTANTE);
   *presul = ProcesarCte();
   break;
  case PARENIZQUIERDO :	/* <primaria> -> PARENIZQUIERDO <expresion> PARENDERECHO */
   Match(PARENIZQUIERDO);
   Expresion(presul);
   Match(PARENDERECHO);
   break;
  default : return;
 }
}

void OperadorAditivo(char * presul)
{
 /* <operadorAditivo> -> SUMA #procesar_op | RESTA #procesar_op */

 TOKEN t = ProximoToken();

 if ( t == SUMA || t == RESTA )
 {
  Match(t);
  strcpy(presul, ProcesarOp());
 }
 else
  ErrorSintactico(t);
}

/**********************Rutinas Semanticas******************************/

REG_EXPRESION ProcesarCte(void)
{
 /* Convierte cadena que representa numero a numero entero y construye un registro semantico */

 REG_EXPRESION reg;

 reg.clase = CONSTANTE;
 strcpy(reg.nombre, buffer);
 sscanf(buffer, "%d", &reg.valor);

 return reg;
}

REG_EXPRESION ProcesarId(void)
{
 /* Declara ID y construye el correspondiente registro semantico */

 REG_EXPRESION reg;

 Chequear(buffer);
 reg.clase = ID;
 strcpy(reg.nombre, buffer);

 return reg;
}

char * ProcesarOp(void)
{
 /* Declara OP y construye el correspondiente registro semantico */

 return buffer;
}

void Leer(REG_EXPRESION in)
{
 /* Genera la instruccion para leer */

 Generar("Read", in.nombre, "Entera", "");
}

void Escribir(REG_EXPRESION out)
{
 /* Genera la instruccion para escribir */

 Generar("Write", Extraer(&out), "Entera", "");
}

REG_EXPRESION GenInfijo(REG_EXPRESION e1, char * op, REG_EXPRESION e2)
{
 /* Genera la instruccion para una operacion infija y construye un registro semantico con el resultado */

 REG_EXPRESION reg;
 static unsigned int numTemp = 1;
 char cadTemp[TAMLEX] ="Temp&";
 char cadNum[TAMLEX];
 char cadOp[TAMLEX];

 if ( op[0] == '-' ) strcpy(cadOp, "Restar");
 if ( op[0] == '+' ) strcpy(cadOp, "Sumar");

 sprintf(cadNum, "%d", numTemp);
 numTemp++;
 strcat(cadTemp, cadNum);

 if ( e1.clase == ID) Chequear(Extraer(&e1));
 if ( e2.clase == ID) Chequear(Extraer(&e2));
 Chequear(cadTemp);
 Generar(cadOp, Extraer(&e1), Extraer(&e2), cadTemp);

 strcpy(reg.nombre, cadTemp);

 return reg;
}
/***************Funciones Auxiliares**********************************/

void Match(TOKEN t)
{
 if ( !(t == ProximoToken()) ) ErrorSintactico();
 flagToken = 0;
}

TOKEN ProximoToken() //ver pq chota hay un flag aca
{
 if ( !flagToken )
 {
  tokenActual = scanner();
  if ( tokenActual == ERRORLEXICO ) ErrorLexico();

  flagToken = 1;

  if ( tokenActual == ID ) Buscar(buffer, TS, &tokenActual);   //Buscar que hace esta cosa
   
   }
 return tokenActual;
}

void ErrorLexico()
{
 printf("Error Lexico\n");
}

void ErrorSintactico()
{
 printf("Error Sintactico\n");
}

void Generar(char * co, char * a, char * b, char * c)
{
 /* Produce la salida de la instruccion para la MV por stdout */

 printf("%s %s%c%s%c%s\n", co, a, ',', b, ',', c);
}

char * Extraer(REG_EXPRESION * preg)
{
 /* Retorna la cadena del registro semantico */

 return preg->nombre;
}

int Buscar(char * id, RegTS * TS, TOKEN * t)
{
 /* Determina si un identificador esta en la TS */

 int i = 0;

 while ( strcmp("$", TS[i].identifi) )
 {
  if ( !strcmp(id, TS[i].identifi) )
  {
   *t = TS[i].t;
   return 1; 
  }
  i++;
 }
 return 0;
}

void Colocar(char * id, RegTS * TS)
{
 /* Agrega un identificador a la TS */

 int i = 4;

 while ( strcmp("$", TS[i].identifi) ) i++;
 
 if ( i < 999 )
 {
  strcpy(TS[i].identifi, id );
  TS[i].t = ID;
  strcpy(TS[++i].identifi, "$" );
 }
}

void Chequear(char * s)
{
 /* Si la cadena No esta en la Tabla de Simbolos la agrega,
    y si es el nombre de una variable genera la instruccion */
 TOKEN t;

 if ( !Buscar(s, TS, &t) )
 {
  Colocar(s, TS);
  Generar("Declara", s, "Entera", "");
 }
}

void Comenzar(void)
{
 /* Inicializaciones Semanticas */
}

void Terminar(void)
{
 /* Genera la instruccion para terminar la ejecucion del programa */

 Generar("Detiene", "", "", "");
}

void Asignar(REG_EXPRESION izq, REG_EXPRESION der)
{
 /* Genera la instruccion para la asignacion */
   
 Generar("Almacena", Extraer(&der), izq.nombre, "");
}

/**************************Scanner************************************/

//este scanner es para hacer un analisis descendente y nosotros tenemos que implementar el ascendente usando flex como scanner

TOKEN scanner()
{
 int tabla[NUMESTADOS][NUMCOLS] = { {  1,  3,  5,  6,  7,  8,  9, 10, 11, 14, 13,  0, 14 },
 				    {  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2 },
 				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
				    {  4,  3,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4 },
				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 14, 14, 14 },
				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
				    { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 } };
 int car;
 int col;
 int estado = 0;

 int i = 0;
 
 do
 {
  car = fgetc(in); //toma un caracter del archivo
  col = columna(car);
  estado = tabla[estado][col];

  if ( col != 11 )
  {
   buffer[i] = car;
   i++;
  } 
 }
 while ( !estadoFinal(estado) && !(estado == 14) );

 buffer[i] = '\0'; //Una vez terminada de leer todas las lineas y habiendo almacenado todo en el buffer, se asigna el ultimo caracter como \0

 switch ( estado )
 {
  case 2 : if ( col != 11 )
	   {
	    ungetc(car, in);
	    buffer[i-1] = '\0';
	   }
   	   return ID;
  case 4 : if ( col != 11 )
	   {
	    ungetc(car, in);
	    buffer[i-1] = '\0';
	   }
           return CONSTANTE;
  case 5 : return SUMA;
  case 6 : return RESTA;
  case 7 : return PARENIZQUIERDO;
  case 8 : return PARENDERECHO;
  case 9 : return COMA;
  case 10 : return PUNTOYCOMA;
  case 12 : return ASIGNACION;
  case 13 : return FDT;
  case 14 : return ERRORLEXICO;
 }
 return 0;
}
int estadoFinal(int e)
{
 if ( e == 0 || e == 1 || e == 3 || e == 11 || e == 14 ) return 0;
 return 1;
}
int columna(int c)
{
 if ( isalpha(c) ) return 0;
 if ( isdigit(c) ) return 1;
 if ( c == '+' ) return 2;
 if ( c == '-' ) return 3;
 if ( c == '(' ) return 4;
 if ( c == ')' ) return 5;
 if ( c == ',' ) return 6;
 if ( c == ';' ) return 7;
 if ( c == ':' ) return 8;
 if ( c == '=' ) return 9;
 if ( c == EOF ) return 10;
 if ( isspace(c) ) return 11;
 return 12;
}
/*************Fin Scanner**********************************************/

