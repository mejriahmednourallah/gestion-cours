# 🚀 Instructions de Compilation et Test

## ⚠️ IMPORTANT: Vous devez RECOMPILER l'application !

Les corrections suivantes ont été appliquées au code source mais l'exécutable `app` est encore l'ancienne version :

### ✅ Corrections appliquées :
1. **Flag -rdynamic** ajouté au Makefile
2. **Tous les callbacks exportés** (static retiré de ~30 fonctions)
3. **ComboBox Rôle** corrigé avec syntaxe GTK3
4. **Rôle "Membre" présélectionné** par défaut
5. **Initialisation programmatique** du combo box

---

## 📝 Pour RECOMPILER (REQUIS):

### Option 1: Utiliser Make
```bash
make clean
make
```

### Option 2: Script de compilation
```bash
chmod +x compile.sh
./compile.sh
```

### Option 3: Compilation manuelle
```bash
# Nettoyer
rm -f src/*.o app

# Compiler chaque module
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0` -c src/main.c -o src/main.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0` -c src/membres.c -o src/membres.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0` -c src/entraineurs.c -o src/entraineurs.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0` -c src/cours.c -o src/cours.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0` -c src/equipements.c -o src/equipements.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0` -c src/centres.c -o src/centres.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0` -c src/evenements.c -o src/evenements.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0` -c src/utils.c -o src/utils.o

# Linker
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0` -o app src/*.o `pkg-config --libs gtk+-3.0`
```

---

## 🔐 Identifiants de Test

Après recompilation, utilisez ces identifiants :

| Rôle | Username | Password | Notes |
|------|----------|----------|-------|
| **Membre** ⭐ | `member` | `member` | Présélectionné par défaut |
| Administrateur | `user` | `pass` | Accès complet |
| Entraîneur | `trainer` | `trainer` | Gestion cours |

---

## ✨ Comportement Attendu du Login

Après recompilation, voici ce qui devrait se passer :

### 1. **Au lancement** (`./app`)
- ✅ Fenêtre de login s'affiche
- ✅ ComboBox "Rôle" affiche "Membre" par défaut
- ✅ Le combo box est cliquable
- ✅ Champs username et password vides

### 2. **Lors du clic sur "Connexion"**

#### Cas de succès :
```
Username: member
Password: member
Rôle: Membre (déjà sélectionné)
```
**Résultat** : 
- ✅ Fenêtre de login se cache
- ✅ Fenêtre principale s'affiche
- ✅ Tableau de bord visible avec menu
- ✅ Message en console : "Authentification réussie"

#### Cas d'erreur :
- Champs vides → Message : "Veuillez remplir tous les champs"
- Identifiants incorrects → Message : "Identifiants invalides"
- Pas de rôle sélectionné → Message : "Veuillez sélectionner un rôle" (impossible maintenant)

---

## 🐛 Résolution de Problèmes

### "Could not find signal handler 'on_login_button_clicked'"
**Cause** : Application non recompilée avec `-rdynamic`  
**Solution** : Recompiler avec `make clean && make`

### "ComboBox vide ou non sélectionnable"
**Cause** : Ancienne version de ui/main.glade  
**Solution** : Le fichier est déjà corrigé, recompilez juste

### "cannot open display"
**Cause** : Pas de serveur X11 dans le conteneur  
**Solution** : C'est normal, l'application est prête mais ne peut pas afficher l'interface dans ce type d'environnement

---

## 📊 Vérification de la Compilation

Après compilation, vérifiez :

```bash
# Vérifier que l'app existe
ls -lh app

# Vérifier les symboles exportés
nm app | grep on_login_button_clicked
# Devrait afficher quelque chose comme: 00000000004XXXXX T on_login_button_clicked

# Date de modification (doit être récente)
stat app
```

---

## 🎯 Test Complet

```bash
# 1. Nettoyer
make clean

# 2. Compiler
make

# 3. Vérifier
ls -lh app

# 4. Lancer (affichera "cannot open display" dans conteneur)
./app
```

---

## 📝 Notes Importantes

1. **Le code source est corrigé** ✅
2. **Vous devez RECOMPILER** pour que les changements prennent effet
3. L'erreur "cannot open display" est **normale** dans un conteneur sans X11
4. Pour tester réellement l'interface, il faut :
   - Un environnement avec serveur X11
   - Ou configurer le forwarding X11
   - Ou utiliser un système avec interface graphique

---

## ✅ Checklist

- [ ] J'ai exécuté `make clean`
- [ ] J'ai exécuté `make` (ou `./compile.sh`)
- [ ] La compilation s'est terminée sans erreur
- [ ] Le fichier `app` a été créé/mis à jour
- [ ] J'ai lancé `./app` pour tester
