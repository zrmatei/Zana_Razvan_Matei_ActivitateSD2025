#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Masina {
    int id;
    int nrLocuri;
    char* marca;
    float capacitateC;
    char normaPoluare;
};

struct Masina initializare(int id, int nrLocuri, const char* marca, float capacitateC, char normaPoluare) {
    struct Masina m;
    m.id = id;
    m.nrLocuri = nrLocuri;
    m.capacitateC = capacitateC;
    m.normaPoluare = normaPoluare;
    
    if (marca) {
        m.marca = (char*)malloc(strlen(marca) + 1);
        if (m.marca) {
            strcpy(m.marca, marca);
        }
    } else {
        m.marca = NULL;
    }
    return m;
}

void afisare(struct Masina m) {
    printf("\nId masina: %d\nNr locuri: %d\nCapacitate: %.2f\nMarca: %s\nEuro%c\n",
           m.id, m.nrLocuri, m.capacitateC, m.marca, m.normaPoluare);
}

void afisareVector(struct Masina* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++) {
        afisare(vector[i]);
    }
}

struct Masina* copiazaDupaPrag(struct Masina* vector, int nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
        *dimensiune = 0;
        for(int i=0; i < nrElemente; i++){
            if(prag < vector[i].capacitateC){
                (*dimensiune)++;
            }
        }
        if(*dimensiune == 0){
            return NULL;
        }
        int j = 0;
        *vectorNou = (struct Masina*)malloc((*dimensiune) * sizeof(struct Masina));
            
        for(int i=0; i < nrElemente;i++){
            if(prag < vector[i].capacitateC){
                (*vectorNou)[j] = vector[i];
                (*vectorNou)[j].marca = malloc(sizeof(char) * (strlen(vector[i].marca) + 1));
                strcpy((*vectorNou)[j].marca, vector[i].marca);
                //puteam sa ma folosesc direct de functia de init pt vectorul nou, folosind vectorul vechi
                //(*vectorNou)[j] = init(vector[i].id, ...restul)
                j++;
            }
        }
        return *vectorNou;
    }


struct Masina getPrimaMasinaDupaMarca(struct Masina* vector, int nrElem, const char* conditie){
    for(int i=0; i < nrElem; i++){
        if(strcmp(conditie, vector[i].marca) == 0){
            return vector[i]; // return-ul face shallow copy
        }
    }
    return initializare(0, 0, "TEST", 0, '0');
}

void dezalocare(struct Masina** vector, int* nrElemente) {
    if (*vector) {
        for (int i = 0; i < *nrElemente; i++) {
            free((*vector)[i].marca);
        }
        free(*vector);
        *vector = NULL;
        *nrElemente = 0;
    }
}

void interschimbMasini(struct Masina* vect1, struct Masina* vect2){
    struct Masina temp = *vect1;
    *vect1 = *vect2;
    *vect2 = temp;
}

struct Masina* stergeMasina(struct Masina* vect, char* cheieMarca,int* nrElem){
    if(vect != NULL){
        for(int i=0; i < *nrElem; i++){
            if(strcmp(vect[i].marca, cheieMarca) == 0){
                free(vect[i].marca);
                for(int j=i; j < *nrElem-1; j++){
                    vect[j] = vect[j+1];
                }
                break;
            }
        }
        (*nrElem)--;
        vect = (struct Masina*)realloc(vect, (*nrElem) * sizeof(struct Masina));
    }
    return vect;
}

void afisareIdPar(struct Masina* vect, int nrElem){
    for(int i = 0; i < nrElem; i++){
        if(vect[i].id % 2 == 0){
            afisare(vect[i]);
        }
    }
}

int main() {
    int nrElem = 4;
    struct Masina* vector = (struct Masina*)malloc(nrElem * sizeof(struct Masina));;

    vector[0] = initializare(1, 4, "Audi", 1.6, '6');
    vector[1] = initializare(2, 4, "BMW", 2.0, '6');
    vector[2] = initializare(3, 4, "Mercedes", 2.5, '6');
    vector[3] = initializare(4, 5, "Ford", 3.0, '6');
    printf("\nMasinile initiale:");
    afisareVector(vector, nrElem);

    struct Masina* vectorMasiniCopiate = NULL;
    int nrElemCop = 0;
    copiazaDupaPrag(vector, nrElem, 2, &vectorMasiniCopiate, &nrElemCop);
    if (vectorMasiniCopiate) {
        printf("\nMasini peste prag:");
        afisareVector(vectorMasiniCopiate, nrElemCop);
        dezalocare(&vectorMasiniCopiate, &nrElemCop);
    } else {
        printf("\nNicio masina nu a fost copiata.");
    }

    struct Masina m1 = getPrimaMasinaDupaMarca(vector, nrElem, "Audi");
    afisare(m1);

    interschimbMasini(&vector[0], &vector[1]);
    printf("\n********INTERSCHIMBARE********");
    afisare(vector[0]);
    afisare(vector[1]);

    printf("\n********STERGERE********");
    stergeMasina(&vector[0], "Audi", &nrElem);
    afisareVector(vector, nrElem);

    printf("\n********ID-URI PARE********");
    afisareIdPar(vector, nrElem);

    dezalocare(&vector, &nrElem);
    return 0;
}