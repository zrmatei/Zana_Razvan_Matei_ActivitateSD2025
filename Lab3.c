#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	//afiseaza toate atributele unei masini
	printf("ID:%d, \n", masina.id);
	printf("Numar de usi : %d, \n ", masina.nrUsi);
	printf(" Pret:%5.2f, \n", masina.pret);
	printf(" Nume Sofer:%s\n, ", masina.numeSofer);
	printf("Serie:%c, \n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0;i < nrMasini;i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0;i < (*nrMasini);i++) {
		aux[i] = (*masini)[i];
	}
	aux[*nrMasini] = masinaNoua;
	(*nrMasini)++;
	free(*masini);
	(*masini) = aux;
}

Masina citireMasinaFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	fgets(buffer,100,file);
	Masina m1;
	aux = strtok(buffer,sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL,sep));
	m1.pret = atof(strtok(NULL,sep));
	aux = strtok(NULL,sep);
	m1.model = malloc(strlen(aux)+1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL,sep);
	return m1;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file = fopen(numeFisier,"r");
	Masina* masini = NULL;
	do {
		adaugaMasinaInVector(&masini,nrMasiniCitite,citireMasinaFisier(file));
	} while (!feof(file));
	fclose(file);
	return masini;
}

float pretMediuDupaNrUsi(Masina* vector, int nrMasini, int nrUsi) {
	float suma = 0;
	float index = 0;
	for (int i = 0;i < nrMasini;i++) {
		if (vector[i].nrUsi == nrUsi) {
			suma += vector[i].pret;
			index++;
		}
	}
	if (index > 0) {
		return suma / index;
	}
	return 0;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0;i < *nrMasini;i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	*vector = NULL;
	*nrMasini = 0;
}

int main() {
	Masina* masini = NULL;
	int nrMasini = 0;
	masini = citireVectorMasiniFisier("Masini.txt",&nrMasini);
	afisareVectorMasini(masini, nrMasini);
	float medie = pretMediuDupaNrUsi(masini, nrMasini, 6);\
		printf("Media masinilor este:%.2f ",medie);
	dezalocareVectorMasini(&masini, &nrMasini);
	return 0;
}