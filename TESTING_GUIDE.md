# Guide de Test - Gestion Centres Sportifs

## 🚀 Démarrage Rapide

### Compilation
```bash
cd /workspaces/gestion-cours
make clean && make
```

### Lancement
```bash
./app
```

**Note**: Nécessite un environnement graphique X11. Dans un conteneur Docker sans GUI, l'app ne s'affichera pas mais compilera sans erreur.

---

## 🧪 Tests Disponibles

### 1. Test Backend (Sans GUI)
Tester les opérations CRUD sans interface graphique:
```bash
make crud-test
./crud_test
```

**Ce qui est testé**:
- Chargement membres depuis data/membres.txt
- Suppression d'un membre (ID 2)
- Rechargement pour vérifier suppression
- Chargement centres depuis data/centres.txt
- Suppression d'un centre (ID 1)

---

## 🔐 Comptes de Test

### Admin (Accès complet)
- **Utilisateur**: `user`
- **Mot de passe**: `pass`
- **Permissions**: Créer, Modifier, Supprimer (tous modules)

### Membre
- **Utilisateur**: `member`
- **Mot de passe**: `member`
- **Permissions**: Voir uniquement (pas de CRUD)
- **Fonctions spéciales**: Demander coach, S'inscrire cours/événements (NON IMPLÉMENTÉES)

### Entraîneur
- **Utilisateur**: `trainer`
- **Mot de passe**: `trainer`
- **Permissions**: Voir uniquement (pas de CRUD)
- **Fonctions spéciales**: S'inscrire centre, Rejoindre cours, Réserver équipement (NON IMPLÉMENTÉES)

---

## 📋 Tests Manuels à Effectuer

### Test 1: Membres
1. Login: `user` / `pass`
2. Cliquer Menu → **Gestion Membres**
3. ✅ Vérifier: 3 membres affichés (Dupont Martin, Durand Sophie, Martin Julie)
4. Sélectionner ligne 2 (Durand Sophie)
5. Cliquer **Supprimer**
6. ✅ Vérifier: Dialog confirmation apparaît
7. Cliquer **Oui**
8. ✅ Vérifier: 2 membres restants
9. Fermer dialog, rouvrir
10. ✅ Vérifier: Sophie toujours absente (persistence)

### Test 2: Centres
1. Menu → **Gestion Centres**
2. ✅ Vérifier: 3 centres (Centre Ville, Quartier Nord, Zone Sud)
3. Sélectionner Centre Ville (ID 1)
4. Cliquer **Supprimer**
5. ✅ Vérifier: Confirmation "Supprimer Centre Ville (ID: 1) ?"
6. Cliquer **Oui**
7. ✅ Vérifier: 2 centres restants

### Test 3: Cours
1. Menu → **Gestion Cours**
2. ✅ Vérifier: 3 cours (Yoga débutants, CrossFit avancé, Pilates intermédiaire)
3. ✅ Vérifier colonnes: ID, Nom, Type, Horaire, EntraineurID, CapaciteMax, Inscrits, CentreID
4. Sélectionner Yoga (ID 1)
5. Cliquer **Supprimer**
6. ✅ Vérifier: Dialog confirmation
7. Cliquer **Oui**
8. ✅ Vérifier: 2 cours restants

### Test 4: Entraîneurs ⭐ NOUVEAU
1. Menu → **Gestion Entraîneurs**
2. ✅ Vérifier: 3 entraîneurs affichés
   - Dupont Michel (Yoga)
   - Martin Claire (CrossFit)
   - Bernard Thomas (Pilates)
3. ✅ Vérifier colonnes: ID, Nom, Prenom, Specialite, Telephone, Disponible, CentreID
4. Sélectionner Dupont Michel
5. Cliquer **Supprimer**
6. ✅ Vérifier: Confirmation "Supprimer l'entraîneur Michel Dupont (ID: 1) ?"
7. Cliquer **Oui**
8. ✅ Vérifier: 2 entraîneurs restants

### Test 5: Équipements ⭐ NOUVEAU
1. Menu → **Gestion Équipements**
2. ✅ Vérifier: 4 équipements
   - Tapis de Yoga (50 unités)
   - Haltères (100 paires)
   - Ballon Pilates (30 unités)
   - Corde à sauter (75 unités)
3. ✅ Vérifier colonnes: ID, Nom, Type, Quantite, Disponible, CentreID
4. Sélectionner Tapis de Yoga
5. Cliquer **Supprimer**
6. ✅ Vérifier: Confirmation
7. Cliquer **Oui**
8. ✅ Vérifier: 3 équipements restants

### Test 6: Événements ⭐ NOUVEAU
1. Menu → **Gestion Événements**
2. ✅ Vérifier: 3 événements
   - Marathon 2024 (10km, 100 places, 45 inscrits)
   - Tournoi Basket (5x5, 50 places, 30 inscrits)
   - Journée Portes Ouvertes (200 places, 120 inscrits)
3. ✅ Vérifier colonnes: ID, Nom, Description, Date, CentreID, CapaciteMax, Inscrits
4. Sélectionner Marathon
5. Cliquer **Supprimer**
6. ✅ Vérifier: Confirmation
7. Cliquer **Oui**
8. ✅ Vérifier: 2 événements restants

### Test 7: Permissions
1. Logout (Menu → Déconnexion)
2. Login: `member` / `member`
3. Ouvrir **Gestion Membres**
4. Sélectionner un membre, cliquer **Supprimer**
5. ✅ Vérifier: Dialog erreur "Vous n'avez pas la permission pour la suppression de membres"
6. Répéter pour autres modules
7. ✅ Vérifier: Member ne peut rien supprimer

