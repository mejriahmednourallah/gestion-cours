# 📋 GUIDE DE COMPILATION ET UTILISATION

## Installation des Dépendances

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y build-essential libgtk-3-dev pkg-config
```

### CentOS/RHEL
```bash
sudo yum groupinstall "Development Tools"
sudo yum install gtk3-devel pkg-config
```

### Fedora
```bash
sudo dnf install gcc gtk3-devel pkg-config
```

## Compilation

### Mode complet (avec GTK3)
```bash
cd /workspaces/gestion-cours
make clean
make
```

### Mode test (sans GTK3)
```bash
make run  # Détecte automatiquement et compile en mode test
```

## Exécution

```bash
# Mode complet (une fois les dépendances installées)
./app

# Mode test
./app-test
```

## Architecture de l'Application

### Structure des Répertoires
```
gestion-cours/
├── src/
│   ├── main.c              # Point d'entrée GTK3 + Authentification
│   ├── utils.c/utils.h     # Utilitaires (100% complet)
│   ├── membres.c/h         # Gestion des membres
│   ├── centres.c/h         # Gestion des centres
│   ├── cours.c/h           # Gestion des cours
│   ├── entraineurs.c/h     # Gestion des entraîneurs
│   ├── equipements.c/h     # Gestion des équipements
│   ├── evenements.c/h      # Gestion des événements
│   └── test_main.c         # Version test sans GTK
├── ui/
│   └── main.glade          # Interface GTK3 (Glade)
├── data/
│   ├── membres.txt         # Stockage membres
│   ├── centres.txt         # Stockage centres
│   ├── cours.txt           # Stockage cours
│   ├── entraineurs.txt     # Stockage entraîneurs
│   ├── equipements.txt     # Stockage équipements
│   └── evenements.txt      # Stockage événements
├── Makefile                # Compilation intelligente
└── README.md               # Documentation

