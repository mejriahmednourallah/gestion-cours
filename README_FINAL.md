# 🏋️ Application de Gestion des Salles de Sport

## 📋 Description

Application desktop de gestion complète pour centres sportifs développée en C avec interface graphique GTK+ 3.0. Cette application permet de gérer les membres, entraîneurs, cours, équipements, centres et événements de manière centralisée et intuitive.

## ✨ Fonctionnalités

### 👤 Système d'Authentification
- 3 rôles: **Administrateur**, **Membre**, **Entraîneur**
- Interface de login sécurisée
- Gestion des permissions par rôle

### 📊 Tableau de Bord
- Statistiques en temps réel
- Vue d'ensemble des activités
- Cartes colorées pour chaque module

### 🎯 Modules de Gestion

#### 1. **Gestion des Membres** 👥
**Administrateur:**
- Ajouter, modifier, supprimer des membres
- Rechercher et filtrer membres
- Voir statistiques d'adhésion

**Membre:**
- Demander un coach privé
- S'inscrire aux cours
- Participer aux événements

#### 2. **Gestion des Entraîneurs** 🏋️
**Administrateur:**
- Gérer les entraîneurs
- Assigner aux centres
- Suivre les spécialités

**Entraîneur:**
- S'inscrire à un cours comme coach
- S'inscrire à un centre
- Réserver des équipements

#### 3. **Gestion des Cours Sportifs** 📚
**Administrateur:**
- Créer et planifier des cours
- Assigner des entraîneurs
- Gérer les capacités

**Membre:**
- S'inscrire aux cours disponibles
- Voir les horaires
- Consulter les places disponibles

#### 4. **Gestion des Équipements** 🎽
**Administrateur:**
- Gérer l'inventaire
- Suivre l'état des équipements
- Répartir par centre

**Entraîneur:**
- Réserver équipements pour cours
- Vérifier disponibilité

#### 5. **Gestion des Centres** 🏢
**Administrateur:**
- Créer et gérer centres
- Définir capacités
- Assigner ressources

**Entraîneur:**
- S'inscrire à un centre
- Voir informations centre

#### 6. **Gestion des Événements** 📅
**Administrateur:**
- Organiser événements
- Gérer inscriptions
- Suivre participation

**Membre:**
- S'inscrire aux événements
- Voir calendrier

## 🏗️ Architecture

```
gestion-cours/
├── src/
│   ├── main.c              # Point d'entrée et authentification
│   ├── membres.c/h         # Module membres
│   ├── entraineurs.c/h     # Module entraîneurs
│   ├── cours.c/h           # Module cours
│   ├── equipements.c/h     # Module équipements
│   ├── centres.c/h         # Module centres
│   ├── evenements.c/h      # Module événements
│   └── utils.c/h           # Fonctions utilitaires
├── ui/
│   ├── main.glade          # Interface principale
│   ├── membres.glade       # Interface module membres
│   ├── entraineurs.glade   # Interface module entraîneurs
│   ├── cours.glade         # Interface module cours
│   ├── equipements.glade   # Interface module équipements
│   ├── centres.glade       # Interface module centres
│   └── evenements.glade    # Interface module événements
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

## 🚀 Installation et Compilation

### Prérequis

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential libgtk-3-dev pkg-config

# Fedora/RHEL
sudo dnf install gcc make gtk3-devel pkg-config

# Arch Linux
sudo pacman -S base-devel gtk3 pkg-config
```

### Compilation

```bash
# Cloner le projet
git clone https://github.com/mejriahmednourallah/gestion-cours.git
cd gestion-cours

# Compiler
make clean
make

# Lancer
./app
```

## 🔐 Identifiants de Test

| Rôle | Username | Password | Accès |
|------|----------|----------|-------|
| **Administrateur** | `user` | `pass` | Accès complet à tous les modules |
| **Membre** | `member` | `member` | Inscription cours/événements, demande coach |
| **Entraîneur** | `trainer` | `trainer` | Gestion cours, réservation équipements |

## 💾 Données de Test

