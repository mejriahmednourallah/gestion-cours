#ifndef EVENEMENTS_H
#define EVENEMENTS_H

// Event structure
typedef struct {
    int id;
    char nom[100];
    char description[200];
    char date[11];      // YYYY-MM-DD
    int centreId;
    int capaciteMax;
    int inscrits;
} Evenement;

// CRUD Operations
void ajouter_evenement();
void modifier_evenement(int id);
void supprimer_evenement(int id);
Evenement* rechercher_evenement(int id);
void afficher_tous_evenements();

// File Operations
void sauvegarder_evenements(Evenement* data, int count);
int charger_evenements(Evenement** data);

// Statistics
int compte_evenements_a_venir();
int get_taux_inscription_moyen();

#endif // EVENEMENTS_H
