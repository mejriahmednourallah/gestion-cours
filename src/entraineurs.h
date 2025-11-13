#ifndef ENTRAINEURS_H
#define ENTRAINEURS_H

// Trainer structure
typedef struct {
    int id;
    char nom[50];
    char prenom[50];
    char specialite[50];
    char telephone[20];
    int disponible;  // 1=available, 0=busy
    int centreId;    // associated center
} Entraineur;

// CRUD Operations
void ajouter_entraineur();
void modifier_entraineur(int id);
void supprimer_entraineur(int id);
Entraineur* rechercher_entraineur(int id);
void afficher_tous_entraineurs();

// File Operations
void sauvegarder_entraineurs(Entraineur* data, int count);
int charger_entraineurs(Entraineur** data);

// Statistics
int compte_entraineurs_disponibles();
int compte_entraineurs_par_centre(int centreId);

#endif // ENTRAINEURS_H
