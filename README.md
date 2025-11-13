# Gestion des Centres Sportifs - Application Desktop GTK3

Application de gestion complète pour les centres sportifs (salles de sport) avec interface graphique GTK3 et Glade.

## Fonctionnalités

### Modules Implémentés
1. **Gestion des Membres** - Gestion des adhérents
2. **Gestion des Entraîneurs** - Gestion du personnel d'entraînement
3. **Gestion des Cours** - Gestion des cours sportifs
4. **Gestion des Équipements** - Inventaire des équipements
5. **Gestion des Centres** - Gestion des centres sportifs
6. **Gestion des Événements** - Gestion des événements spéciaux

### Rôles Utilisateurs
- **Administrateur** - Accès complet (CRUD sur tous les modules)
- **Membre** - Inscription aux cours et événements
- **Entraîneur** - Gestion des cours et réservation d'équipements

## Structure du Projet

```
gestion-cours/
├── src/
│   ├── main.c
│   ├── membres.c / membres.h
│   ├── entraineurs.c / entraineurs.h
│   ├── cours.c / cours.h
│   ├── equipements.c / equipements.h
│   ├── centres.c / centres.h
│   ├── evenements.c / evenements.h
│   └── utils.c / utils.h
├── ui/
│   └── main.glade
├── data/
│   ├── membres.txt
│   ├── entraineurs.txt
│   ├── cours.txt
│   ├── equipements.txt
│   ├── centres.txt
│   └── evenements.txt
├── Makefile
└── README.md
```

## Prérequis

- GCC compiler
- GTK+3 development libraries
- pkg-config

### Installation des dépendances (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install libgtk-3-dev
sudo apt-get install pkg-config
```

## Compilation

```bash
# Compiler l'application
make

# Compiler et nettoyer les anciens fichiers
make clean
make

# Compiler et exécuter
make run
```

## Utilisation

### Lancer l'application
```bash
./app
```

### Authentification
1. Sélectionner le rôle utilisateur (Admin/Membre/Entraîneur)
2. Utiliser les identifiants (administrateur vide pour accès admin)

### Format des fichiers de données

Les données sont stockées en format CSV (Comma-Separated Values) dans le répertoire `data/`.

**Exemple: membres.txt**
```
1,Doe,John,john@example.com,0123456789,2024-01-15,1
2,Smith,Jane,jane@example.com,0987654321,2024-02-20,1
```

## Modules Détaillés

### Membres
- Ajouter/Modifier/Supprimer des membres
- Rechercher par ID ou nom
- Afficher tous les membres avec filtres
- Statistiques: Total des adhérents, actifs, nouvelles inscriptions

### Entraîneurs
- Ajouter/Modifier/Supprimer des entraîneurs
- Gérer la disponibilité
- Assigner à un centre
- Statistiques: Disponibilité, répartition par centre

### Cours Sportifs
- Ajouter/Modifier/Supprimer des cours
- Gérer la capacité et inscriptions
- Types: Yoga, Cardio, Musculation, etc.
- Statistiques: Cours populaires, taux d'utilisation

### Équipements
- Ajouter/Modifier/Supprimer des équipements
- Gérer la disponibilité et réservation
- Inventaire par centre
- Statistiques: Disponibilité, répartition

### Centres
- Ajouter/Modifier/Supprimer des centres
- Gérer adresse, téléphone, capacité
- Statistiques: Nombre total, capacité totale

### Événements
- Ajouter/Modifier/Supprimer des événements
- Gérer les inscriptions
- Suivi des dates
- Statistiques: Événements à venir, taux d'inscription

## Standards de Codage

- Noms de variables significatifs en anglais/français
- Commentaires pour la logique complexe
- Gestion d'erreurs appropriée (I/O, validation)
- Libération correcte de la mémoire
- Principes de programmation modulaire
- Chaque fonction avec une responsabilité unique

## Règles de Validation

- IDs uniques et auto-incrémentés
- Emails avec format valide (contient @)
- Numéros de téléphone: chiffres uniquement
- Dates au format YYYY-MM-DD
- Vérification de capacité avant inscription
- Prévention des doubles inscriptions

## Compilation et Tests

```bash
# Compile complète
make clean && make

# Exécution
./app

# Nettoyage
make clean
```

## Auteur

Projet de gestion de centres sportifs - 2024

## Licence

MIT