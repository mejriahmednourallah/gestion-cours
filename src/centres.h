#ifndef CENTRES_H
#define CENTRES_H

// Center structure
typedef struct {
    int id;
    char nom[100];
    char adresse[200];
    char telephone[20];
    int capacite;
} Centre;

// CRUD Operations
void ajouter_centre();
void modifier_centre(int id);
void supprimer_centre(int id);
Centre* rechercher_centre(int id);
void afficher_tous_centres();

// File Operations
void sauvegarder_centres(Centre* data, int count);
int charger_centres(Centre** data);

// Statistics
int compte_centres_total();
int get_capacite_totale();

#endif // CENTRES_H
