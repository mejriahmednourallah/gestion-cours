#include "equipements.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ajouter_equipement() {
    print_info("Module equipements - Fonction ajouter_equipement appelée");
}

void modifier_equipement(int id) {
    print_info("Module equipements - Fonction modifier_equipement appelée");
    (void)id;
}

void supprimer_equipement(int id) {
    Equipement* arr = NULL;
    int count = charger_equipements(&arr);
    if (count == 0 || arr == NULL) return;
    
    FILE* f = fopen("data/equipements.txt", "w");
    if (!f) { free(arr); return; }
    
    for (int i = 0; i < count; i++) {
        if (arr[i].id != id) {
            fprintf(f, "%d,%s,%s,%d,%d,%d\n",
                arr[i].id, arr[i].nom, arr[i].type,
                arr[i].quantite, arr[i].disponible, arr[i].centreId);
        }
    }
    fclose(f);
    free(arr);
    print_info("Équipement supprimé");
}

Equipement* rechercher_equipement(int id) {
    Equipement* arr = NULL;
    int count = charger_equipements(&arr);
    for (int i = 0; i < count; i++) {
        if (arr[i].id == id) {
            Equipement* result = malloc(sizeof(Equipement));
            if (result) memcpy(result, &arr[i], sizeof(Equipement));
            free(arr);
            return result;
        }
    }
    free(arr);
    return NULL;
}

void afficher_tous_equipements() {
    Equipement* arr = NULL;
    int count = charger_equipements(&arr);
    for (int i = 0; i < count; i++) {
        printf("%d | %s | %s | Qté: %d | %s\n",
            arr[i].id, arr[i].nom, arr[i].type, arr[i].quantite,
            arr[i].disponible ? "Disponible" : "Réservé");
    }
    free(arr);
}

void sauvegarder_equipements(Equipement* data, int count) {
    FILE* f = fopen("data/equipements.txt", "w");
    if (!f) return;
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d,%s,%s,%d,%d,%d\n",
            data[i].id, data[i].nom, data[i].type,
            data[i].quantite, data[i].disponible, data[i].centreId);
    }
    fclose(f);
}

int charger_equipements(Equipement** data) {
    FILE* f = fopen("data/equipements.txt", "r");
    if (!f) { *data = NULL; return 0; }
    
    int count = 0;
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 1) count++;
    }
    if (count == 0) { fclose(f); *data = NULL; return 0; }
    
    Equipement* arr = malloc(sizeof(Equipement) * count);
    if (!arr) { fclose(f); *data = NULL; return 0; }
    
    rewind(f);
    int i = 0;
    while (i < count && fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 1) {
            sscanf(buffer, "%d,%49[^,],%29[^,],%d,%d,%d",
                &arr[i].id, arr[i].nom, arr[i].type,
                &arr[i].quantite, &arr[i].disponible, &arr[i].centreId);
            i++;
        }
    }
    fclose(f);
    *data = arr;
    return count;
}

int compte_equipements_disponibles() {
    Equipement* arr = NULL;
    int count = charger_equipements(&arr);
    int dispo = 0;
    for (int i = 0; i < count; i++) {
        if (arr[i].disponible) dispo++;
    }
    free(arr);
    return dispo;
}

int compte_equipements_par_centre(int centreId) {
    Equipement* arr = NULL;
    int count = charger_equipements(&arr);
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (arr[i].centreId == centreId) total++;
    }
    free(arr);
    return total;
}
