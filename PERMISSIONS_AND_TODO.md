# Permissions et État des Fonctionnalités

## ✅ Système de Permissions Implémenté

### Contrôle d'Accès par Rôle

Le système vérifie maintenant le rôle de l'utilisateur (`current_role`) avant d'ouvrir chaque module :

#### **Admin** (user/pass)
- ✅ **Accès complet** à tous les modules :
  - Membres (gestion complète)
  - Centres (gestion complète)
  - Cours (gestion complète)
  - Entraîneurs (gestion complète)
  - Équipements (gestion complète)
  - Événements (gestion complète)

#### **Membre** (member/member)
- ✅ **Accès limité** :
  - Membres (voir/modifier son propre profil uniquement)
  - Cours (inscription aux cours)
  - Événements (inscription aux événements)
- ❌ **Accès refusé** à :
  - Centres
  - Entraîneurs
  - Équipements

#### **Entraîneur** (trainer/trainer)
- ✅ **Accès limité** :
  - Entraîneurs (voir/modifier son propre profil)
  - Cours (s'inscrire comme coach)
  - Équipements (réserver du matériel)
  - Centres (s'inscrire dans des centres)
  - Événements (voir tous les événements)
- ❌ **Accès refusé** à :
  - Membres (pas d'accès aux membres)

### Messages d'Erreur

Quand un utilisateur tente d'accéder à un module sans permission :
```
┌─────────────────────────────────┐
│      Accès refusé              │
├─────────────────────────────────┤
│ Vous n'avez pas les permissions │
│ nécessaires pour accéder à      │
│ [nom du module]                 │
│                                 │
│           [ OK ]                │
└─────────────────────────────────┘
```

## ⚠️ À FAIRE : Connexion des Fonctions CRUD

### État Actuel des Boutons

Les boutons des dialogs **appellent des fonctions stubs** qui ne font qu'afficher des messages :

```c
void on_membres_add(GtkWidget* widget, gpointer data) {
    print_info("Ajouter membre");
    ajouter_membre();  // ← Cette fonction affiche seulement "Ajout d'un membre..."
}
```

### Ce Qui Doit Être Fait

#### 1. **Créer des Dialogs de Formulaire**

Pour chaque module, créer un dialog avec des champs de saisie :

**Exemple pour Membres** (ui/membre_form.glade) :
```xml
<dialog id="membre_form_dialog">
  <entry id="nom_entry"/>
  <entry id="prenom_entry"/>
  <entry id="email_entry"/>
  <entry id="telephone_entry"/>
  <calendar id="date_inscription_calendar"/>
  <switch id="actif_switch"/>
  <button id="save_button" signal="on_save_button_clicked"/>
</dialog>
```

#### 2. **Implémenter les Fonctions de Chargement de Données**

Modifier les fonctions `charger_*()` pour lire depuis les fichiers .txt :

**Exemple membres.c** :
```c
int charger_membres(Membre** data) {
    FILE* f = fopen("data/membres.txt", "r");
    if (!f) return 0;
    
    int count = 0;
    Membre* membres = malloc(sizeof(Membre) * 100);
    
    while (fscanf(f, "%d|%49[^|]|%49[^|]|%99[^|]|%19[^|]|%10[^|]|%d\n",
                  &membres[count].id,
                  membres[count].nom,
                  membres[count].prenom,
                  membres[count].email,
                  membres[count].telephone,
                  membres[count].date_inscription,
                  &membres[count].actif) == 7) {
        count++;
    }
    
    fclose(f);
    *data = membres;
    return count;
}
```

#### 3. **Peupler les TreeViews**

Dans chaque `on_*_refresh()` :

```c
void on_membres_refresh(GtkWidget* widget, gpointer data) {
    // 1. Récupérer le ListStore du TreeView
    GtkListStore* store = GTK_LIST_STORE(gtk_builder_get_object(builder, "membres_liststore"));
    gtk_list_store_clear(store);
    
    // 2. Charger les données
    Membre* membres;
    int count = charger_membres(&membres);
    
    // 3. Ajouter chaque ligne au ListStore
    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, membres[i].id,
            1, membres[i].nom,
            2, membres[i].prenom,
            3, membres[i].email,
            4, membres[i].telephone,
            5, membres[i].date_inscription,
            6, membres[i].actif ? "Oui" : "Non",
            -1);
    }
    
    free(membres);
}
```

#### 4. **Implémenter Add (Ajouter)**

```c
void on_membres_add(GtkWidget* widget, gpointer data) {
    // 1. Charger le dialog de formulaire
    GtkBuilder* form_builder = gtk_builder_new_from_file("ui/membre_form.glade");
    GtkDialog* form = GTK_DIALOG(gtk_builder_get_object(form_builder, "membre_form_dialog"));
    
    // 2. Afficher le dialog et attendre la réponse
    int response = gtk_dialog_run(form);
    
    if (response == GTK_RESPONSE_OK) {
        // 3. Récupérer les valeurs des champs
        GtkEntry* nom = GTK_ENTRY(gtk_builder_get_object(form_builder, "nom_entry"));
        // ... autres champs ...
        
        // 4. Créer le nouveau membre
        Membre nouveau;
        nouveau.id = generer_nouvel_id();
        strcpy(nouveau.nom, gtk_entry_get_text(nom));
        // ... remplir autres champs ...
        
        // 5. Sauvegarder dans le fichier
        FILE* f = fopen("data/membres.txt", "a");
        fprintf(f, "%d|%s|%s|%s|%s|%s|%d\n", 
                nouveau.id, nouveau.nom, nouveau.prenom,
                nouveau.email, nouveau.telephone,
                nouveau.date_inscription, nouveau.actif);
        fclose(f);
        
        // 6. Rafraîchir l'affichage
        on_membres_refresh(NULL, NULL);
    }
    
    gtk_widget_destroy(GTK_WIDGET(form));
}
```

#### 5. **Implémenter Edit (Modifier)**

```c
void on_membres_edit(GtkWidget* widget, gpointer data) {
    // 1. Récupérer la sélection dans le TreeView
    GtkTreeView* treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "membres_treeview"));
    GtkTreeSelection* selection = gtk_tree_view_get_selection(treeview);
    GtkTreeIter iter;
    
    if (!gtk_tree_selection_get_selected(selection, NULL, &iter)) {
        show_error_dialog("Veuillez sélectionner un membre à modifier");
        return;
    }
    
    // 2. Récupérer l'ID du membre sélectionné
    int id;
    GtkTreeModel* model = gtk_tree_view_get_model(treeview);
    gtk_tree_model_get(model, &iter, 0, &id, -1);
    
    // 3. Charger les données du membre
    Membre* membre = rechercher_membre(id);
    
    // 4. Afficher le dialog pré-rempli
    GtkBuilder* form_builder = gtk_builder_new_from_file("ui/membre_form.glade");
    GtkDialog* form = GTK_DIALOG(gtk_builder_get_object(form_builder, "membre_form_dialog"));
    
    // Pré-remplir les champs
    GtkEntry* nom = GTK_ENTRY(gtk_builder_get_object(form_builder, "nom_entry"));
    gtk_entry_set_text(nom, membre->nom);
    // ... autres champs ...
    
    // 5. Si OK, sauvegarder les modifications
    if (gtk_dialog_run(form) == GTK_RESPONSE_OK) {
        // Lire tous les membres
        // Modifier celui avec l'ID correspondant
        // Réécrire le fichier complet
        // Rafraîchir l'affichage
    }
}
```

#### 6. **Implémenter Delete (Supprimer)**

```c
void on_membres_delete(GtkWidget* widget, gpointer data) {
    // 1. Récupérer la sélection
    GtkTreeView* treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "membres_treeview"));
    GtkTreeSelection* selection = gtk_tree_view_get_selection(treeview);
    GtkTreeIter iter;
    
    if (!gtk_tree_selection_get_selected(selection, NULL, &iter)) {
        show_error_dialog("Veuillez sélectionner un membre à supprimer");
        return;
    }
    
    // 2. Confirmation
    GtkDialog* confirm = gtk_message_dialog_new(
        main_window,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "Confirmer la suppression ?"
    );
    
    int response = gtk_dialog_run(confirm);
    gtk_widget_destroy(GTK_WIDGET(confirm));
    
    if (response == GTK_RESPONSE_YES) {
        // 3. Récupérer l'ID
        int id;
        GtkTreeModel* model = gtk_tree_view_get_model(treeview);
        gtk_tree_model_get(model, &iter, 0, &id, -1);
        
        // 4. Supprimer du fichier
        supprimer_membre(id);
        
        // 5. Rafraîchir
        on_membres_refresh(NULL, NULL);
    }
}
```

#### 7. **Implémenter Search (Rechercher)**

```c
void on_membres_search(GtkWidget* widget, gpointer data) {
    // 1. Récupérer le texte de recherche
    GtkEntry* search = GTK_ENTRY(gtk_builder_get_object(builder, "search_entry"));
    const char* query = gtk_entry_get_text(search);
    
    if (strlen(query) == 0) {
        on_membres_refresh(NULL, NULL);  // Si vide, tout afficher
        return;
    }
    
    // 2. Filtrer les résultats
    GtkListStore* store = GTK_LIST_STORE(gtk_builder_get_object(builder, "membres_liststore"));
    gtk_list_store_clear(store);
    
    Membre* membres;
    int count = charger_membres(&membres);
    
    for (int i = 0; i < count; i++) {
        // Rechercher dans nom, prénom, email
        if (strstr(membres[i].nom, query) ||
            strstr(membres[i].prenom, query) ||
            strstr(membres[i].email, query)) {
            
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            // ... ajouter les données ...
        }
    }
    
    free(membres);
}
```

### Modules à Compléter

| Module | Fonctions à Implémenter | Priorité |
|--------|-------------------------|----------|
| **Membres** | Add, Edit, Delete, Search, Refresh | 🔴 Haute |
| **Centres** | Add, Edit, Delete, Refresh | 🔴 Haute |
| **Cours** | Add, Edit, Delete, Refresh, Enroll | 🟡 Moyenne |
| **Entraîneurs** | Add, Edit, Delete, Refresh | 🟡 Moyenne |
| **Équipements** | Add, Edit, Delete, Refresh, Reserve | 🟢 Basse |
| **Événements** | Add, Edit, Delete, Refresh, Register | 🟢 Basse |

## 📋 Checklist de Développement

### Phase 1 : Infrastructure (✅ Terminé)
- [x] Système d'authentification
- [x] Vérification des permissions par rôle
- [x] Dialogs des modules
- [x] Structure des TreeViews

### Phase 2 : Chargement des Données (❌ À faire)
- [ ] Implémenter `charger_membres()` réel
- [ ] Implémenter `charger_centres()` réel
- [ ] Implémenter `charger_cours()` réel
- [ ] Implémenter `charger_entraineurs()` réel
- [ ] Implémenter `charger_equipements()` réel
- [ ] Implémenter `charger_evenements()` réel

### Phase 3 : Affichage (❌ À faire)
- [ ] Peupler TreeView Membres au chargement du dialog
- [ ] Peupler TreeView Centres au chargement
- [ ] Peupler TreeView Cours au chargement
- [ ] Peupler TreeView Entraîneurs au chargement
- [ ] Peupler TreeView Équipements au chargement
- [ ] Peupler TreeView Événements au chargement

### Phase 4 : Formulaires (❌ À faire)
- [ ] Créer `ui/membre_form.glade`
- [ ] Créer `ui/centre_form.glade`
- [ ] Créer `ui/cours_form.glade`
- [ ] Créer `ui/entraineur_form.glade`
- [ ] Créer `ui/equipement_form.glade`
- [ ] Créer `ui/evenement_form.glade`

### Phase 5 : CRUD Complet (❌ À faire)
- [ ] **Membres** : Add, Edit, Delete, Search
- [ ] **Centres** : Add, Edit, Delete
- [ ] **Cours** : Add, Edit, Delete, Enrollment
- [ ] **Entraîneurs** : Add, Edit, Delete
- [ ] **Équipements** : Add, Edit, Delete, Reservation
- [ ] **Événements** : Add, Edit, Delete, Registration

### Phase 6 : Permissions Avancées (❌ À faire)
- [ ] Membre : Ne peut modifier que son propre profil
- [ ] Membre : Bouton "S'inscrire au cours"
- [ ] Entraîneur : Ne peut modifier que son propre profil
- [ ] Entraîneur : Bouton "Devenir coach pour ce cours"
- [ ] Admin : Boutons Add/Edit/Delete visibles
- [ ] Non-Admin : Boutons Add/Edit/Delete masqués

## 🔐 Sécurité des Permissions

### Vérifications Actuelles

1. **Menu handlers** : Chaque `on_menu_*()` vérifie le rôle avant d'ouvrir le dialog
2. **Messages d'erreur** : Dialog modal avec message clair si accès refusé
3. **Rôle global** : Variable `current_role` définie à la connexion

### Vérifications Manquantes (À Ajouter)

1. **Au niveau des boutons CRUD** :
   ```c
   void on_membres_add(GtkWidget* widget, gpointer data) {
       if (current_role != ROLE_ADMIN) {
           show_permission_error("l'ajout de membres");
           return;
       }
       // ... reste du code ...
   }
   ```

2. **Masquage des boutons selon le rôle** :
   ```c
   // Dans on_menu_membres()
   if (current_role != ROLE_ADMIN) {
       GtkButton* add_btn = GTK_BUTTON(gtk_builder_get_object(builder, "add_button"));
       GtkButton* delete_btn = GTK_BUTTON(gtk_builder_get_object(builder, "delete_button"));
       gtk_widget_set_sensitive(GTK_WIDGET(add_btn), FALSE);
       gtk_widget_set_sensitive(GTK_WIDGET(delete_btn), FALSE);
   }
   ```

3. **Filtrage des données** :
   ```c
   // Un membre ne voit que son propre profil
   if (current_role == ROLE_MEMBER) {
       // Charger uniquement le membre avec current_user
       Membre* membre = rechercher_membre_par_nom(current_user);
       // Afficher seulement ce membre
   }
   ```

## 🎯 Ordre de Développement Recommandé

1. **Commencer par Membres** (module le plus simple)
   - Créer le formulaire
   - Implémenter charger_membres()
   - Implémenter on_membres_refresh()
   - Tester l'affichage
   - Implémenter Add
   - Implémenter Edit
   - Implémenter Delete
   - Implémenter Search

2. **Dupliquer pour Centres** (structure similaire)

3. **Cours** (plus complexe avec inscriptions)

4. **Entraîneurs** (similaire à Membres)

5. **Équipements** (avec réservations)

6. **Événements** (avec registrations)

7. **Permissions avancées** (filtrage par utilisateur)

## 📝 Résumé

### ✅ Ce qui fonctionne
- Authentification avec 3 rôles
- Contrôle d'accès aux modules
- Messages d'erreur de permission
- Dialogs des 6 modules
- Structure des TreeViews

### ⚠️ Ce qui manque
- Chargement réel des données depuis fichiers
- Affichage des données dans les TreeViews
- Formulaires de saisie
- Opérations CRUD fonctionnelles
- Filtrage des données selon le rôle (membre voit uniquement son profil)
- Masquage des boutons selon les permissions

### 🎯 Prochaine étape prioritaire
**Implémenter le module Membres complet** comme modèle de référence, puis dupliquer la logique pour les autres modules.
