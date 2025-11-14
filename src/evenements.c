#include "evenements.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ajouter_evenement() {
    print_info("Module evenements - Fonction ajouter_evenement appelée");
}

void modifier_evenement(int id) {
    print_info("Module evenements - Fonction modifier_evenement appelée");
    (void)id;
}

void supprimer_evenement(int id) {
    Evenement* arr = NULL;
    int count = charger_evenements(&arr);
    if (count == 0 || arr == NULL) return;
    
    FILE* f = fopen("data/evenements.txt", "w");
    if (!f) { free(arr); return; }
    
    for (int i = 0; i < count; i++) {
        if (arr[i].id != id) {
            fprintf(f, "%d,%s,%s,%s,%d,%d,%d\n",
                arr[i].id, arr[i].nom, arr[i].description, arr[i].date,
                arr[i].centreId, arr[i].capaciteMax, arr[i].inscrits);
        }
    }
    fclose(f);
    free(arr);
    print_info("Événement supprimé");
}

Evenement* rechercher_evenement(int id) {
    Evenement* arr = NULL;
    int count = charger_evenements(&arr);
    for (int i = 0; i < count; i++) {
        if (arr[i].id == id) {
            Evenement* result = malloc(sizeof(Evenement));
            if (result) memcpy(result, &arr[i], sizeof(Evenement));
            free(arr);
            return result;
        }
    }
    free(arr);
    return NULL;
}

void afficher_tous_evenements() {
    Evenement* arr = NULL;
    int count = charger_evenements(&arr);
    for (int i = 0; i < count; i++) {
        printf("%d | %s | %s | Inscrits: %d/%d\n",
            arr[i].id, arr[i].nom, arr[i].date,
            arr[i].inscrits, arr[i].capaciteMax);
    }
    free(arr);
}

void sauvegarder_evenements(Evenement* data, int count) {
    FILE* f = fopen("data/evenements.txt", "w");
    if (!f) return;
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d,%s,%s,%s,%d,%d,%d\n",
            data[i].id, data[i].nom, data[i].description, data[i].date,
            data[i].centreId, data[i].capaciteMax, data[i].inscrits);
    }
    fclose(f);
}

int charger_evenements(Evenement** data) {
    FILE* f = fopen("data/evenements.txt", "r");
    if (!f) { *data = NULL; return 0; }
    
    int count = 0;
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 1) count++;
    }
    if (count == 0) { fclose(f); *data = NULL; return 0; }
    
    Evenement* arr = malloc(sizeof(Evenement) * count);
    if (!arr) { fclose(f); *data = NULL; return 0; }
    
    rewind(f);
    int i = 0;
    while (i < count && fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 1) {
            sscanf(buffer, "%d,%99[^,],%199[^,],%10[^,],%d,%d,%d",
                &arr[i].id, arr[i].nom, arr[i].description, arr[i].date,
                &arr[i].centreId, &arr[i].capaciteMax, &arr[i].inscrits);
            i++;
        }
    }
    fclose(f);
    *data = arr;
    return count;
}

int compte_evenements_a_venir() {
    Evenement* arr = NULL;
    int count = charger_evenements(&arr);
    free(arr);
    return count;
}

int get_taux_inscription_moyen() {
    Evenement* arr = NULL;
    int count = charger_evenements(&arr);
    if (count == 0) return 0;
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (arr[i].capaciteMax > 0) {
            total += (arr[i].inscrits * 100) / arr[i].capaciteMax;
        }
    }
    free(arr);
    return total / count;
}
