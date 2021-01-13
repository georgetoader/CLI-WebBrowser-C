#include <stdio.h>
#include "TStiva.h"
#include "TCoada.h"
#include "structures.h"
#include "utils.h"
#include <string.h>

#define MAX_CMD 30
#define DEF_BAND 1024

/* write element of type string to output file */
void afisString(void* a, FILE *outputFile) {
	fprintf(outputFile, "%s", (char*)a);
}

/* comparison function of remaining size to be downloaded */
int fCmp(void *a, void *b) {
	Resource *x1 = (Resource *)a;
	Resource *x2 = (Resource *)b;
	long sum1 = x1->dimension - x1->currently_downloaded;
	long sum2 = x2->dimension - x2->currently_downloaded;
	return sum1 - sum2;
}

/* initialize browser */
Browser* INIT_BR() {
	Browser *browser = (Browser *)calloc(1,sizeof(Browser));
	if (!browser)
		return NULL;
	browser->tabs = InitS();
	browser->history = InitQ();
	browser->downloads = InitQ();
	browser->finished = InitQ();
	return browser;
}

/* set bandwidth */
unsigned long set_band(int value) {
	return (unsigned long)value;
}

/* open new tab */
void new_tab(Browser *browser) {
	Tab *newtab = (Tab *)calloc(1,sizeof(Tab));
	if (!newtab)
		return;
	//initialize tab
	newtab->current_page = NULL;
	newtab->back_stack = InitS();
	newtab->forward_stack = InitS();
	browser->current_tab = newtab;
	//add to stack
	Push(browser->tabs, newtab);
}

/* delete last opened tab */
void del_tab(Browser *browser) {
	if(Vida(browser->tabs))
		return;
	if (browser->current_tab == Top(browser->tabs)) {
		//current tab is also the last one opened
		Pop(browser->tabs);
		browser->current_tab = Top(browser->tabs);
	}else {
		Pop(browser->tabs);
	}
}

/* change current tab with the one on index "pos" */
Tab* change_tab(Browser *browser, int pos){
	//copy stack of tabs
	TStiva aux = InitS();
	CopiereS(aux,browser->tabs);

	int dim = NrES(browser->tabs) - 1;
	//number of steps to the desired tab
	int nr_pasi = dim - pos;
	int count = -1;
	while (!Vida(aux)) {
		count++;
		void *el = Pop(aux);
		//tab found so return a reference to it
		if (count == nr_pasi) return (Tab *)el;
	}
	DistrS(aux);
	//tab not found
	return browser->current_tab;
}

/* print info about open tabs */
void print_open_tabs(Browser *browser, FILE *outputFile) {
	if (Vida(browser->tabs)) {
		fprintf(outputFile, "Nu avem taburi deschise.");
		return;
	}
	//copy stack of tabs
	TStiva aux = InitS();
	CopiereS(aux,browser->tabs);
	InverseazaS(aux);

	int count = -1;
	//go through tabs
	while (!Vida(aux)) {
		count++;
		Tab* tab = (Tab *)Pop(aux);
		if (!tab->current_page) {
			//no open page
			fprintf(outputFile, "(%d: empty)\n", count);
		} else {
			fprintf(outputFile, "(%d: %s)\n", count, tab->current_page->url);
		}
	}
	DistrS(aux);
}

/* fictitious passage of time for the download of resources */
void wait(Browser *browser, int nr, unsigned long BANDWIDTH) {
	unsigned long bytes = BANDWIDTH * nr;
	while (!Vida(browser->downloads) && bytes != 0) {
		//highest priority element
		Resource *res = (Resource *)Peek(browser->downloads);
		//remaining size to download
		unsigned long remaining = res->dimension - res->currently_downloaded;
		if (bytes > remaining) {
			//finished downloading so added to "finished"
			//remaining bytes are subtracted from the remaining download of next resource
			bytes -= remaining;
			Enqueue(browser->finished, res);
			Dequeue(browser->downloads);
		} else {
			//didn't finish downloading
			res->currently_downloaded += bytes;
			bytes = 0;
		}
	}
}

