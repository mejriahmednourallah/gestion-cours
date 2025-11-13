#ifndef EQUIPEMENTS_H
#define EQUIPEMENTS_H

// Equipment structure
typedef struct {
    int id;
    char nom[50];
    char type[30];
    int quantite;
    int disponible;  // 1=available, 0=reserved
    int centreId;
} Equipement;

// CRUD Operations
void ajouter_equipement();
void modifier_equipement(int id);
void supprimer_equipement(int id);
Equipement* rechercher_equipement(int id);
void afficher_tous_equipements();

// File Operations
void sauvegarder_equipements(Equipement* data, int count);
int charger_equipements(Equipement** data);

// Statistics
int compte_equipements_disponibles();
int compte_equipements_par_centre(int centreId);

#endif // EQUIPEMENTS_H
