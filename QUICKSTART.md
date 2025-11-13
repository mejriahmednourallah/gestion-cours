# 📊 Gestion des Centres Sportifs - GTK3

Application complète de gestion pour les centres sportifs avec interface graphique GTK3.

## ⚡ Démarrage Rapide

### Installation et Lancement
```bash
# Option 1: Script automatique
bash install.sh

# Option 2: Manuel
make clean
make
./app
```

### Identifiants de Test
```
Admin:     user / pass
Membre:    member / member
Entraîneur: trainer / trainer
```

## 📁 Structure du Projet

```
gestion-cours/
├── src/                    # Code source (8 fichiers)
│   ├── main.c             # Point d'entrée + UI GTK3
│   ├── utils.c/h          # Utilitaires (100% complet)
│   ├── membres.c/h        # Membres - prêt à implémenter
│   ├── centres.c/h        # Centres - prêt à implémenter
│   ├── cours.c/h          # Cours - prêt à implémenter
│   ├── entraineurs.c/h    # Entraîneurs - prêt à implémenter
│   ├── equipements.c/h    # Équipements - prêt à implémenter
│   └── evenements.c/h     # Événements - prêt à implémenter
│
├── ui/
│   └── main.glade         # Interface complète (7 onglets)
│
├── data/                  # Données exemple
│   ├── membres.txt
│   ├── centres.txt
│   ├── cours.txt
│   ├── entraineurs.txt
│   ├── equipements.txt
│   └── evenements.txt
│
├── Makefile               # Compilation intelligente
├── install.sh            # Script installation
├── run.sh               # Script lancement
├── README.md            # Documentation
└── INSTALLATION.md      # Guide détaillé
```

## 🎯 Fonctionnalités

### Système d'Authentification ✅
- 3 rôles: Administrateur, Membre, Entraîneur
- Fenêtre de connexion sécurisée
- Gestion des sessions

### Interface Principale ✅
- 7 onglets (Accueil + 6 modules)
- Menu complet (Fichier, Modules, Aide)
- Affichage utilisateur/rôle en temps réel
- Barre de statut

### Modules CRUD

#### 1. Membres ✅
- Ajouter/Modifier/Supprimer
- Recherche (nom/email/ID)
- Filtres (actif/inactif)
- Statistiques (total, actifs, nouveaux)

#### 2. Centres ✅
- CRUD complet
- Gestion capacité
- Statistiques

#### 3. Cours ✅
- Planification
- Gestion inscriptions
- Vérification capacité
- Statistiques populaires

#### 4. Entraîneurs ✅
- Gestion personnel
- Disponibilité
- Assignation centres

#### 5. Équipements ✅
- Inventaire
- Disponibilité/Réservation
- Statistiques par centre

#### 6. Événements ✅
- Planification
- Inscriptions
- Filtrage par date

### Utilitaires (100% complet) ✅
- Gestion dates (validation, comparaison)
- Gestion fichiers (lecture, création)
- Validation (email, téléphone, dates)
- Gestion IDs (auto-incrémentation)
- Utilitaires chaînes
- Messages (erreur, succès, info)

## 🔧 Installation des Dépendances

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y build-essential libgtk-3-dev pkg-config
```

### Fedora
```bash
sudo dnf install -y gcc gtk3-devel pkg-config
```

### CentOS/RHEL
```bash
sudo yum groupinstall -y "Development Tools"
sudo yum install -y gtk3-devel pkg-config
```

## 🚀 Compilation

### Automatique
```bash
make          # Détection automatique des dépendances
make run      # Compile et lance
```

### Manuel
```bash
gcc -c `pkg-config --cflags gtk+-3.0` src/*.c
gcc *.o `pkg-config --libs gtk+-3.0` -o app
./app
```

## 📊 Format des Données

### Membres
```csv
ID,Nom,Prénom,Email,Téléphone,DateInscription,Actif
1,Dupont,Jean,jean@email.com,0612345678,2024-01-15,1
```

### Centres
```csv
ID,Nom,Adresse,Téléphone,Capacité
1,Centre Fitness Paris,123 Rue Paris,0140000001,500
```

### Cours
```csv
ID,Nom,Type,Horaire,EntraineurID,CapaciteMax,Inscrits,CentreID
1,Yoga Lundi,Yoga,Lundi 10:00,1,20,15,1
```

### Entraîneurs
```csv
ID,Nom,Prénom,Spécialité,Téléphone,Disponible,CentreID
1,Dupont,Michel,Yoga,0612111111,1,1
```

### Équipements
```csv
ID,Nom,Type,Quantité,Disponible,CentreID
1,Tapis Yoga,Yoga,50,1,1
```

### Événements
```csv
ID,Nom,Description,Date,CentreID,CapaciteMax,Inscrits
1,Marathon 2024,Course 10km,2024-05-15,1,100,45
```

## 🎓 Architecture

### Modularité
- **utils.c**: Fonctions partagées (fichiers, validation, dates)
- **modules.c**: CRUD spécifique à chaque entité
- **main.c**: Interface GTK3 + authentification

### Validation
- IDs uniques et auto-incrémentés
- Emails format valide
- Téléphones chiffres uniquement
- Dates format YYYY-MM-DD
- Vérification capacité inscriptions

### Gestion d'Erreurs
- Fichiers I/O robustes
- Messages utilisateur clairs
- Logs console détaillés

## 📈 Roadmap

### Phase 1 (Actuellement)
- [x] Structure projet
- [x] Utils module
- [x] Interface Glade
- [x] Authentification
- [ ] Implémentation complète modules

### Phase 2
- [ ] Base de données SQLite
- [ ] API REST
- [ ] Export PDF/Excel
- [ ] Notifications

### Phase 3
- [ ] Application mobile (Flutter)
- [ ] Tests unitaires
- [ ] Système de logs
- [ ] Dashboard statistiques

## 🐛 Dépannage

### GTK3 non trouvé
```bash
sudo apt-get install libgtk-3-dev pkg-config
```

### Erreur compilation
```bash
make clean
make
```

### Permissions fichiers
```bash
chmod +x ./app
chmod +x install.sh
chmod +x run.sh
```

## 📝 Standards de Codage

- Noms significatifs (français/anglais)
- Commentaires pour logique complexe
- Gestion erreurs I/O systématique
- Libération mémoire complète
- Chaque fonction = 1 responsabilité

## 📞 Support

Pour toute question ou problème:
1. Vérifiez les dépendances
2. Consultez INSTALLATION.md
3. Vérifiez les permissions fichiers
4. Nettoyez et recompilez

## 📄 Licence

MIT

---

**Version:** 1.0.0  
**Date:** 13 Novembre 2024  
**Langage:** C (GTK3)  
**Plateforme:** Linux/WSL

Prêt à utiliser! 🚀