/* open a new webpage in current tab */
void go_to(Browser *browser, char *url, unsigned long BANDWIDTH) {
	Tab *tab = browser->current_tab;

	int *nr = calloc(1,sizeof(int));
	if (!nr)
		return;
	Resource* x = get_page_resources(url, nr);

	//initialize new webpage
	Webpage *webp = (Webpage *)calloc(1,sizeof(Webpage));
	if (!webp)
		return;
	strcpy(webp->url, url);
	webp->num_res = (unsigned long)*nr;
	webp->resources = x;

	//if not first page opened in tab
	//add current page to "back_stack"
	if (tab->current_page) {
		Push(tab->back_stack, tab->current_page);
	}
	tab->current_page = webp;

	//empty stack "forward_stack"
	while (!Vida(tab->forward_stack))
		Pop(tab->forward_stack);

	//add to search history
	Enqueue(browser->history, webp);

	//1 second passes, in this time resources are downloaded
	wait(browser, 1, BANDWIDTH);
}

/* go back to previous page */
void back(Browser *browser, FILE *outputFile) {
	Tab *tab = browser->current_tab;
	if (Vida(tab->back_stack)) {
		fprintf(outputFile, "can’t go back, no pages in stack\n");
		return;
	}
	//add current page to "forward_stack"
	Push(tab->forward_stack, tab->current_page);
	//update current page
	tab->current_page = (Webpage *)Pop(tab->back_stack);
}

/* go to next page */
void forward(Browser *browser, FILE *outputFile) {
	Tab *tab = browser->current_tab;
	if (Vida(tab->forward_stack)) {
		fprintf(outputFile, "can’t go forward, no pages in stack\n");
		return;
	}
	//add current page to "back_stack"
	Push(tab->back_stack, tab->current_page);
	//update current page
	tab->current_page = (Webpage *)Pop(tab->forward_stack);
}

/* print search history */
void history(Browser *browser, FILE *outputFile) {
	AfisareQ(browser->history, afisString, outputFile);
}

/* delete first "nr" searches from search history */
void del_history(Browser *browser, int nr) {
	int i;
	//if 0 then delete the whole search history
	if (nr == 0) nr = NrEQ(browser->history);
	for (i = 1; i <= nr; i++) {
		Dequeue(browser->history);
	}
}

/* list of downloadable resources */
void list_dl(Browser *browser, FILE *outputFile) {
	Tab *tab = browser->current_tab;
	if (!tab->current_page)
		return;
	Webpage *webp = tab->current_page;
	int i;
	//print all resources
	for (i = 0; i < webp->num_res; i++) {
		fprintf(outputFile, "[%d - \"%s\" : %ld]\n", i, webp->resources[i].name,
				webp->resources[i].dimension);
	}
}

/* print current download status */
void print_downloads(Browser *browser, FILE *outputFile) {
	if (Vida(browser->downloads))
		return;
	TCoada aux = InitQ();
	CopiereQ(aux,browser->downloads);

	//print the queue of resources not yet fully downloaded
	while (!Vida(aux)) {
		Resource *res = (Resource *)Dequeue(aux);
		fprintf(outputFile, "[\"%s\" : %ld/%ld]\n", res->name, res->dimension -
				res->currently_downloaded, res->dimension);
	}
	DistrQ(aux);

	aux = InitQ();
	CopiereQ(aux,browser->finished);
	//print the queue of resources fully downloaded
	while (!Vida(aux)) {
		Resource *res = (Resource *)Dequeue(aux);
		fprintf(outputFile, "[\"%s\" : completed]\n", res->name);
	}
	DistrQ(aux);
}

