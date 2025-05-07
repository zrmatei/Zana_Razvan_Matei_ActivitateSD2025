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

typedef struct Nod Nod;

struct Nod {
	Masina info;
	Nod* st;
	Nod* dr;
};

//creare structura pentru un nod dintr-un arbore binar de cautare

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltimeArbore(Nod* radacina) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (radacina != NULL) {
		int inaltimest = calculeazaInaltimeArbore(radacina->st);
		int inaltimedr = calculeazaInaltimeArbore(radacina->dr);
		return 1 + (inaltimest > inaltimedr ? inaltimest : inaltimedr);
	}
	else
	{
		return 0;
	}
}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.

void rotireStanga(Nod** radacina) {
	Nod* nod;
	nod = (*radacina)->dr;
	(*radacina)->dr = nod->st;
	nod->st = *radacina;
	*radacina = nod;
}

void rotireDreapta(Nod** radacina) {
	Nod* aux;
	aux = (*radacina)->st;
	(*radacina)->st = aux->dr;
	aux->dr = (*radacina);
	*radacina = aux;
}

int gradEchilibru(Nod* radacina) {
	if (radacina != NULL) {
		return (calculeazaInaltimeArbore(radacina->st) - calculeazaInaltimeArbore(radacina->dr));
	}
	else {
		return 0;
	}
}

void adaugaMasinaInArboreEchilibrat(Nod** radacina, Masina masinaNoua) {
	if (*radacina == NULL) {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = masinaNoua;
		nod->st = NULL;
		nod->dr = NULL;
		*radacina = nod;
	}
	else {
		if ((*radacina)->info.id > masinaNoua.id) {
			adaugaMasinaInArboreEchilibrat(&(*radacina)->st, masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&(*radacina)->dr, masinaNoua);
		}

		int grad = gradEchilibru(*radacina);
		if (grad == 2) { // dezechilibru stanga
			if (gradEchilibru((*radacina)->st) >= 0) {
				rotireDreapta(radacina);
			}
			else {
				rotireStanga(&((*radacina)->st));
				rotireDreapta(radacina);
			}
		}
		else if (grad == -2) { // dezechilibru dreapta
			if (gradEchilibru((*radacina)->dr) <= 0) {
				rotireStanga(radacina);
			}
			else {
				rotireDreapta(&((*radacina)->dr));
				rotireStanga(radacina);
			}
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
		Nod* arbore = NULL;
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			adaugaMasinaInArboreEchilibrat(&arbore, m);
		}
		fclose(f);
		return arbore;

}

void afisarePreOrdine(Nod* arbore) {
	if (arbore) {
				afisareMasina(arbore->info);
				afisarePreOrdine(arbore->st);
				afisarePreOrdine(arbore->dr);
			}
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if (*arbore) {
		dezalocareArboreDeMasini(&(*arbore)->st);
		dezalocareArboreDeMasini(&(*arbore)->dr);
		free((*arbore)->info.model);
		free((*arbore)->info.numeSofer);
		free(*arbore);
		(*arbore) = NULL;
	}
}

Masina getMasinaByID(Nod* arbore, int id) {
	Masina m;
	m.id = -1;
	if (arbore) {
		if (arbore->info.id < id) {
			return getMasinaByID(arbore->dr, id);
		}
		else if (arbore->info.id > id) {
			return getMasinaByID(arbore->st, id);
		}
		else {
			return arbore->info;
		}
	}
	return m;
}

int determinaNumarNoduri(/*arborele de masini*/);

float calculeazaPretulMasinilorUnuiSofer(const char* numeSofer);

float calculeazaPretTotal(Nod* radacina) {
	if (radacina != NULL) {
		float pretSubarboreStang = calculeazaPretTotal(radacina->st);
		float pretSubarboreDrept = calculeazaPretTotal(radacina->dr);
		float pretTotal = pretSubarboreStang + pretSubarboreDrept + radacina->info.pret;
		return pretTotal;
	}
	else {
		return 0;
	}
}


float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer);

int main() {

	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arb.txt");
	afisarePreOrdine(arbore);
	printf("Pretul total al masinilor %.2f", calculeazaPretTotal(arbore));
	return 0;
}