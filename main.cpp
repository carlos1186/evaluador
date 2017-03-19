# evaluador
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

using namespace std;

#define Inicial        0
#define Suma           1
#define Resta          2
#define Division       3
#define Multiplicacion 4
#define Digit          5
#define ParentesisIzq      6
#define ParentesisDer      7
#define Point          8
#define Number       10
#define AlphaError  20
#define Error          97
#define Aceptacion     98
#define FinalDelaExpresion 99
#define TRUE  1
#define FALSE 0

struct TAExp {
     TAExp *Izq;
     TAExp *Der;
     int          Info;
     float        Valor;
     };
     
void LimpiaCadena( char , int );
void TakeToken1( void );
void TakeToken( void );
TAExp *E( void );
TAExp *T( void );
TAExp *F( void );
TAExp *CreaArbol( TAExp, int , float );
void DestroysArbol( TAExp ** );
void EvaluaExpresion( void );
float Eval( TAExp * );
void DiagramaArbol( int, TAExp *  );


char Lexema[20];
char Exp[100];
int  CodigoErroneo;
int  Token;
int  Cent = 0;
int  PrevToken;

void LimpiaCadena( char punta[] , int largo )
{
    char *puntaTemp = punta;
    int  i;

    for(i=0;i<largo;i++)
       *(puntaTemp++) = '\0';
}

int Scan( char Sym )
{
    if (( Sym >= 48 ) && ( Sym <=57))
       return Digit;
       else switch (Sym) {
               case 43 : return Suma;
               case 45 : return Resta;
               case 47 : return Division;
               case 42 : return Multiplicacion;
               case 40 : return ParentesisIzq;
               case 41 : return ParentesisDer;
               case 36 : return FinalDelaExpresion;
               case 46 : return Point;
               default  : return AlphaError;
            }
}

void TakeToken()
{
   int  Estado = Inicial;
   char Sym = '\0';
   char *c;
   int Alto = FALSE;
   int IsPoint = FALSE;

   LimpiaCadena(Lexema,20);
   c = Lexema;
   while ( Alto != TRUE ) {
    Sym = Exp[Cent++];
    switch (Estado) {
    case Inicial    : 
	switch(Scan(Sym)) 
	{
      case Digit         :  *c++   = Sym;
      Estado = Number;
      break;
      case Multiplicacion :  *c++   = Sym;
      Estado = Multiplicacion;
      Alto   = TRUE;
      break;
      case Division       :  *c++   = Sym;
      Estado = Division;
      Alto   = TRUE;
      break;
      case Suma           :  *c++   = Sym;
      Estado = Suma;
      Alto   = TRUE;
      break;
      case Resta          :  *c++   = Sym;
      Estado = Resta;
      Alto   = TRUE;
      break;
      case ParentesisIzq      :  *c++   = Sym;
      Estado = ParentesisIzq;
      Alto   = TRUE;
      break;
      case ParentesisDer      :  *c++   = Sym;
      Estado = ParentesisDer;
      Alto   = TRUE;
      break;
      case FinalDelaExpresion :  Estado = FinalDelaExpresion;
      *c++   = Sym;
      Alto   = TRUE;
      break;
      default             :  Estado = AlphaError;
      CodigoErroneo = AlphaError;
      *c++   = Sym;
      Alto   = TRUE;
      } 
      break;
      case Number     : switch(Scan(Sym)) {
      case Digit  :
	  *c++ = Sym;
      break;
      case Point    :
	  *c++ = Sym;
	  if (IsPoint == FALSE)
      IsPoint = TRUE;
      else {
      Estado = AlphaError;
      CodigoErroneo = AlphaError;
      Alto = TRUE;
                                                        }
                                    break;
                                    default :  Cent--; 
                                    Alto = TRUE;

                                 }
        } 
   }
   Token = Estado;
}
void TakeToken1()
{
if ( PrevToken == TRUE )
PrevToken = FALSE;
else TakeToken();
} 