/* add resource to download queue */
void download(Browser *browser, int nr) {
	Tab *tab = browser->current_tab;
	if (!tab->current_page)
		return;
	Webpage *webp = tab->current_page;
	Resource *new = (Resource *)calloc(1,sizeof(Resource));
	if (!new)
		return;
	//create copy of resource
	*new = webp->resources[nr];
	//add in queue on appropriate position
	//so that the priority order is being kept
	pushInQueue(browser->downloads, (void *)new, fCmp);
}

/* free allocated memory */
void dealoc(Browser *browser) {
	DistrQ(browser->finished);
	DistrQ(browser->downloads);
	free(browser->history);
	//go through tabs
	while (!Vida(browser->tabs)) {
		Tab *tab = (Tab *)Pop(browser->tabs);
		//free "back_stack"
		while (!Vida(tab->back_stack)) {
			Webpage *webp = (Webpage *)Pop(tab->back_stack);
			free(webp->resources);
			free(webp);
		}
		free(tab->back_stack);
		//free "forward_stack"
		while (!Vida(tab->forward_stack)) {
			Webpage *webp = (Webpage *)Pop(tab->forward_stack);
			free(webp->resources);
			free(webp);
		}
		free(tab->forward_stack);
		if (tab->current_page) {
			free(tab->current_page->resources);
			free(tab->current_page);
		}
		free(tab);
	}
	//free browser
	free(browser->tabs);
	free(browser);
}

int main(int argc, char *argv[]) {

	unsigned long BANDWIDTH = DEF_BAND;
	//initialize browser
	Browser *browser = INIT_BR();
	new_tab(browser);
	//necessary variables
	char *input = calloc(1, MAX_CMD);
	char *inputFileName = calloc(1, MAX_CMD);
	char *outputFileName = calloc(1, MAX_CMD);
	if (!input || !inputFileName || !outputFileName)
		return -1;
	strcpy(inputFileName,argv[1]);
	strcpy(outputFileName,argv[2]);

	FILE *inputFile = fopen(inputFileName, "rt");
	FILE *outputFile = fopen(outputFileName, "wt");
    if (!inputFile || !outputFile) {
        fprintf(stderr, "ERROR: Can't open file");
        return -1;
    }
    while (fscanf(inputFile, "%s", input) != EOF) {
    	if (strcmp(input,"set_band") == 0) {
			int value;
			fscanf(inputFile, "%d", &value);
			BANDWIDTH = set_band(value);
		} else if (strcmp(input,"newtab") == 0) {
			new_tab(browser);
		} else if (strcmp(input,"deltab") == 0) {
			del_tab(browser);
		} else if (strcmp(input,"change_tab") == 0) {
			int value;
			fscanf(inputFile, "%d", &value);
			browser->current_tab = change_tab(browser, value);
		} else if (strcmp(input,"print_open_tabs") == 0) {
			print_open_tabs(browser, outputFile);
		} else if (strcmp(input,"goto") == 0) {
			fscanf(inputFile, "%s", input);
			go_to(browser, input, BANDWIDTH);
		} else if (strcmp(input,"back") == 0) {
			back(browser, outputFile);
		} else if (strcmp(input,"forward") == 0) {
			forward(browser, outputFile);
		} else if (strcmp(input,"history") == 0) {
			history(browser, outputFile);
		} else if (strcmp(input,"del_history") == 0) {
			int value;
			fscanf(inputFile, "%d", &value);
			del_history(browser, value);
		} else if (strcmp(input,"list_dl") == 0) {
			list_dl(browser, outputFile);
		} else if (strcmp(input,"downloads") == 0) {
			print_downloads(browser, outputFile);
		} else if (strcmp(input,"download") == 0) {
			int value;
			fscanf(inputFile, "%d", &value);
			download(browser, value);
		} else if (strcmp(input,"wait") == 0) {
			int value;
			fscanf(inputFile, "%d", &value);
			wait(browser, value, BANDWIDTH);
		} else {
			printf("Comanda nu exista.");
		}
    }
    fclose(inputFile);
    fclose(outputFile);
    free(input);
    free(inputFileName);
    free(outputFileName);
    dealoc(browser);

	return 0;
}