### Test 8: Boutons Ajouter/Modifier (Stubs)
1. Login Admin
2. Ouvrir n'importe quel module
3. Cliquer **Ajouter**
4. ✅ Vérifier: Message console "[INFO] Module X - Fonction ajouter_X appelée"
5. Cliquer **Modifier**
6. ✅ Vérifier: Message stub similaire

### Test 9: Recherche Membres
1. Ouvrir **Gestion Membres**
2. Dans champ recherche, taper "Martin"
3. Cliquer **Rechercher**
4. ✅ Vérifier: Filtre appliqué (Dupont Martin, Martin Julie si présents)
5. Effacer recherche
6. Cliquer **Rafraîchir**
7. ✅ Vérifier: Tous membres réapparaissent

---

## 🐛 Bugs Attendus (Connus)

### Warnings Compilation
- ⚠️ "Implicit declaration of on_*_refresh" - **Bénin** (déclarations forward manquantes)
- ⚠️ "Unused parameter widget/data" - **Normal** (convention GTK)
- ⚠️ "Too many arguments for format" - **Cosmétique** (dialogs)

### Fonctionnalités Manquantes
- ❌ **Ajouter/Modifier** → Affiche stub au lieu de formulaire
- ❌ **Recherche** → Fonctionne uniquement pour membres
- ❌ **Boutons Member/Trainer** → Pas d'actions spécifiques (S'inscrire, Demander coach, etc.)

### Limites UI
- ⚠️ Pas de tri colonnes TreeView
- ⚠️ Pas de double-clic pour éditer
- ⚠️ Pas d'indicateur visuel sélection multiple
- ⚠️ Dialogs modaux (bloquent fenêtre principale)

---

## 📁 Fichiers de Données

### Localisation
```
data/
├── membres.txt       (3 membres initiaux)
├── centres.txt       (3 centres)
├── cours.txt         (3 cours)
├── entraineurs.txt   (3 entraîneurs)
├── equipements.txt   (4 équipements)
└── evenements.txt    (3 événements)
```

### Format CSV (Exemples)

**membres.txt**:
```csv
1,Dupont,Martin,martin.dupont@email.com,0612345678,2024-01-15,1
2,Durand,Sophie,sophie.durand@email.com,0623456789,2024-02-20,1
3,Martin,Julie,julie.martin@email.com,0634567890,2024-03-10,1
```

**entraineurs.txt**:
```csv
1,Dupont,Michel,Yoga,0612111111,1,1
2,Martin,Claire,CrossFit,0623222222,1,2
3,Bernard,Thomas,Pilates,0634333333,0,1
```

**equipements.txt**:
```csv
1,Tapis de Yoga,Yoga,50,1,1
2,Haltères,Musculation,100,1,2
3,Ballon Pilates,Pilates,30,1,1
4,Corde à sauter,Cardio,75,1,3
```

### Restaurer Données Initiales
Si vous avez supprimé trop de données:
```bash
cd /workspaces/gestion-cours
git checkout data/*.txt
```

---

## 🔧 Dépannage

### Erreur "Builder non disponible"
**Cause**: Global builder NULL (dialog fermé)
**Solution**: Rouvrir le module depuis le menu

### TreeView vide après Rafraîchir
**Cause**: Fichier data/*.txt corrompu ou vide
**Solution**: 
```bash
cat data/membres.txt  # Vérifier contenu
git checkout data/membres.txt  # Restaurer
```

### Compilation échoue
**Cause**: Fichiers .o obsolètes
**Solution**:
```bash
make clean && make
```

### Segfault au lancement
**Cause**: Fichier .glade manquant ou corrompu
**Solution**:
```bash
ls -la ui/*.glade  # Vérifier présence
git status ui/     # Vérifier modifications
```

---

## 📊 Résultats Attendus

### Tests Réussis
- ✅ Compilation sans erreur
- ✅ Lancement sans crash
- ✅ Login Admin/Member/Trainer
- ✅ 6 modules chargent données
- ✅ 6 modules suppriment avec confirmation
- ✅ Permissions bloquent Member/Trainer delete
- ✅ Données persistent après fermeture/réouverture

### Tests Échoués (Attendus)
- ❌ Ajouter/Modifier → Stubs
- ❌ Recherche centres/cours/etc → Non implémenté
- ❌ Boutons spécifiques rôles → Non implémenté

---

## 🎯 Checklist Complète

**Backend (6/6 modules)**:
- [x] Membres - charger/supprimer
- [x] Centres - charger/supprimer
- [x] Cours - charger/supprimer
- [x] Entraîneurs - charger/supprimer ⭐
- [x] Équipements - charger/supprimer ⭐
- [x] Événements - charger/supprimer ⭐

**Frontend (6/6 modules)**:
- [x] Membres - refresh/delete callbacks
- [x] Centres - refresh/delete callbacks
- [x] Cours - refresh/delete callbacks
- [x] Entraîneurs - refresh/delete callbacks ⭐
- [x] Équipements - refresh/delete callbacks ⭐
- [x] Événements - refresh/delete callbacks ⭐

**Système**:
- [x] Permissions (Admin/Member/Trainer)
- [x] Authentification (3 comptes)
- [x] Global builders (6 modules)
- [x] Dialogs confirmation suppression

**Non Implémenté**:
- [ ] Formulaires Add/Edit
- [ ] Recherche (sauf membres)
- [ ] Fonctions spécifiques rôles
- [ ] UI différenciée par rôle

---

**Date**: $(date)
**Version**: 1.0 (CRUD Read/Delete complet)
