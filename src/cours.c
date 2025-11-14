#include "cours.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CRUD Operations stubs
void ajouter_cours() {
    print_info("Module cours - Fonction ajouter_cours à implémenter");
}

void modifier_cours(int id) {
    print_info("Module cours - Fonction modifier_cours à implémenter");
}

void supprimer_cours(int id) {
    print_info("Module cours - Fonction supprimer_cours à implémenter");
}

Cours* rechercher_cours(int id) {
    print_info("Module cours - Fonction rechercher_cours à implémenter");
    return NULL;
}

void afficher_tous_cours() {
    print_info("Module cours - Fonction afficher_tous_cours à implémenter");
}

// File Operations stubs
void sauvegarder_cours(Cours* data, int count) {
    print_info("Module cours - Fonction sauvegarder_cours à implémenter");
}

int charger_cours(Cours** data) {
    FILE* f = fopen("data/cours.txt", "r");
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
    
    Cours* cours = malloc(sizeof(Cours) * count);
    if (!cours) {
        fclose(f);
        *data = NULL;
        return 0;
    }
    
    rewind(f);
    int i = 0;
    while (i < count && fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 1) {
            sscanf(buffer, "%d,%49[^,],%29[^,],%19[^,],%d,%d,%d,%d",
                   &cours[i].id,
                   cours[i].nom,
                   cours[i].type,
                   cours[i].horaire,
                   &cours[i].entraineurId,
                   &cours[i].capaciteMax,
                   &cours[i].inscrits,
                   &cours[i].centreId);
            i++;
        }
    }
    
    fclose(f);
    *data = cours;
    return count;
}

// Statistics stubs
Cours* get_cours_plus_populaire(int* id_cours) {
    return NULL;
}

int get_taux_utilisation_moyen() {
    return 0;
}
