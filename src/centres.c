#include "centres.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ajouter_centre() {
    print_info("Module centres - Fonction ajouter_centre appelée");
}

void modifier_centre(int id) {
    print_info("Module centres - Fonction modifier_centre appelée");
    (void)id;
}

void supprimer_centre(int id) {
    Centre* centres = NULL;
    int count = charger_centres(&centres);
    
    if (count == 0 || centres == NULL) {
        print_error("Impossible de charger les centres");
        return;
    }
    
    FILE* f = fopen("data/centres.txt", "w");
    if (!f) {
        print_error("Impossible d'ouvrir le fichier centres.txt");
        free(centres);
        return;
    }
    
    for (int i = 0; i < count; i++) {
        if (centres[i].id != id) {
            fprintf(f, "%d,%s,%s,%s,%d\n",
                    centres[i].id,
                    centres[i].nom,
                    centres[i].adresse,
                    centres[i].telephone,
                    centres[i].capacite);
        }
    }
    
    fclose(f);
    free(centres);
    print_info("Centre supprimé");
}

Centre* rechercher_centre(int id) {
    Centre* centres = NULL;
    int count = charger_centres(&centres);
    
    for (int i = 0; i < count; i++) {
        if (centres[i].id == id) {
            Centre* result = malloc(sizeof(Centre));
            if (result) {
                memcpy(result, &centres[i], sizeof(Centre));
            }
            free(centres);
            return result;
        }
    }
    
    free(centres);
    return NULL;
}

void afficher_tous_centres() {
    Centre* centres = NULL;
    int count = charger_centres(&centres);
    for (int i = 0; i < count; i++) {
        printf("%d | %s | %s | %s | %d\n",
               centres[i].id, centres[i].nom, centres[i].adresse,
               centres[i].telephone, centres[i].capacite);
    }
    free(centres);
}

void sauvegarder_centres(Centre* data, int count) {
    FILE* f = fopen("data/centres.txt", "w");
    if (!f) return;
    
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d,%s,%s,%s,%d\n",
                data[i].id, data[i].nom, data[i].adresse,
                data[i].telephone, data[i].capacite);
    }
    fclose(f);
}

int charger_centres(Centre** data) {
    FILE* f = fopen("data/centres.txt", "r");
    if (!f) {
        *data = NULL;
        return 0;
    }
    
    int count = 0;
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 1) count++;
    }
    
    if (count == 0) {
        fclose(f);
        *data = NULL;
        return 0;
    }
    
    Centre* centres = malloc(sizeof(Centre) * count);
    if (!centres) {
        fclose(f);
        *data = NULL;
        return 0;
    }
    
    rewind(f);
    int i = 0;
    while (i < count && fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 1) {
            sscanf(buffer, "%d,%99[^,],%199[^,],%19[^,],%d",
                   &centres[i].id,
                   centres[i].nom,
                   centres[i].adresse,
                   centres[i].telephone,
                   &centres[i].capacite);
            i++;
        }
    }
    
    fclose(f);
    *data = centres;
    return count;
}

int compte_centres_total() {
    Centre* centres = NULL;
    int count = charger_centres(&centres);
    free(centres);
    return count;
}

int get_capacite_totale() {
    Centre* centres = NULL;
    int count = charger_centres(&centres);
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += centres[i].capacite;
    }
    free(centres);
    return total;
}
