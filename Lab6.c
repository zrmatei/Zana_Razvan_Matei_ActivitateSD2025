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

typedef struct Nod {
	Masina info;
	struct Nod* urm;
} Nod;

struct HashTable {
	int dim;
	Nod** v;
};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;

	aux = strtok(buffer, sep);
	m1.id = atoi(aux);

	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = (float)atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	aux = strtok(NULL, sep);
	m1.serie = aux[0];

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

void afisareListaMasini(Nod* v) {
	while (v != NULL) {
		afisareMasina(v->info);
		v = v->urm;
	}
}

void adaugaMasinaInLista(Nod* v, Masina masinaNoua) {
	if (v == NULL)
		return;

	while (v->urm != NULL) {
		v = v->urm;
	}

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->urm = NULL;
	v->urm = nou;
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.v = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.v[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* nume, int dimensiune) {
	if (dimensiune == 0)
		return 0;
	int s = 0;
	for (int i = 0; i < strlen(nume); i++) {
		s += nume[i];
	}
	return s % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int poz = calculeazaHash(masina.numeSofer, hash.dim);

	if (hash.v[poz] == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masina;
		nou->urm = NULL;
		hash.v[poz] = nou;
	}
	else {
		adaugaMasinaInLista(hash.v[poz], masina);
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		printf("Eroare la deschiderea fisierului.\n");
		exit(1);
	}

	HashTable ht = initializareHashTable(dimensiune);

	while (!feof(file)) {
		char peek = fgetc(file);
		if (peek == EOF) break;
		ungetc(peek, file);
		Masina m = citireMasinaDinFisier(file);
		inserareMasinaInTabela(ht, m);
	}

	fclose(file);
	return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("Cluster %d\n", i + 1);
		afisareListaMasini(ht.v[i]);
	}
}


void dezalocareTabelaDeMasini(HashTable* ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	return NULL;
}

Masina getMasinaDupaCheie(HashTable ht, const char* nume) {
	Masina m;
	m.id = -1;

	int hash = calculeazaHash(nume, ht.dim);
	if (ht.v[hash]) {
		Nod* p = ht.v[hash];
		while (p && strcmp(nume, p->info.numeSofer) != 0) {
			p = p->urm;
		}
		if (p) {
			m = p->info;

			// alocare și copiere model
			m.model = (char*)malloc(strlen(p->info.model) + 1);
			strcpy(m.model, p->info.model);

			// alocare și copiere nume sofer
			m.numeSofer = (char*)malloc(strlen(p->info.numeSofer) + 1);
			strcpy(m.numeSofer, p->info.numeSofer);
		}
	}
	return m;
}


int main(){
	HashTable ht = citireMasiniDinFisier("Masini.txt", 10);
	afisareTabelaDeMasini(ht);
	Masina m = getMasinaDupaCheie(ht, "Gigel");
	afisareMasina(m);
	printf("Cautam masina lui Gigel \n");
	if (m.id != 1) {
		afisareMasina(m);
		free(m.model);
		free(m.numeSofer);
	}
	else {
		printf("Nu a fost gasit\n");
	}
	return 0;
}