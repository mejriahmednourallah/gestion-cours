#include "entraineurs.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ajouter_entraineur() {
    print_info("Module entraineurs - Fonction ajouter_entraineur appelée");
}

void modifier_entraineur(int id) {
    print_info("Module entraineurs - Fonction modifier_entraineur appelée");
    (void)id;
}

void supprimer_entraineur(int id) {
    Entraineur* arr = NULL;
    int count = charger_entraineurs(&arr);
    if (count == 0 || arr == NULL) return;
    
    FILE* f = fopen("data/entraineurs.txt", "w");
    if (!f) { free(arr); return; }
    
    for (int i = 0; i < count; i++) {
        if (arr[i].id != id) {
            fprintf(f, "%d,%s,%s,%s,%s,%d,%d\n",
                arr[i].id, arr[i].nom, arr[i].prenom, arr[i].specialite,
                arr[i].telephone, arr[i].disponible, arr[i].centreId);
        }
    }
    fclose(f);
    free(arr);
    print_info("Entraîneur supprimé");
}

Entraineur* rechercher_entraineur(int id) {
    Entraineur* arr = NULL;
    int count = charger_entraineurs(&arr);
    for (int i = 0; i < count; i++) {
        if (arr[i].id == id) {
            Entraineur* result = malloc(sizeof(Entraineur));
            if (result) memcpy(result, &arr[i], sizeof(Entraineur));
            free(arr);
            return result;
        }
    }
    free(arr);
    return NULL;
}

void afficher_tous_entraineurs() {
    Entraineur* arr = NULL;
    int count = charger_entraineurs(&arr);
    for (int i = 0; i < count; i++) {
        printf("%d | %s %s | %s | %s | %s\n",
            arr[i].id, arr[i].prenom, arr[i].nom, arr[i].specialite,
            arr[i].telephone, arr[i].disponible ? "Disponible" : "Occupé");
    }
    free(arr);
}

void sauvegarder_entraineurs(Entraineur* data, int count) {
    FILE* f = fopen("data/entraineurs.txt", "w");
    if (!f) return;
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d,%s,%s,%s,%s,%d,%d\n",
            data[i].id, data[i].nom, data[i].prenom, data[i].specialite,
            data[i].telephone, data[i].disponible, data[i].centreId);
    }
    fclose(f);
}

int charger_entraineurs(Entraineur** data) {
    FILE* f = fopen("data/entraineurs.txt", "r");
    if (!f) { *data = NULL; return 0; }
    
    int count = 0;
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 1) count++;
    }
    if (count == 0) { fclose(f); *data = NULL; return 0; }
    
    Entraineur* arr = malloc(sizeof(Entraineur) * count);
    if (!arr) { fclose(f); *data = NULL; return 0; }
    
    rewind(f);
    int i = 0;
    while (i < count && fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 1) {
            sscanf(buffer, "%d,%49[^,],%49[^,],%49[^,],%19[^,],%d,%d",
                &arr[i].id, arr[i].nom, arr[i].prenom, arr[i].specialite,
                arr[i].telephone, &arr[i].disponible, &arr[i].centreId);
            i++;
        }
    }
    fclose(f);
    *data = arr;
    return count;
}

int compte_entraineurs_disponibles() {
    Entraineur* arr = NULL;
    int count = charger_entraineurs(&arr);
    int dispo = 0;
    for (int i = 0; i < count; i++) {
        if (arr[i].disponible) dispo++;
    }
    free(arr);
    return dispo;
}

int compte_entraineurs_par_centre(int centreId) {
    Entraineur* arr = NULL;
    int count = charger_entraineurs(&arr);
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (arr[i].centreId == centreId) total++;
    }
    free(arr);
    return total;
}
