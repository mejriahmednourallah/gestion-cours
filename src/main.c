#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "membres.h"
#include "entraineurs.h"
#include "cours.h"
#include "equipements.h"
#include "centres.h"
#include "evenements.h"

// Global variables
GtkBuilder* builder = NULL;
GtkWindow* login_window = NULL;
GtkWindow* main_window = NULL;

// Enum for user roles
typedef enum {
    ROLE_ADMIN,
    ROLE_MEMBER,
    ROLE_TRAINER
} UserRole;

UserRole current_role = ROLE_ADMIN;
char current_user[50] = "Admin";

// Global builders for each module
GtkBuilder* global_membres_builder = NULL;
GtkBuilder* global_centres_builder = NULL;
GtkBuilder* global_cours_builder = NULL;
GtkBuilder* global_entraineurs_builder = NULL;
GtkBuilder* global_equipements_builder = NULL;
GtkBuilder* global_evenements_builder = NULL;

// ============= Permission Check Functions =============

gboolean can_access_admin_features() {
    return current_role == ROLE_ADMIN;
}

gboolean can_access_member_features() {
    return current_role == ROLE_MEMBER || current_role == ROLE_ADMIN;
}

gboolean can_access_trainer_features() {
    return current_role == ROLE_TRAINER || current_role == ROLE_ADMIN;
}

void show_permission_error(const char* feature) {
    GtkWidget* error_dialog = gtk_message_dialog_new(
        main_window,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_WARNING,
        GTK_BUTTONS_OK,
        "Accès refusé"
    );
    gtk_message_dialog_format_secondary_text(
        GTK_MESSAGE_DIALOG(error_dialog),
        "Vous n'avez pas les permissions nécessaires pour accéder à %s",
        feature
    );
    gtk_dialog_run(GTK_DIALOG(error_dialog));
    gtk_widget_destroy(error_dialog);
}

// ============= Authentication Functions =============

gboolean validate_credentials(const char* username, const char* password, UserRole* role) {
    // Simple hardcoded credentials for demo
    if (strcmp(username, "user") == 0 && strcmp(password, "pass") == 0) {
        *role = ROLE_ADMIN;
        return TRUE;
    }
    if (strcmp(username, "member") == 0 && strcmp(password, "member") == 0) {
        *role = ROLE_MEMBER;
        return TRUE;
    }
    if (strcmp(username, "trainer") == 0 && strcmp(password, "trainer") == 0) {
        *role = ROLE_TRAINER;
        return TRUE;
    }
    return FALSE;
}

// ============= Login Window Callbacks =============

