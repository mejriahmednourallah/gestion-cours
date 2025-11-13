## 🔐 Identifiants de Test

### Connexion à l'application

L'application utilise un système d'authentification avec 3 rôles différents.

| Rôle | Username | Password | Permissions |
|------|----------|----------|-------------|
| **Administrateur** | `user` | `pass` | Accès complet à toutes les fonctionnalités |
| **Membre** | `member` | `member` | Accès limité (consultation principalement) |
| **Entraîneur** | `trainer` | `trainer` | Gestion des cours et membres |

### Données de test disponibles

#### Membres (15)
- Jean Dupont, Marie Martin, Pierre Bernard, Sophie Durand, Lucas Lefebvre
- Emma Moreau, Thomas Simon, Julie Laurent, Antoine Petit, Laura Garcia
- Alexandre Roux (inactif), Camille David, Nicolas Bertrand, Sarah Morel, Hugo Fournier

#### Entraîneurs (8)
- Michel Dupont (Yoga), Sophie Martin (Cardio), Luc Bernard (Musculation)
- Claire Leroy (Natation), Marc Bonnet (Pilates), Julie François (CrossFit)
- Paul Girard (Boxe), Alice Lambert (Danse)

#### Centres (5)
- Centre Fitness Paris
- Centre Yoga Marseille
- Centre Musculation Lyon
- Centre Aquatique Nice
- Centre Sport Toulouse

#### Cours (10)
- Yoga Débutant, Cardio Intense, Musculation Force
- Natation Adultes, Pilates Matinal, CrossFit Challenge
- Boxe Cardio, Danse Zumba, Yoga Avancé, Cardio Débutant

#### Équipements (10)
- Tapis de Yoga, Altères, Rameur, Vélo Statique
- Ballon Pilates, Kettlebell, Gants de Boxe, Tapis de Course
- Banc de Musculation, Corde à Sauter

#### Événements (6)
- Marathon Paris 2024
- Compétition Natation
- Triathlon d'Été
- Challenge CrossFit
- Tournoi de Boxe
- Gala de Danse

### Lancement de l'application

```bash
# Compiler
make clean && make

# Lancer l'application
./app
```

### Notes
- Le combo box "Rôle" sélectionne automatiquement "Administrateur" par défaut
- Vous pouvez changer le rôle en cliquant sur le menu déroulant
- Les données sont stockées dans le répertoire `data/`
