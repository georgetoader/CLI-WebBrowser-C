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

#ifndef _COADA_H_
#define _COADA_H_

/* initialize queue */
TCoada InitQ();

/* add element in queue */
void Enqueue(TCoada q, void *x);

/* remove element from queue */
void *Dequeue(TCoada q);

/* return reference to top element in queue */
void *Peek(TCoada q);

/* move an element from "source" to "dest" */
void MutaQ(TCoada dest, TCoada source);

/* copy queue "source" in "dest" without modifying the initial queue */
void CopiereQ(TCoada dest, TCoada source);

/* number of elements in queue */
int NrEQ(TCoada q);

/* move all elements from "source" to "dest" */
void ConcatQ(TCoada dest, TCoada source);

/* print elements of queue using "afiEl" for different types of elements */
void AfisareQ(TCoada q, void (*afiEl)(void *, FILE *outputFile), FILE *outputFile);

/* delete queue and free the memory occupied by it */
void DistrQ(TCoada q);

/* reverse queue */
void InverseazaQ(TCoada q);

/*
* Add element in queue on certain position such as the
* comparison criterion in function "cmp" is valid.
*/
void pushInQueue(TCoada q, void *temp, TFCmp cmp);

/* sort queue bv the criterion in "cmp" */
void SortareQ(TCoada q, TFCmp cmp);

#endif /* _COADA_H_ */