void DestroysArbol( TAExp **Punta )
{
if (*Punta != NULL ) {
DestroysArbol( &(*Punta)->Izq );
DestroysArbol( &(*Punta)->Der );
free( *Punta );
}
}
TAExp *CreaArbol(TAExp *Izq,TAExp *Der,int Tipo,float Valor)
{
   TAExp *NodoArbol = NULL;

   if ((NodoArbol=(TAExp *)malloc(sizeof(TAExp)))==0)
    return NULL; 
    else {
    NodoArbol->Izq   = Izq;   
    NodoArbol->Der   = Der;   
    NodoArbol->Info  = Tipo;  
    NodoArbol->Valor = Valor; 
    return NodoArbol;
    }
}
TAExp *F()
{
   TAExp *Ptr = NULL;;

   TakeToken1();
   if (Token == ParentesisIzq) {
   Ptr = E();
   if (Ptr == NULL)
   return NULL;
   TakeToken1();
   if (Token == ParentesisDer)
   return Ptr;
   }
   else if ( Token == Number )
   return CreaArbol( NULL , NULL , Number , atof(Lexema) );
   else return NULL;
} 
TAExp *T()
{
TAExp *Izq = NULL;
TAExp *Der = NULL;
int Op;

Izq = F();
if ( Izq == NULL )
return NULL;  
else {
TakeToken();
if ((Token==Multiplicacion)||(Token==Division)) {
Op = Token;
Der = T();
if (Der == NULL) { 
DestroysArbol( &Izq );
return NULL;
}
else return CreaArbol( Izq , Der , Op , 0 );
}
else {
PrevToken = TRUE;
return Izq;
}
}
}

TAExp *E()
{
    TAExp *Izq = NULL;
    TAExp *Der = NULL;
    int Operador;

    Izq = T();
    if ( Izq == NULL )
    return NULL;   
    else {
    TakeToken1();
    if ((Token==Suma)||(Token==Resta)) {
    Operador = Token;
    Der = E();
    if (Der == NULL) { 
    DestroysArbol( &Izq );
    return NULL;
    }
    else return CreaArbol( Izq , Der , Operador , 0 );
    }
    else
    if (Token == AlphaError ) {
    DestroysArbol( &Izq );
    return NULL;
    }
    else {
    PrevToken = TRUE;
    return Izq;
    }
  }
}
void DiagramaArbol( TAExp *RaizPunta , int Nivel )
{
   if ( RaizPunta != NULL )
      {
    DiagramaArbol( RaizPunta->Der , Nivel + 1 );
    for(int i=0;i<Nivel;i++)
    printf("    ");
    switch (RaizPunta->Info) {
    case Multiplicacion : printf("*");
    break;
    case Division       : printf("/");
    break;
    case Suma           : printf("+");
    break;
    case Resta          : printf("-");
    break;
    default             : printf("%f",RaizPunta->Valor);
    }
    printf("\n");
    DiagramaArbol( RaizPunta->Izq , Nivel + 1 );
      }
}
float Eval( TAExp *RaizPunta )
{
    float Derecho   = 0;
    float Izquierdo = 0;
    char tecla = '\0';

    if ( RaizPunta != NULL ) {
        if (RaizPunta->Info == Number)
           return RaizPunta->Valor;
        else {
        Izquierdo = Eval( RaizPunta->Izq );
        Derecho   = Eval( RaizPunta->Der );
        switch (RaizPunta->Info) {
        case Suma : return (Izquierdo + Derecho);
                                       break;
        case Resta : return (Izquierdo - Derecho);
                                       break;
        case Division : if ( Derecho == 0 ) {
        printf("Error, imposible dividir entre cero !\n");
        exit(0);
        }
        return (Izquierdo / Derecho);
        break;
        case Multiplicacion : return (Izquierdo * Derecho);
        break;
        }
      }
    }
}
void EvaluaExpresion()
{
    TAExp *RaizPunta = NULL;
    char tecla = '\0';
    float Resultado = 0;

    RaizPunta = E();    // La funcion devuelve un puntero al ?rbol de expresiones
    if (RaizPunta == NULL )
       printf("Error \n");
       else {
               printf("\n\n");
               DiagramaArbol(RaizPunta, 1 );
               Resultado = Eval(RaizPunta );
               DestroysArbol(&RaizPunta );
               printf("\nEste es el resultado: %f\n",Resultado);
            }
}

//                                   Programa principal

int main( int argn , char *argv[])
{
               LimpiaCadena(Exp,100);
               for (int i=1;i<argn;i++)
                    strcat(Exp,argv[i]);
               strcat(Exp,"$\0");
            
    			PrevToken = FALSE;
   				EvaluaExpresion();
}