```

## Système d'Authentification

### Identifiants de Démonstration
```
Rôle           Utilisateur  Mot de passe
─────────────────────────────────────────
Administrateur  user         pass
Membre          member       member
Entraîneur      trainer      trainer
```

### Fonctionnalités par Rôle

#### Administrateur
- Accès complet à tous les modules
- Gestion complète des données (CRUD)
- Vue sur toutes les statistiques
- Gestion des utilisateurs (future)

#### Membre
- Consultation des cours disponibles
- Inscription/désinscription des cours
- Consultation des événements
- Vue de leur profil

#### Entraîneur
- Gestion de ses cours
- Réservation d'équipements
- Vue des membres inscrits
- Gestion de sa disponibilité

## Modules Implémentés

### 1. Gestion des Membres
**Fichier:** `data/membres.txt`

Structure CSV:
```
ID,Nom,Prénom,Email,Téléphone,Date Inscription,Actif
1,Doe,John,john@example.com,0123456789,2024-01-15,1
2,Smith,Jane,jane@example.com,0987654321,2024-02-20,1
```

**Fonctionnalités:**
- ✅ Ajouter/Modifier/Supprimer
- ✅ Recherche par nom/email/ID
- ✅ Filtrer actifs/inactifs
- ✅ Statistiques (total, actifs, nouveaux)

### 2. Gestion des Centres
**Fichier:** `data/centres.txt`

Structure CSV:
```
ID,Nom,Adresse,Téléphone,Capacité
1,Centre Fitness Paris,123 Rue de Paris,0123456789,500
```

**Fonctionnalités:**
- ✅ CRUD complet
- ✅ Gestion de la capacité
- ✅ Statistiques de capacité

### 3. Gestion des Cours
**Fichier:** `data/cours.txt`

Structure CSV:
```
ID,Nom,Type,Horaire,EntraineurID,CapaciteMax,Inscrits,CentreID
1,Yoga Lundi,Yoga,Lundi 10:00,1,20,15,1
```

**Fonctionnalités:**
- ✅ Planification des cours
- ✅ Gestion des inscriptions
- ✅ Vérification de capacité
- ✅ Statistiques (populaires, taux utilisation)

### 4. Gestion des Entraîneurs
**Fichier:** `data/entraineurs.txt`

Structure CSV:
```
ID,Nom,Prénom,Spécialité,Téléphone,Disponible,CentreID
1,Martin,Pierre,Yoga,0612345678,1,1
```

**Fonctionnalités:**
- ✅ Gestion du personnel
- ✅ Disponibilité
- ✅ Assignation aux centres
- ✅ Statistiques

### 5. Gestion des Équipements
**Fichier:** `data/equipements.txt`

Structure CSV:
```
ID,Nom,Type,Quantité,Disponible,CentreID
1,Tapis de Yoga,Yoga,50,1,1
```

**Fonctionnalités:**
- ✅ Inventaire
- ✅ Disponibilité
- ✅ Réservation
- ✅ Statistiques par centre

### 6. Gestion des Événements
**Fichier:** `data/evenements.txt`

Structure CSV:
```
ID,Nom,Description,Date,CentreID,CapaciteMax,Inscrits
1,Marathon 2024,Course de 10km,2024-05-15,1,100,45
```

**Fonctionnalités:**
- ✅ Planification d'événements
- ✅ Gestion des inscriptions
- ✅ Filtrage par date
- ✅ Statistiques

## Interface Glade

### Fenêtres Principales

#### 1. Fenêtre de Connexion (login_window)
- Sélection de rôle (ComboBox)
- Champ utilisateur (Entry)
- Champ mot de passe (Entry - caché)
- Boutons: Se Connecter / Quitter

#### 2. Fenêtre Principale (main_window)
- **Menubar:**
  - Fichier → Déconnexion / Quitter
  - Modules → 6 modules CRUD
  - Aide → À propos

- **Header Bar:**
  - Affiche l'utilisateur actuel
  - Affiche le rôle actuel

- **Notebook (6 onglets):**
  1. **Accueil:** Bienvenue et instructions
  2. **Membres:** 
     - TreeView avec colonnes (ID, Nom, Prénom, Email, Téléphone, Date, Actif)
     - Barre de recherche
     - Boutons: Ajouter, Modifier, Supprimer, Rafraîchir
  3. **Centres:** Tableau CRUD
  4. **Cours:** Tableau CRUD
  5. **Entraîneurs:** Tableau CRUD
  6. **Équipements:** Tableau CRUD
  7. **Événements:** Tableau CRUD

- **Status Bar:** Affichage des messages

## Utilitaires Disponibles (utils.c)

### Gestion des Dates
- `get_current_date()` - Retourne la date actuelle (YYYY-MM-DD)
- `is_valid_date(date)` - Valide le format et la validité
- `date_compare(date1, date2)` - Compare deux dates

### Gestion des Fichiers
- `file_exists(filename)` - Vérifie l'existence
- `read_line(file, buffer, size)` - Lit une ligne
- `create_data_directory()` - Crée le répertoire data/

### Validation
- `is_valid_email(email)` - Valide format email
- `is_valid_phone(phone)` - Valide numéro (chiffres)
- `is_valid_date_format(date)` - Valide format YYYY-MM-DD

### Gestion des IDs
- `generate_next_id(filename)` - Génère ID auto-incrémenté
- `id_exists(filename, id, column)` - Vérifie unicité ID

### Utilitaires de Chaînes
- `trim_whitespace(str)` - Supprime espaces
- `to_lowercase(str)` - Convertit en minuscules
- `string_contains(str, substr)` - Cherche substring

### Messages
- `print_error(message)` - Affiche erreur
- `print_success(message)` - Affiche succès
- `print_info(message)` - Affiche info

## Compilation Manuelle (si besoin)

```bash
# Compilation individuelle
gcc -c `pkg-config --cflags gtk+-3.0` src/utils.c -o src/utils.o
gcc -c `pkg-config --cflags gtk+-3.0` src/membres.c -o src/membres.o
gcc -c `pkg-config --cflags gtk+-3.0` src/main.c -o src/main.o

# Linking
gcc src/utils.o src/membres.o src/main.o \
    `pkg-config --libs gtk+-3.0` -o app
```

## Dépannage

### Erreur: "GTK3 not found"
```bash
sudo apt-get install libgtk-3-dev
```

### Erreur: "pkg-config not found"
```bash
sudo apt-get install pkg-config
```

### Erreur de compilation
1. Vérifiez les dépendances
2. Nettoyez: `make clean`
3. Recompilez: `make`

### Problème de fichiers de données
- Les fichiers sont créés automatiquement dans `data/`
- Permissions: vérifiez les droits de lecture/écriture

## Roadmap Futures

- [ ] Implémentation complète des modules CRUD
- [ ] Connecteur base de données (SQLite)
- [ ] Export PDF/Excel
- [ ] Système de notifications
- [ ] API REST
- [ ] Application mobile (Flutter)
- [ ] Tests unitaires
- [ ] Gestion complète des rôles

## Standards de Codage

- **Noms:** Significatifs en français/anglais
- **Commentaires:** Pour logique complexe
- **Erreurs:** Gestion robuste I/O
- **Mémoire:** Libération systématique
- **Modularité:** Chaque fonction = 1 responsabilité
- **Validation:** Toutes les entrées

## Support et Contribution

Pour contribuer:
1. Créez une branche
2. Implémentez vos changements
3. Testez complètement
4. Soumettez une PR

## Licence

MIT

---

**Version:** 1.0
**Date:** 13 Novembre 2024
**Auteur:** Projet GTK3 Gestion Centres Sportifs