### Membres (15)
- Jean Dupont, Marie Martin, Pierre Bernard, Sophie Durand, Lucas Lefebvre
- Emma Moreau, Thomas Simon, Julie Laurent, Antoine Petit, Laura Garcia
- Et 5 autres...

### Entraîneurs (8)
- Michel Dupont (Yoga), Sophie Martin (Cardio), Luc Bernard (Musculation)
- Claire Leroy (Natation), Marc Bonnet (Pilates), Julie François (CrossFit)
- Paul Girard (Boxe), Alice Lambert (Danse)

### Centres (5)
- Centre Fitness Paris, Centre Yoga Marseille, Centre Musculation Lyon
- Centre Aquatique Nice, Centre Sport Toulouse

### Cours (10)
- Yoga Débutant, Cardio Intense, Musculation Force
- Natation Adultes, Pilates Matinal, CrossFit Challenge
- Boxe Cardio, Danse Zumba, Yoga Avancé, Cardio Débutant

### Équipements (10)
- Tapis de Yoga, Altères, Rameur, Vélo Statique
- Ballon Pilates, Kettlebell, Gants de Boxe, Tapis de Course
- Banc de Musculation, Corde à Sauter

### Événements (6)
- Marathon Paris 2024, Compétition Natation, Triathlon d'Été
- Challenge CrossFit, Tournoi de Boxe, Gala de Danse

## 🎨 Captures d'écran

### Écran de Connexion
Interface simple avec sélection de rôle, username et password

### Tableau de Bord
Vue d'ensemble avec statistiques colorées pour chaque module

### Modules de Gestion
Chaque module s'ouvre dans une fenêtre dédiée avec:
- Barre de recherche
- Boutons d'action (Ajouter, Modifier, Supprimer, Actualiser)
- Tableau de données avec tri
- Statistiques en bas

## 🛠️ Technologies

- **Langage**: C (standard C99)
- **Interface**: GTK+ 3.0
- **Designer UI**: Glade
- **Build**: Make
- **Système de fichiers**: Fichiers texte (.txt)

## 📝 Utilisation

### 1. Connexion
1. Lancer `./app`
2. Sélectionner un rôle (Membre présélectionné)
3. Entrer identifiants
4. Cliquer sur "Connexion"

### 2. Navigation
- Utiliser le menu **"Modules"** pour accéder aux différentes sections
- Chaque module s'ouvre dans sa propre fenêtre
- Menu **"Fichier"** pour se déconnecter ou quitter

### 3. Gestion des Données
- **Ajouter**: Cliquer sur ➕ Ajouter
- **Modifier**: Sélectionner une ligne, cliquer sur ✏️ Modifier
- **Supprimer**: Sélectionner une ligne, cliquer sur 🗑️ Supprimer
- **Rechercher**: Utiliser la barre de recherche
- **Actualiser**: Cliquer sur 🔄 pour recharger

## 👥 Équipe de Développement

Projet réalisé dans le cadre du cours de Programmation C:
- **Module Membres**: Étudiant 1
- **Module Entraîneurs**: Étudiant 2
- **Module Cours**: Étudiant 3
- **Module Équipements**: Étudiant 4
- **Module Centres**: Étudiant 5
- **Module Événements**: Étudiant 6

## 📄 Licence

Projet académique - 2024-2025

## 🐛 Dépannage

### Erreur GLIBC
Si vous voyez `GLIBC_2.34 not found`, recompilez sur votre système:
```bash
make clean
make
```

### Erreur GTK
Si GTK n'est pas trouvé:
```bash
sudo apt-get install libgtk-3-dev pkg-config
```

### Cannot open display
Normal dans un environnement sans X11 (conteneurs, SSH sans X forwarding).
Testez sur une machine avec interface graphique.

## 📞 Support

Pour toute question ou problème:
1. Vérifier la documentation
2. Consulter les logs en console
3. Recompiler avec `make clean && make`

---

**Sport Club Premium** - Shape Your Body, Gym Sport Club Personal Trainer Fitness Club Premium 🏋️‍♀️💪