void on_login_button_clicked(GtkWidget* widget, gpointer data) {
    GtkEntry* username_entry = GTK_ENTRY(gtk_builder_get_object(builder, "username_entry"));
    GtkEntry* password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry"));
    GtkComboBoxText* role_combo = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "role_combo"));
    
    const char* username = gtk_entry_get_text(username_entry);
    const char* password = gtk_entry_get_text(password_entry);
    gint role_index = gtk_combo_box_get_active(GTK_COMBO_BOX(role_combo));
    
    // Check if a role is selected
    if (role_index < 0) {
        GtkDialog* error = GTK_DIALOG(
            gtk_message_dialog_new(
                login_window,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Veuillez sélectionner un rôle"
            )
        );
        gtk_dialog_run(error);
        gtk_widget_destroy(GTK_WIDGET(error));
        return;
    }
    
    UserRole selected_role = (UserRole)role_index;
    
    if (strlen(username) == 0 || strlen(password) == 0) {
        GtkDialog* error = GTK_DIALOG(
            gtk_message_dialog_new(
                login_window,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Erreur d'authentification",
                "Veuillez remplir tous les champs"
            )
        );
        gtk_dialog_run(error);
        gtk_widget_destroy(GTK_WIDGET(error));
        return;
    }
    
    if (!validate_credentials(username, password, &selected_role)) {
        GtkDialog* error = GTK_DIALOG(
            gtk_message_dialog_new(
                login_window,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Identifiants invalides",
                "Veuillez vérifier votre nom d'utilisateur et mot de passe"
            )
        );
        gtk_dialog_run(error);
        gtk_widget_destroy(GTK_WIDGET(error));
        return;
    }
    
    // Authentication successful
    strncpy(current_user, username, sizeof(current_user) - 1);
    current_role = selected_role;
    
    print_info("Authentification réussie - Chargement de l'interface principale...");
    
    // Update labels
    GtkLabel* user_label = GTK_LABEL(gtk_builder_get_object(builder, "current_user_label"));
    GtkLabel* role_label = GTK_LABEL(gtk_builder_get_object(builder, "current_role_label"));
    
    char user_text[100];
    char role_text[100];
    const char* role_names[] = {"Administrateur", "Membre", "Entraîneur"};
    
    snprintf(user_text, sizeof(user_text), "Utilisateur: %s", current_user);
    snprintf(role_text, sizeof(role_text), "Rôle: %s", role_names[current_role]);
    
    if (user_label) {
        gtk_label_set_text(user_label, user_text);
        print_info("Label utilisateur mis à jour");
    }
    if (role_label) {
        gtk_label_set_text(role_label, role_text);
        print_info("Label rôle mis à jour");
    }
    
    // Hide login window and show main window with all children
    gtk_widget_hide(GTK_WIDGET(login_window));
    print_info("Fenêtre de login masquée");
    
    gtk_widget_show_all(GTK_WIDGET(main_window));
    print_success("Fenêtre principale affichée - Bienvenue!");
}

