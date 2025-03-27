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

//creare structura pentru un nod dintr-o lista dublu inlantuita
typedef struct nodld{
    Masina info;
    struct nodld* urm;
    struct nodld* prec;
}nodld;

//creare structura pentru Lista Dubla 
typedef struct Lista{
    nodld* prim;
    nodld* ultim;
}Lista;

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
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
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

void afisareListaMasini(Lista list) {
    //pun -> doar daca am pointer
	nodld* p = list.prim;
    while(p){
        afisareMasina(p->info);
        p = p->urm;
    }
}


//nu trebuia sa pun pointer pt ca nu modific lista in sine
//recomandat sa pun un pointer constant daca vreau sa folosesc
void adaugaMasinaInListaSF(Lista* list, Masina masinaNoua) {
    // la orice inserare creez nodul
    nodld* inserat = (nodld*)malloc(sizeof(nodld));
    inserat->info = masinaNoua; // shallow
    inserat->urm = NULL;
    if(list->ultim){
        list->ultim->urm = inserat;
    }else{
        list->prim = inserat;
    }
    list->ultim = inserat;
}

void afisareListaMasiniSf(Lista list) {
	nodld* p = list.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->prec;
	}
}

void adaugaLaInceputInLista(Lista* list, Masina masinaNoua) {
    nodld* p = (nodld*)malloc(sizeof(nodld));
    p->info = masinaNoua;
    p->urm = list->prim;
    p->prec = NULL;
    if(list->prim){
        list->prim->prec = p;
    }else{
	    list->ultim = p;
	}
	list->prim = p;
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
    Lista list;
    list.prim = NULL;
    list.ultim = NULL;
    while(!feof(f)){
        //adaugaMasinaInListaSF(&list, citireMasinaDinFisier(f));
        adaugaLaInceputInLista(&list, citireMasinaDinFisier(f));
    }
    fclose(f);
    return list;
}

void dezalocareLDMasini(Lista* list) {
    nodld* p = list->prim;
    while(p->urm != NULL){
        free(p->info.model);
        free(p->info.numeSofer);
        p = p->urm;
        free(p->prec);
    }
    free(p->info.model);
    free(p->info.numeSofer);
    free(p);
}

float calculeazaPretMediu(Lista list) {
	nodld* aux = list.prim;
    float suma = 0;
    float cnt = 0;
    while(aux){
        suma+= aux->info.pret;
        cnt++;
        aux = aux->urm;
    }
    return suma/cnt;
}

void stergeMasinaDupaID(Lista* list, int id) {
    nodld* temp = list->prim;
    while(temp){
        if(temp->info.id == id){
            //caz 1: sterg capul
            if(temp == list->prim){
                list->prim = temp->urm;
                list->prim->prec = NULL;
            //caz 2: sterg coada
            }else if(temp == list->ultim){
                list->ultim = temp->prec;
                list->ultim->urm = NULL;
            //caz 3: sterg alt nod
            }else{
                temp->prec->urm = temp->urm;
                temp->urm->prec = temp->prec;
            }
            free(temp->info.model);
            free(temp->info.numeSofer);
            free(temp);
        }
        temp = temp->urm;
    }
}

char* getNumeSoferMasinaScumpa(Lista list) {
    nodld* temp = list.prim;
    nodld* max = temp;
    while(temp->urm != NULL){
        if(temp->info.pret > max->info.pret){
            max = temp;
        }
        temp = temp->urm;
    }
    return max->info.numeSofer;
}

int main() {
    Lista list;
    list = citireLDMasiniDinFisier("Masini.txt");
    afisareListaMasiniSf(list);

    float pretMediu = calculeazaPretMediu(list);
    printf("%0.2f Medie", pretMediu);

    char* soferMasinaScumpa = getNumeSoferMasinaScumpa(list);
    printf("\nNume sofer masina scumpa: %s\n", soferMasinaScumpa);

    printf("\n***VERIFICARE CAZURI***\n");
    //stergeMasinaDupaID(&list, 1);
    //stergeMasinaDupaID(&list, 3);
    stergeMasinaDupaID(&list, 6);
    afisareListaMasiniSf(list);

    dezalocareLDMasini(&list);
	return 0;
}