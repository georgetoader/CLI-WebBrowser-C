#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#ifndef _ELEMENT_
#define _ELEMENT_

/* check empty */
#define Vida(T) ((*T) == NULL)
/* comparing function */
typedef int (*TFCmp)(void*, void*);

typedef struct celula
{
	void *info;
	struct celula *urm;
} TCelula, *TElem, **TStiva, **TCoada;

#endif /* _ELEMENT_ */

#ifndef _STIVA_H_
#define _STIVA_H_

/* initialize stack */
TStiva InitS();

/* add element in stack */
void Push(TStiva s, void *x);

/* remove element from stack */
void *Pop(TStiva s);

/* return reference to top element in stack */
void *Top(TStiva s);

/* move an element from "source" to "dest" */
void MutaS(TStiva dest, TStiva source);

/* copy stack "source" in "dest" without modifying the initial stack */
void CopiereS(TStiva dest, TStiva source);

/* number of elements in stack */
int NrES(TStiva s);

/* print elements of stack using "afiEl" for different types of elements */
void AfisareS(TStiva s, void (*afiEl)(void *));

/* delete stack and free the memory occupied by it */
void DistrS(TStiva s);

/* reverse stack */
void InverseazaS(TStiva s);

/* sort stack bv the criterion in "cmp" */
void SortareS(TStiva s, TFCmp cmp);

#endif /* _STIVA_H_ */
