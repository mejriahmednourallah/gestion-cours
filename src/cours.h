#ifndef COURS_H
#define COURS_H

// Course structure
typedef struct {
    int id;
    char nom[50];
    char type[30];      // Yoga, Cardio, Musculation, etc.
    char horaire[20];   // e.g., "Lundi 10:00"
    int entraineurId;
    int capaciteMax;
    int inscrits;
    int centreId;
} Cours;

// CRUD Operations
void ajouter_cours();
void modifier_cours(int id);
void supprimer_cours(int id);
Cours* rechercher_cours(int id);
void afficher_tous_cours();

// File Operations
void sauvegarder_cours(Cours* data, int count);
int charger_cours(Cours** data);

// Statistics
Cours* get_cours_plus_populaire(int* id_cours);
int get_taux_utilisation_moyen();

#endif // COURS_H
