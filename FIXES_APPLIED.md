# 🔧 Corrections Appliquées - ComboBox Rôle

## Problèmes identifiés et résolus

### 1. ❌ Erreur: `Unhandled tag: <columns>` 
**Solution**: Remplacement des balises `<columns>` et `<column>` par `<child>` dans le GtkTreeView (ligne 580 de main.glade)

### 2. ❌ Erreur: `Could not find signal handler 'on_login_button_clicked'`
**Causes**:
- Fonctions callbacks déclarées `static` → Non exportées
- Flag `-rdynamic` manquant lors de la compilation

**Solutions appliquées**:
- ✅ Ajout du flag `-rdynamic` dans le Makefile
- ✅ Suppression de `static` de tous les callbacks (on_*)
- ✅ Connexion automatique des signaux via `gtk_builder_connect_signals()`

### 3. ❌ Problème: ComboBox rôle non sélectionnable
**Causes**:
- Syntaxe incorrecte: `<items>Administrateur\nMembre\nEntraîneur</items>`
- Pas d'initialisation programmatique

**Solutions appliquées**:
- ✅ Correction de la syntaxe GTK3:
  ```xml
  <items>
    <item translatable="no">Administrateur</item>
    <item translatable="no">Membre</item>
    <item translatable="no">Entraîneur</item>
  </items>
  ```
- ✅ Propriété `active="1"` pour sélectionner "Membre" par défaut
- ✅ Initialisation programmatique dans `setup_ui()`:
  ```c
  gtk_combo_box_set_active(GTK_COMBO_BOX(role_combo), 1);
  ```

## Fichiers modifiés

### `/workspaces/gestion-cours/Makefile`
```diff
- CFLAGS = -Wall -Wextra -std=c99 `pkg-config --cflags gtk+-3.0`
+ CFLAGS = -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0`
```

### `/workspaces/gestion-cours/ui/main.glade`
1. Lignes 580-657: Correction de la syntaxe des colonnes du TreeView
2. Lignes 73-83: Correction du ComboBoxText avec items individuels et active="1"

### `/workspaces/gestion-cours/src/main.c`
1. Tous les callbacks: `static void on_*` → `void on_*` (environ 30 fonctions)
2. Fonction `setup_ui()`: Ajout de l'initialisation du combo box
3. Fonction `on_login_button_clicked()`: Ajout de la validation du rôle sélectionné

## Test de validation

```bash
# Compiler
chmod +x build.sh
./build.sh

# Ou manuellement
make clean && make

# Lancer
./app
```

## Comportement attendu

1. **Au lancement**: 
   - La fenêtre de login s'affiche
   - Le ComboBox "Rôle" affiche "Membre" par défaut
   - Le ComboBox est cliquable et permet de sélectionner d'autres rôles

2. **Lors de la connexion**:
   - Si aucun rôle sélectionné (cas impossible maintenant): Message d'erreur
   - Si identifiants vides: Message d'erreur
   - Si identifiants incorrects: Message d'erreur
   - Si authentification réussie: Fenêtre principale s'affiche

## Identifiants de test

| Rôle           | Username | Password |
|----------------|----------|----------|
| Administrateur | user     | pass     |
| **Membre** ⭐  | member   | member   |
| Entraîneur     | trainer  | trainer  |

⭐ = Rôle sélectionné par défaut

## Notes techniques

- Le flag `-rdynamic` permet l'export de tous les symboles pour la liaison dynamique
- `gtk_builder_connect_signals()` utilise `g_module_open()` pour trouver les callbacks
- Les balises `<item>` sont la syntaxe standard GTK3 pour GtkComboBoxText
- L'attribut `translatable="no"` empêche la traduction automatique (optionnel mais recommandé)
