#ifndef MEMBRES_H
#define MEMBRES_H

// Member structure
typedef struct {
    int id;
    char nom[50];
    char prenom[50];
    char email[100];
    char telephone[20];
    char dateInscription[11];  // YYYY-MM-DD
    int actif;  // 1=active, 0=inactive
} Membre;

// CRUD Operations
void ajouter_membre();
void modifier_membre(int id);
void supprimer_membre(int id);
Membre* rechercher_membre(int id);
void afficher_tous_membres();

// File Operations
void sauvegarder_membres(Membre* data, int count);
int charger_membres(Membre** data);

// Statistics
int compte_membres_actifs();
int compte_membres_total();
char* get_date_inscription_mois_actuel();

#endif // MEMBRES_H