void on_cancel_button_clicked(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

void on_login_window_destroy(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

// ============= Main Window Callbacks =============

void on_main_window_destroy(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

void on_quit_menu(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

void on_logout_menu(GtkWidget* widget, gpointer data) {
    gtk_widget_hide(GTK_WIDGET(main_window));
    
    // Clear fields
    GtkEntry* username_entry = GTK_ENTRY(gtk_builder_get_object(builder, "username_entry"));
    GtkEntry* password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry"));
    gtk_entry_set_text(username_entry, "");
    gtk_entry_set_text(password_entry, "");
    
    gtk_widget_show(GTK_WIDGET(login_window));
    print_info("Déconnexion réussie");
}

void on_about_menu(GtkWidget* widget, gpointer data) {
    GtkDialog* about = GTK_DIALOG(
        gtk_message_dialog_new(
            main_window,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "À propos",
            "Gestion des Centres Sportifs v1.0\n\n"
            "Application GTK3 pour la gestion complète\n"
            "des centres sportifs\n\n"
            "© 2024"
        )
    );
    gtk_dialog_run(about);
    gtk_widget_destroy(GTK_WIDGET(about));
}

// ============= Module Menu Handlers =============

void on_menu_membres(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    // Membres: Admin (full access), Member (view/edit self only)
    if (!can_access_admin_features() && !can_access_member_features()) {
        show_permission_error("le module Membres");
        return;
    }
    
    print_info("Ouverture du module Membres");
    
    GError* error = NULL;
    global_membres_builder = gtk_builder_new();
    
    if (gtk_builder_add_from_file(global_membres_builder, "ui/membres.glade", &error)) {
        GtkDialog* dialog = GTK_DIALOG(gtk_builder_get_object(global_membres_builder, "membres_dialog"));
        gtk_builder_connect_signals(global_membres_builder, NULL);
        
        // Charger les données au démarrage
        on_membres_refresh(NULL, NULL);
        
        gtk_widget_show_all(GTK_WIDGET(dialog));
        gtk_dialog_run(dialog);
        gtk_widget_destroy(GTK_WIDGET(dialog));
    } else {
        print_error(error ? error->message : "Erreur lors du chargement de membres.glade");
        if (error) g_error_free(error);
    }
    
    if (global_membres_builder) {
        g_object_unref(global_membres_builder);
        global_membres_builder = NULL;
    }
}

void on_menu_centres(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    // Centres: Admin (full access), Trainer (register at centers)
    if (!can_access_admin_features() && !can_access_trainer_features()) {
        show_permission_error("le module Centres");
        return;
    }
    
    print_info("Ouverture du module Centres");
    
    GError* error = NULL;
    global_centres_builder = gtk_builder_new();
    
    if (gtk_builder_add_from_file(global_centres_builder, "ui/centres.glade", &error)) {
        GtkDialog* dialog = GTK_DIALOG(gtk_builder_get_object(global_centres_builder, "centres_dialog"));
        gtk_builder_connect_signals(global_centres_builder, NULL);
        on_centres_refresh(NULL, NULL);
        gtk_widget_show_all(GTK_WIDGET(dialog));
        gtk_dialog_run(dialog);
        gtk_widget_destroy(GTK_WIDGET(dialog));
    } else {
        print_error(error ? error->message : "Erreur lors du chargement de centres.glade");
        if (error) g_error_free(error);
    }
    
    if (global_centres_builder) {
        g_object_unref(global_centres_builder);
        global_centres_builder = NULL;
    }
}

void on_menu_courses(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    // Cours: Accessible à tous (Admin, Member, Trainer)
    // Pas de restriction - tous peuvent voir/s'inscrire aux cours
    
    print_info("Ouverture du module Cours");
    
    GError* error = NULL;
    global_cours_builder = gtk_builder_new();
    
    if (gtk_builder_add_from_file(global_cours_builder, "ui/cours.glade", &error)) {
        GtkDialog* dialog = GTK_DIALOG(gtk_builder_get_object(global_cours_builder, "cours_dialog"));
        gtk_builder_connect_signals(global_cours_builder, NULL);
        on_cours_refresh(NULL, NULL);
        gtk_widget_show_all(GTK_WIDGET(dialog));
        gtk_dialog_run(dialog);
        gtk_widget_destroy(GTK_WIDGET(dialog));
    } else {
        print_error(error ? error->message : "Erreur lors du chargement de cours.glade");
        if (error) g_error_free(error);
    }
    
    if (global_cours_builder) {
        g_object_unref(global_cours_builder);
        global_cours_builder = NULL;
    }
}

void on_menu_trainers(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    // Entraîneurs: Admin (full access), Trainer (view/edit self, enroll as coach)
    if (!can_access_admin_features() && !can_access_trainer_features()) {
        show_permission_error("le module Entraîneurs");
        return;
    }
    
    print_info("Ouverture du module Entraîneurs");
    
    GError* error = NULL;
    GtkBuilder* trainers_builder = gtk_builder_new();
    
    if (gtk_builder_add_from_file(trainers_builder, "ui/entraineurs.glade", &error)) {
        GtkDialog* dialog = GTK_DIALOG(gtk_builder_get_object(trainers_builder, "entraineurs_dialog"));
        gtk_builder_connect_signals(trainers_builder, NULL);
        gtk_widget_show_all(GTK_WIDGET(dialog));
        gtk_dialog_run(dialog);
        gtk_widget_destroy(GTK_WIDGET(dialog));
    } else {
        print_error(error ? error->message : "Erreur lors du chargement de entraineurs.glade");
        if (error) g_error_free(error);
    }
    g_object_unref(trainers_builder);
}

void on_menu_equipment(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    // Équipements: Admin (full access), Trainer (reserve equipment)
    if (!can_access_admin_features() && !can_access_trainer_features()) {
        show_permission_error("le module Équipements");
        return;
    }
    
    print_info("Ouverture du module Équipements");
    
    GError* error = NULL;
    GtkBuilder* equipment_builder = gtk_builder_new();
    
    if (gtk_builder_add_from_file(equipment_builder, "ui/equipements.glade", &error)) {
        GtkDialog* dialog = GTK_DIALOG(gtk_builder_get_object(equipment_builder, "equipements_dialog"));
        gtk_builder_connect_signals(equipment_builder, NULL);
        gtk_widget_show_all(GTK_WIDGET(dialog));
        gtk_dialog_run(dialog);
        gtk_widget_destroy(GTK_WIDGET(dialog));
    } else {
        print_error(error ? error->message : "Erreur lors du chargement de equipements.glade");
        if (error) g_error_free(error);
    }
    g_object_unref(equipment_builder);
}

void on_menu_events(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    // Événements: Accessible à tous (Admin, Member, Trainer)
    // Pas de restriction - tous peuvent voir/s'inscrire aux événements
    
    print_info("Ouverture du module Événements");
    
    GError* error = NULL;
    GtkBuilder* events_builder = gtk_builder_new();
    
    if (gtk_builder_add_from_file(events_builder, "ui/evenements.glade", &error)) {
        GtkDialog* dialog = GTK_DIALOG(gtk_builder_get_object(events_builder, "evenements_dialog"));
        gtk_builder_connect_signals(events_builder, NULL);
        gtk_widget_show_all(GTK_WIDGET(dialog));
        gtk_dialog_run(dialog);
        gtk_widget_destroy(GTK_WIDGET(dialog));
    } else {
        print_error(error ? error->message : "Erreur lors du chargement de evenements.glade");
        if (error) g_error_free(error);
    }
    g_object_unref(events_builder);
}

// ============= Module CRUD Stubs =============

// Membres
void on_membres_add(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (current_role != ROLE_ADMIN) {
        show_permission_error("l'ajout de membres");
        return;
    }
    
    print_info("Ajouter membre - Fonction à implémenter avec formulaire");
    ajouter_membre();
}

void on_membres_edit(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (current_role != ROLE_ADMIN) {
        show_permission_error("la modification de membres");
        return;
    }
    
    print_info("Modifier membre - Sélection requise");
}

void on_membres_delete(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (current_role != ROLE_ADMIN) {
        show_permission_error("la suppression de membres");
        return;
    }
    
    if (!global_membres_builder) {
        print_error("Builder non disponible");
        return;
    }
    
    // Récupérer le TreeView et la sélection
    GtkTreeView* treeview = GTK_TREE_VIEW(gtk_builder_get_object(global_membres_builder, "membres_treeview"));
    GtkTreeSelection* selection = gtk_tree_view_get_selection(treeview);
    GtkTreeIter iter;
    GtkTreeModel* model;
    
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        GtkWidget* error_dialog = gtk_message_dialog_new(
            main_window,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Veuillez sélectionner un membre à supprimer"
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Récupérer l'ID
    int id;
    gtk_tree_model_get(model, &iter, 0, &id, -1);
    
    // Confirmation
    GtkWidget* confirm = gtk_message_dialog_new(
        main_window,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "Confirmer la suppression du membre ID %d ?",
        id
    );
    
    int response = gtk_dialog_run(GTK_DIALOG(confirm));
    gtk_widget_destroy(confirm);
    
    if (response == GTK_RESPONSE_YES) {
        supprimer_membre(id);
        on_membres_refresh(NULL, NULL);
    }
}

void on_membres_search(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (!global_membres_builder) {
        return;
    }
    
    GtkEntry* search_entry = GTK_ENTRY(gtk_builder_get_object(global_membres_builder, "search_entry"));
    const char* query = gtk_entry_get_text(search_entry);
    
    if (strlen(query) == 0) {
        on_membres_refresh(NULL, NULL);
        return;
    }
    
    // Charger tous les membres
    Membre* membres = NULL;
    int count = charger_membres(&membres);
    
    if (count == 0 || membres == NULL) {
        return;
    }
    
    // Filtrer et afficher
    GtkListStore* store = GTK_LIST_STORE(gtk_builder_get_object(global_membres_builder, "membres_liststore"));
    gtk_list_store_clear(store);
    
    for (int i = 0; i < count; i++) {
        // Rechercher dans nom, prénom, email
        if (strstr(membres[i].nom, query) ||
            strstr(membres[i].prenom, query) ||
            strstr(membres[i].email, query)) {
            
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                0, membres[i].id,
                1, membres[i].nom,
                2, membres[i].prenom,
                3, membres[i].email,
                4, membres[i].telephone,
                5, membres[i].dateInscription,
                6, membres[i].actif ? "Oui" : "Non",
                -1);
        }
    }
    
    free(membres);
    print_info("Recherche effectuée");
}

void on_membres_refresh(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (!global_membres_builder) {
        print_error("Builder non disponible");
        return;
    }
    
    // Charger les membres
    Membre* membres = NULL;
    int count = charger_membres(&membres);
    
    // Récupérer le ListStore
    GtkListStore* store = GTK_LIST_STORE(gtk_builder_get_object(global_membres_builder, "membres_liststore"));
    gtk_list_store_clear(store);
    
    if (count == 0 || membres == NULL) {
        print_info("Aucun membre à afficher");
        return;
    }
    
    // Remplir le ListStore
    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, membres[i].id,
            1, membres[i].nom,
            2, membres[i].prenom,
            3, membres[i].email,
            4, membres[i].telephone,
            5, membres[i].dateInscription,
            6, membres[i].actif ? "Oui" : "Non",
            -1);
    }
    
    free(membres);
    print_info("Membres chargés");
}

// Centres
void on_centres_add(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    if (current_role != ROLE_ADMIN) {
        show_permission_error("l'ajout de centres");
        return;
    }
    print_info("Ajouter centre - Formulaire à implémenter");
    ajouter_centre();
}

void on_centres_edit(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    if (current_role != ROLE_ADMIN) {
        show_permission_error("la modification de centres");
        return;
    }
    print_info("Modifier centre - Sélection requise");
}

void on_centres_delete(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    if (current_role != ROLE_ADMIN) {
        show_permission_error("la suppression de centres");
        return;
    }
    
    if (!global_centres_builder) return;
    
    GtkTreeView* treeview = GTK_TREE_VIEW(gtk_builder_get_object(global_centres_builder, "centres_treeview"));
    GtkTreeSelection* selection = gtk_tree_view_get_selection(treeview);
    GtkTreeIter iter;
    GtkTreeModel* model;
    
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        GtkWidget* error_dialog = gtk_message_dialog_new(main_window, GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Sélectionnez un centre");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    int id;
    gtk_tree_model_get(model, &iter, 0, &id, -1);
    
    GtkWidget* confirm = gtk_message_dialog_new(main_window, GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Supprimer le centre ID %d ?", id);
    
    if (gtk_dialog_run(GTK_DIALOG(confirm)) == GTK_RESPONSE_YES) {
        supprimer_centre(id);
        on_centres_refresh(NULL, NULL);
    }
    gtk_widget_destroy(confirm);
}

void on_centres_refresh(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    if (!global_centres_builder) return;
    
    Centre* centres = NULL;
    int count = charger_centres(&centres);
    
    GtkListStore* store = GTK_LIST_STORE(gtk_builder_get_object(global_centres_builder, "centres_liststore"));
    gtk_list_store_clear(store);
    
    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, centres[i].id,
            1, centres[i].nom,
            2, centres[i].adresse,
            3, centres[i].telephone,
            4, centres[i].capacite,
            -1);
    }
    
    free(centres);
    print_info("Centres chargés");
}

// Cours
void on_cours_add(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    if (current_role != ROLE_ADMIN) {
        show_permission_error("l'ajout de cours");
        return;
    }
    print_info("Ajouter cours - Formulaire à implémenter");
    ajouter_cours();
}

void on_cours_edit(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    if (current_role != ROLE_ADMIN) {
        show_permission_error("la modification de cours");
        return;
    }
    print_info("Modifier cours - Sélection requise");
}

void on_cours_delete(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    print_info("Supprimer cours");
}

void on_cours_refresh(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    if (!global_cours_builder) return;
    
    Cours* cours = NULL;
    int count = charger_cours(&cours);
    
    GtkListStore* store = GTK_LIST_STORE(gtk_builder_get_object(global_cours_builder, "cours_liststore"));
    gtk_list_store_clear(store);
    
    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, cours[i].id,
            1, cours[i].nom,
            2, cours[i].type,
            3, cours[i].horaire,
            4, cours[i].capaciteMax,
            5, cours[i].inscrits,
            6, cours[i].entraineurId,
            7, cours[i].centreId,
            -1);
    }
    
    free(cours);
    print_info("Cours chargés");
}

