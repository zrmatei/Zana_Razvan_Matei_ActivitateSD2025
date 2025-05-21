#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinBuffer(char* buffer) {
	char sep[3] = ",\n";
	char* aux;
	Masina m1;

	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	aux = strtok(NULL, sep);
	m1.serie = aux ? aux[0] : '?';

	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

typedef struct Nod {
	Masina info;
	struct Nod* next;
} Nod;

void pushStack(Nod** stiva, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = (*stiva);
	(*stiva) = nou;
}

Masina popStack(Nod** stiva) {
	if ((*stiva) != NULL) {
		Masina m = (*stiva)->info;
		Nod* temp = (*stiva);
		(*stiva) = (*stiva)->next;
		free(temp);
		return m;
	}
	Masina m;
	m.id = -1;
	m.model = NULL;
	m.numeSofer = NULL;
	return m;
}

unsigned char emptyStack(Nod* stiva) {
	return stiva == NULL;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	Nod* stiva = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		perror("Eroare la deschiderea fisierului");
		return NULL;
	}

	char buffer[100];
	while (fgets(buffer, sizeof(buffer), f)) {
		Masina m = citireMasinaDinBuffer(buffer);
		pushStack(&stiva, m);
	}
	fclose(f);
	return stiva;
}

void dezalocareStivaDeMasini(Nod** stiva) {
	while ((*stiva) != NULL) {
		Masina m = popStack(stiva);
		free(m.model);
		free(m.numeSofer);
	}
}

int stackSize(Nod* stiva) {
	int nr = 0;
	while (stiva != NULL) {
		nr++;
		stiva = stiva->next;
	}
	return nr;
}

// QUEUE (LDI)
typedef struct NodDublu {
	Masina info;
	struct NodDublu* next;
	struct NodDublu* prev;
} NodDublu;

typedef struct {
	NodDublu* prim;
	NodDublu* ultim;
} ListaDubla;

void enqueue(ListaDubla* coada, Masina masina) {
	NodDublu* nou = (NodDublu*)malloc(sizeof(NodDublu));
	nou->info = masina;
	nou->next = NULL;
	nou->prev = coada->ultim;

	if (coada->ultim != NULL)
		coada->ultim->next = nou;
	else
		coada->prim = nou;

	coada->ultim = nou;
}

Masina dequeue(ListaDubla* lista) {
	Masina m;
	m.id = -1;
	m.model = NULL;
	m.numeSofer = NULL;

	if (lista->prim != NULL) {
		m = lista->prim->info;
		NodDublu* temp = lista->prim;

		if (lista->prim->next != NULL) {
			lista->prim = lista->prim->next;
			lista->prim->prev = NULL;
		}
		else {
			lista->prim = NULL;
			lista->ultim = NULL;
		}
		free(temp);
	}
	return m;
}

ListaDubla citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	ListaDubla lista = { NULL, NULL };
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		perror("Eroare la deschiderea fisierului");
		return lista;
	}

	char buffer[100];
	while (fgets(buffer, sizeof(buffer), f)) {
		Masina m = citireMasinaDinBuffer(buffer);
		enqueue(&lista, m);
	}
	fclose(f);
	return lista;
}

void dezalocareCoadaDeMasini(ListaDubla* lista) {
	NodDublu* aux = lista->ultim;
	while (aux != NULL) {
		NodDublu* temp = aux;
		aux = aux->prev;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
	lista->prim = NULL;
	lista->ultim = NULL;
}

float calculeazaPretTotal(ListaDubla* coada) {
	float suma = 0;
	ListaDubla nou = { NULL, NULL };

	while (coada->prim) {
		Masina m = dequeue(coada);
		suma += m.pret;
		enqueue(&nou, m);
	}
	*coada = nou;
	return suma;
}

int main() {
	Nod* stiva = citireStackMasiniDinFisier("masini.txt");
	if (stiva) {
		Masina m = popStack(&stiva);
		afisareMasina(m);
		free(m.numeSofer);
		free(m.model);

		m = popStack(&stiva);
		afisareMasina(m);
		free(m.numeSofer);
		free(m.model);

		int nr = stackSize(stiva);
		printf("Elemente ramase in stiva: %d \n\n", nr);

		dezalocareStivaDeMasini(&stiva);
	}

	ListaDubla coada = citireCoadaDeMasiniDinFisier("Masini.txt");
	Masina m1 = dequeue(&coada);
	afisareMasina(m1);
	free(m1.numeSofer);
	free(m1.model);

	printf("Suma preturilor ramase in coada: %.2f\n", calculeazaPretTotal(&coada));
	dezalocareCoadaDeMasini(&coada);

	return 0;
}