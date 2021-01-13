#include "TCoada.h"
#include "TStiva.h"

TCoada InitQ() {
	TCoada new = (TCoada)malloc(sizeof(TElem));
	if (!new) return NULL;
	*new = NULL;

	return new;
}

void Enqueue(TCoada q, void *x) {
	TElem new = (TElem)malloc(sizeof(TCelula));
	if (!new) return;
	new->info = x;
	new->urm = NULL;

	if(Vida(q)) {
		*q = new;
		return;
	}
	TElem aux = *q;
	while(aux->urm != NULL)
		aux = aux->urm;
	aux->urm = new;
}

void *Dequeue(TCoada q) {
	if(Vida(q))
		return NULL;
	void *result = (*q)->info;
	TElem aux = *q;
	*q = (*q)->urm;
	free(aux);

	return result;
}

void *Peek(TCoada q) {
	if (Vida(q))
		return NULL;

	return (*q)->info;
}

void MutaQ(TCoada dest, TCoada source) {
	Enqueue(dest, Dequeue(source));
}

void CopiereQ(TCoada dest, TCoada source) {
	TCoada aux = InitQ();
	while (!Vida(source)) {
		void *elm = Dequeue(source);
		Enqueue(aux, elm);
		Enqueue(dest, elm);
	}
	while (!Vida(aux)) {
		MutaQ(source,aux);
	}
}

int NrEQ(TCoada q) {
	TCoada aux = InitQ();
	int count = 0;

	while (!Vida(q)) {
		MutaQ(aux, q);
		count++;
	}

	while(!Vida(aux))
		MutaQ(q, aux);

	return count;
}

void ConcatQ(TCoada dest, TCoada source) {
	while(!Vida(source))
		MutaQ(dest, source);
}

void AfisareQ(TCoada q, void (*afiEl)(void *, FILE *outputFile), FILE *outputFile) {
	TCoada aux = InitQ();

	while (!Vida(q)) {
		void *elm = Dequeue(q);
		afiEl(elm, outputFile);
		fprintf(outputFile, "\n");
		Enqueue(aux, elm);
	}

	while(!Vida(aux)) {
		Enqueue(q, Dequeue(aux));
	}

	DistrQ(aux);
}

void DistrQ(TCoada q) {
	while (!Vida(q))
		free(Dequeue(q));

	free(q);
	q = NULL;
}

void InverseazaQ(TCoada q) {
	TStiva s = InitS();
	while (!Vida(q)) {
		void *elm = Dequeue(q);
		Push(s, elm);
	}
	while (!Vida(s)) {
		void *elm = Pop(s);
		Enqueue(q, elm);
	}
	DistrS(s);
}

void pushInQueue(TCoada q, void *temp, TFCmp cmp) {
	int i;
	int n = NrEQ(q);
    if (Vida(q)) {
        Enqueue(q, temp);
        return;
    }
    else {
		int ok = 0;
		for (i = 0; i < n; i++) {
			void *aux = Dequeue(q);
			if (cmp(temp, aux) <= 0 && ok == 0) {
				Enqueue(q, temp);
				ok = 1;
			}
			Enqueue(q, aux);
		}
		if (!ok) Enqueue(q, temp);
    }
}

void SortareQ(TCoada q, TFCmp cmp) {
    if (Vida(q))
        return;
    void *temp = Dequeue(q);
    SortareQ(q, cmp);
    pushInQueue(q, temp, cmp);
}