// Trainers
void on_trainers_add(GtkWidget* widget, gpointer data) {
    print_info("Ajouter entraîneur");
    ajouter_entraineur();
}

void on_trainers_edit(GtkWidget* widget, gpointer data) {
    print_info("Modifier entraîneur");
}

void on_trainers_delete(GtkWidget* widget, gpointer data) {
    print_info("Supprimer entraîneur");
}

void on_trainers_refresh(GtkWidget* widget, gpointer data) {
    print_info("Rafraîchir entraîneurs");
    afficher_tous_entraineurs();
}

// Equipment
void on_equipment_add(GtkWidget* widget, gpointer data) {
    print_info("Ajouter équipement");
    ajouter_equipement();
}

void on_equipment_edit(GtkWidget* widget, gpointer data) {
    print_info("Modifier équipement");
}

void on_equipment_delete(GtkWidget* widget, gpointer data) {
    print_info("Supprimer équipement");
}

void on_equipment_refresh(GtkWidget* widget, gpointer data) {
    print_info("Rafraîchir équipements");
    afficher_tous_equipements();
}

// Events
void on_events_add(GtkWidget* widget, gpointer data) {
    print_info("Ajouter événement");
    ajouter_evenement();
}

void on_events_edit(GtkWidget* widget, gpointer data) {
    print_info("Modifier événement");
}

