#include "TStiva.h"
#include "TCoada.h"

TStiva InitS() {
	TStiva new = (TStiva)malloc(sizeof(TElem));
	if (!new) return NULL;
	*new = NULL;

	return new;
}

void Push(TStiva s, void *x) {
	TElem new = (TElem)malloc(sizeof(TCelula));
	if (!new) return;
	new->info = x;
	new->urm = NULL;

	if(Vida(s)) {
		*s = new;
		return;
	}

	new->urm = *s;
	*s = new;
}

void *Pop(TStiva s) {
	if(Vida(s))
		return NULL;

	void *result = (*s)->info;
	TElem aux = *s;
	*s = (*s)->urm;
	free(aux);

	return result;
}

void *Top(TStiva s) {
	if (Vida(s))
		return NULL;

	return (*s)->info;
}

void MutaS(TStiva dest, TStiva source) {
	Push(dest, Pop(source));
}

void InverseazaS(TStiva s) {
	TCoada q = InitQ();
	while (!Vida(s)) {
		void *elm = Pop(s);
		Enqueue(q, elm);
	}
	while (!Vida(q)) {
		void *elm = Dequeue(q);
		Push(s, elm);
	}
	DistrQ(q);
}

void CopiereS(TStiva dest, TStiva source) {
	TStiva aux = InitS();
	while (!Vida(source)) {
		void *elm = Pop(source);
		Push(aux, elm);
		Push(dest, elm);
	}
	while (!Vida(aux)) {
		MutaS(source,aux);
	}
	InverseazaS(dest);
}

int NrES(TStiva s) {
	TStiva aux = InitS();
	int count = 0;

	while (!Vida(s)) {
		MutaS(aux, s);
		count++;
	}

	while(!Vida(aux))
		MutaS(s, aux);

	return count;
}

void AfisareS(TStiva s, void (*afiEl)(void *)) {
	TStiva aux = InitS();

	while (!Vida(s)) {
		void *elm = Pop(s);
		afiEl(elm);
		printf("\n");
		Push(aux, elm);
	}

	while(!Vida(aux))
		MutaS(s, aux);

	DistrS(aux);
}

void DistrS(TStiva s) {
	while (!Vida(s))
		free(Pop(s));

	free(s);
	s = NULL;
}

void sortedInsertS(TStiva s, void *x, TFCmp cmp) {
	void *aux = Top(s);
	if (Vida(s) || (aux && cmp(x,aux) > 0)) {
		Push(s, x);
		return;
	}
    void *temp = Pop(s);
    sortedInsertS(s, x, cmp);
	Push(s, temp);
}

void SortareS(TStiva s, TFCmp cmp) {
    if (!Vida(s)){
        void *x = Pop(s);
        SortareS(s, cmp);
        sortedInsertS(s, x, cmp);
	}
}

