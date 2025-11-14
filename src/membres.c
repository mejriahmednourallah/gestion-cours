#include "membres.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// CRUD Operations
void ajouter_membre() {
    print_info("Module membres - Fonction ajouter_membre appelée");
}

void modifier_membre(int id) {
    print_info("Module membres - Fonction modifier_membre appelée");
    (void)id;
}

void supprimer_membre(int id) {
    Membre* membres = NULL;
    int count = charger_membres(&membres);
    
    if (count == 0 || membres == NULL) {
        print_error("Impossible de charger les membres");
        return;
    }
    
    FILE* f = fopen("data/membres.txt", "w");
    if (!f) {
        print_error("Impossible d'ouvrir le fichier membres.txt");
        free(membres);
        return;
    }
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (membres[i].id != id) {
            fprintf(f, "%d,%s,%s,%s,%s,%s,%d\n",
                    membres[i].id,
                    membres[i].nom,
                    membres[i].prenom,
                    membres[i].email,
                    membres[i].telephone,
                    membres[i].dateInscription,
                    membres[i].actif);
        } else {
            found = 1;
        }
    }
    
    fclose(f);
    free(membres);
    
    if (found) {
        print_info("Membre supprimé avec succès");
    } else {
        print_warning("Membre non trouvé");
    }
}

Membre* rechercher_membre(int id) {
    Membre* membres = NULL;
    int count = charger_membres(&membres);
    
    if (count == 0 || membres == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < count; i++) {
        if (membres[i].id == id) {
            Membre* result = malloc(sizeof(Membre));
            if (result) {
                memcpy(result, &membres[i], sizeof(Membre));
            }
            free(membres);
            return result;
        }
    }
    
    free(membres);
    return NULL;
}

void afficher_tous_membres() {
    Membre* membres = NULL;
    int count = charger_membres(&membres);
    
    printf("\n=== Liste des Membres (%d) ===\n", count);
    for (int i = 0; i < count; i++) {
        printf("%d | %s %s | %s | %s | %s | %s\n",
               membres[i].id,
               membres[i].prenom,
               membres[i].nom,
               membres[i].email,
               membres[i].telephone,
               membres[i].dateInscription,
               membres[i].actif ? "Actif" : "Inactif");
    }
    
    free(membres);
}

// File Operations
void sauvegarder_membres(Membre* data, int count) {
    FILE* f = fopen("data/membres.txt", "w");
    if (!f) {
        print_error("Impossible d'ouvrir le fichier membres.txt");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d,%s,%s,%s,%s,%s,%d\n",
                data[i].id,
                data[i].nom,
                data[i].prenom,
                data[i].email,
                data[i].telephone,
                data[i].dateInscription,
                data[i].actif);
    }
    
    fclose(f);
    print_info("Membres sauvegardés avec succès");
}

int charger_membres(Membre** data) {
    FILE* f = fopen("data/membres.txt", "r");
    if (!f) {
        print_warning("Fichier membres.txt non trouvé");
        *data = NULL;
        return 0;
    }
    
    // Compter les lignes
    int count = 0;
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 1) {
            count++;
        }
    }
    
    if (count == 0) {
        fclose(f);
        *data = NULL;
        return 0;
    }
    
    // Allouer la mémoire
    Membre* membres = malloc(sizeof(Membre) * count);
    if (!membres) {
        fclose(f);
        *data = NULL;
        return 0;
    }
    
    // Relire depuis le début
    rewind(f);
    int i = 0;
    while (i < count && fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 1) {
            sscanf(buffer, "%d,%49[^,],%49[^,],%99[^,],%19[^,],%10[^,],%d",
                   &membres[i].id,
                   membres[i].nom,
                   membres[i].prenom,
                   membres[i].email,
                   membres[i].telephone,
                   membres[i].dateInscription,
                   &membres[i].actif);
            i++;
        }
    }
    
    fclose(f);
    *data = membres;
    return count;
}

// Statistics
int compte_membres_actifs() {
    Membre* membres = NULL;
    int count = charger_membres(&membres);
    int actifs = 0;
    
    for (int i = 0; i < count; i++) {
        if (membres[i].actif) {
            actifs++;
        }
    }
    
    free(membres);
    return actifs;
}

int compte_membres_total() {
    Membre* membres = NULL;
    int count = charger_membres(&membres);
    free(membres);
    return count;
}

char* get_date_inscription_mois_actuel() {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    static char date[11];
    snprintf(date, sizeof(date), "%04d-%02d-01", t->tm_year + 1900, t->tm_mon + 1);
    return date;
}