void on_events_delete(GtkWidget* widget, gpointer data) {
    print_info("Supprimer événement");
}

void on_events_refresh(GtkWidget* widget, gpointer data) {
    print_info("Rafraîchir événements");
    afficher_tous_evenements();
}

// ============= GTK Initialization =============

static gboolean setup_ui() {
    GError* error = NULL;
    
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "ui/main.glade", &error)) {
        if (error) {
            fprintf(stderr, "Erreur lors du chargement du fichier Glade: %s\n", error->message);
            g_error_free(error);
        }
        return FALSE;
    }
    
    // Connect signals
    gtk_builder_connect_signals(builder, NULL);
    
    // Get windows
    login_window = GTK_WINDOW(gtk_builder_get_object(builder, "login_window"));
    main_window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    
    if (!login_window || !main_window) {
        print_error("Impossible de charger les fenêtres depuis Glade");
        return FALSE;
    }
    
    // Initialize role combo box with default selection (Membre = index 1)
    GtkComboBoxText* role_combo = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "role_combo"));
    if (role_combo) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(role_combo), 1); // Select "Membre" by default
        print_info("Rôle par défaut: Membre");
    }
    
    return TRUE;
}

// ============= Main =============

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);
    
    print_info("Démarrage de l'application Gestion des Centres Sportifs");
    create_data_directory();
    
    if (!setup_ui()) {
        print_error("Impossible de charger l'interface");
        return 1;
    }
    
    gtk_widget_show(GTK_WIDGET(login_window));
    gtk_main();
    
    if (builder) {
        g_object_unref(builder);
    }
    
    return 0;
}
