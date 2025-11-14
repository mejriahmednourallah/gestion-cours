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
    
    // Membres: Admin (full access), Member (view only)
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
        
        // Masquer/afficher boutons selon rôle
        GtkWidget* btn_add = GTK_WIDGET(gtk_builder_get_object(global_membres_builder, "btn_membres_add"));
        GtkWidget* btn_edit = GTK_WIDGET(gtk_builder_get_object(global_membres_builder, "btn_membres_edit"));
        GtkWidget* btn_delete = GTK_WIDGET(gtk_builder_get_object(global_membres_builder, "btn_membres_delete"));
        
        if (current_role == ROLE_ADMIN) {
            // Admin: tout visible
            if (btn_add) gtk_widget_set_visible(btn_add, TRUE);
            if (btn_edit) gtk_widget_set_visible(btn_edit, TRUE);
            if (btn_delete) gtk_widget_set_visible(btn_delete, TRUE);
        } else {
            // Member/Trainer: masquer CRUD
            if (btn_add) gtk_widget_set_visible(btn_add, FALSE);
            if (btn_edit) gtk_widget_set_visible(btn_edit, FALSE);
            if (btn_delete) gtk_widget_set_visible(btn_delete, FALSE);
        }
        
        gtk_widget_show_all(GTK_WIDGET(dialog));
        
        // Charger les données après affichage
        on_membres_refresh(NULL, NULL);
        
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
        
        // Masquer/afficher boutons selon rôle
        GtkWidget* btn_add = GTK_WIDGET(gtk_builder_get_object(global_centres_builder, "centres_btn_add"));
        GtkWidget* btn_edit = GTK_WIDGET(gtk_builder_get_object(global_centres_builder, "centres_btn_edit"));
        GtkWidget* btn_delete = GTK_WIDGET(gtk_builder_get_object(global_centres_builder, "centres_btn_delete"));
        
        if (current_role == ROLE_ADMIN) {
            if (btn_add) gtk_widget_set_visible(btn_add, TRUE);
            if (btn_edit) gtk_widget_set_visible(btn_edit, TRUE);
            if (btn_delete) gtk_widget_set_visible(btn_delete, TRUE);
        } else {
            if (btn_add) gtk_widget_set_visible(btn_add, FALSE);
            if (btn_edit) gtk_widget_set_visible(btn_edit, FALSE);
            if (btn_delete) gtk_widget_set_visible(btn_delete, FALSE);
        }
        
        gtk_widget_show_all(GTK_WIDGET(dialog));
        on_centres_refresh(NULL, NULL);
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
    
    print_info("Ouverture du module Cours");
    
    GError* error = NULL;
    global_cours_builder = gtk_builder_new();
    
    if (gtk_builder_add_from_file(global_cours_builder, "ui/cours.glade", &error)) {
        GtkDialog* dialog = GTK_DIALOG(gtk_builder_get_object(global_cours_builder, "cours_dialog"));
        gtk_builder_connect_signals(global_cours_builder, NULL);
        
        // Masquer/afficher boutons selon rôle
        GtkWidget* btn_add = GTK_WIDGET(gtk_builder_get_object(global_cours_builder, "cours_btn_add"));
        GtkWidget* btn_edit = GTK_WIDGET(gtk_builder_get_object(global_cours_builder, "cours_btn_edit"));
        GtkWidget* btn_delete = GTK_WIDGET(gtk_builder_get_object(global_cours_builder, "cours_btn_delete"));
        GtkWidget* btn_enroll = GTK_WIDGET(gtk_builder_get_object(global_cours_builder, "cours_btn_enroll"));
        GtkWidget* btn_join = GTK_WIDGET(gtk_builder_get_object(global_cours_builder, "cours_btn_join_trainer"));
        
        if (current_role == ROLE_ADMIN) {
            // Admin: voir Add/Edit/Delete, masquer Member/Trainer buttons
            if (btn_add) gtk_widget_set_visible(btn_add, TRUE);
            if (btn_edit) gtk_widget_set_visible(btn_edit, TRUE);
            if (btn_delete) gtk_widget_set_visible(btn_delete, TRUE);
            if (btn_enroll) gtk_widget_set_visible(btn_enroll, FALSE);
            if (btn_join) gtk_widget_set_visible(btn_join, FALSE);
        } else if (current_role == ROLE_MEMBER) {
            // Member: masquer CRUD, afficher S'inscrire
            if (btn_add) gtk_widget_set_visible(btn_add, FALSE);
            if (btn_edit) gtk_widget_set_visible(btn_edit, FALSE);
            if (btn_delete) gtk_widget_set_visible(btn_delete, FALSE);
            if (btn_enroll) gtk_widget_set_visible(btn_enroll, TRUE);
            if (btn_join) gtk_widget_set_visible(btn_join, FALSE);
        } else if (current_role == ROLE_TRAINER) {
            // Trainer: masquer CRUD, afficher Rejoindre
            if (btn_add) gtk_widget_set_visible(btn_add, FALSE);
            if (btn_edit) gtk_widget_set_visible(btn_edit, FALSE);
            if (btn_delete) gtk_widget_set_visible(btn_delete, FALSE);
            if (btn_enroll) gtk_widget_set_visible(btn_enroll, FALSE);
            if (btn_join) gtk_widget_set_visible(btn_join, TRUE);
        }
        
        gtk_widget_show_all(GTK_WIDGET(dialog));
        on_cours_refresh(NULL, NULL);
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
    global_entraineurs_builder = gtk_builder_new();
    
    if (gtk_builder_add_from_file(global_entraineurs_builder, "ui/entraineurs.glade", &error)) {
        GtkDialog* dialog = GTK_DIALOG(gtk_builder_get_object(global_entraineurs_builder, "entraineurs_dialog"));
        gtk_builder_connect_signals(global_entraineurs_builder, NULL);
        
        gtk_widget_show_all(GTK_WIDGET(dialog));
        
        // Charger les données après affichage
        on_trainers_refresh(NULL, NULL);
        
        gtk_dialog_run(dialog);
        gtk_widget_destroy(GTK_WIDGET(dialog));
    } else {
        print_error(error ? error->message : "Erreur lors du chargement de entraineurs.glade");
        if (error) g_error_free(error);
    }
    
    if (global_entraineurs_builder) {
        g_object_unref(global_entraineurs_builder);
        global_entraineurs_builder = NULL;
    }
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
    global_equipements_builder = gtk_builder_new();
    
    if (gtk_builder_add_from_file(global_equipements_builder, "ui/equipements.glade", &error)) {
        GtkDialog* dialog = GTK_DIALOG(gtk_builder_get_object(global_equipements_builder, "equipements_dialog"));
        gtk_builder_connect_signals(global_equipements_builder, NULL);
        
        // Masquer/afficher boutons selon rôle
        GtkWidget* btn_add = GTK_WIDGET(gtk_builder_get_object(global_equipements_builder, "equipment_btn_add"));
        GtkWidget* btn_edit = GTK_WIDGET(gtk_builder_get_object(global_equipements_builder, "equipment_btn_edit"));
        GtkWidget* btn_delete = GTK_WIDGET(gtk_builder_get_object(global_equipements_builder, "equipment_btn_delete"));
        GtkWidget* btn_reserve = GTK_WIDGET(gtk_builder_get_object(global_equipements_builder, "equipment_btn_reserve"));
        
        if (current_role == ROLE_ADMIN) {
            if (btn_add) gtk_widget_set_visible(btn_add, TRUE);
            if (btn_edit) gtk_widget_set_visible(btn_edit, TRUE);
            if (btn_delete) gtk_widget_set_visible(btn_delete, TRUE);
            if (btn_reserve) gtk_widget_set_visible(btn_reserve, FALSE);
        } else if (current_role == ROLE_TRAINER) {
            if (btn_add) gtk_widget_set_visible(btn_add, FALSE);
            if (btn_edit) gtk_widget_set_visible(btn_edit, FALSE);
            if (btn_delete) gtk_widget_set_visible(btn_delete, FALSE);
            if (btn_reserve) gtk_widget_set_visible(btn_reserve, TRUE);
        }
        
        gtk_widget_show_all(GTK_WIDGET(dialog));
        
        // Charger les données après affichage
        on_equipment_refresh(NULL, NULL);
        
        gtk_dialog_run(dialog);
        gtk_widget_destroy(GTK_WIDGET(dialog));
    } else {
        print_error(error ? error->message : "Erreur lors du chargement de equipements.glade");
        if (error) g_error_free(error);
    }
    
    if (global_equipements_builder) {
        g_object_unref(global_equipements_builder);
        global_equipements_builder = NULL;
    }
}

void on_menu_events(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    // Événements: Accessible à tous (Admin, Member, Trainer)
    // Pas de restriction - tous peuvent voir/s'inscrire aux événements
    
    print_info("Ouverture du module Événements");
    
    GError* error = NULL;
    global_evenements_builder = gtk_builder_new();
    
    if (gtk_builder_add_from_file(global_evenements_builder, "ui/evenements.glade", &error)) {
        GtkDialog* dialog = GTK_DIALOG(gtk_builder_get_object(global_evenements_builder, "evenements_dialog"));
        gtk_builder_connect_signals(global_evenements_builder, NULL);
        
        // Masquer/afficher boutons selon rôle
        GtkWidget* btn_add = GTK_WIDGET(gtk_builder_get_object(global_evenements_builder, "events_btn_add"));
        GtkWidget* btn_edit = GTK_WIDGET(gtk_builder_get_object(global_evenements_builder, "events_btn_edit"));
        GtkWidget* btn_delete = GTK_WIDGET(gtk_builder_get_object(global_evenements_builder, "events_btn_delete"));
        GtkWidget* btn_enroll = GTK_WIDGET(gtk_builder_get_object(global_evenements_builder, "events_btn_enroll"));
        
        if (current_role == ROLE_ADMIN) {
            if (btn_add) gtk_widget_set_visible(btn_add, TRUE);
            if (btn_edit) gtk_widget_set_visible(btn_edit, TRUE);
            if (btn_delete) gtk_widget_set_visible(btn_delete, TRUE);
            if (btn_enroll) gtk_widget_set_visible(btn_enroll, FALSE);
        } else {
            // Member/Trainer: masquer CRUD, afficher S'inscrire
            if (btn_add) gtk_widget_set_visible(btn_add, FALSE);
            if (btn_edit) gtk_widget_set_visible(btn_edit, FALSE);
            if (btn_delete) gtk_widget_set_visible(btn_delete, FALSE);
            if (btn_enroll) gtk_widget_set_visible(btn_enroll, TRUE);
        }
        
        gtk_widget_show_all(GTK_WIDGET(dialog));
        
        // Charger les données après affichage
        on_events_refresh(NULL, NULL);
        
        gtk_dialog_run(dialog);
        gtk_widget_destroy(GTK_WIDGET(dialog));
    } else {
        print_error(error ? error->message : "Erreur lors du chargement de evenements.glade");
        if (error) g_error_free(error);
    }
    
    if (global_evenements_builder) {
        g_object_unref(global_evenements_builder);
        global_evenements_builder = NULL;
    }
}

// ============= Module CRUD Stubs =============

// Membres
void on_membres_add(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (current_role != ROLE_ADMIN) {
        show_permission_error("l'ajout de membres");
        return;
    }
    
    // Créer dialog avec formulaire
    GtkWidget* dialog = gtk_dialog_new_with_buttons(
        "Ajouter un membre",
        NULL,
        GTK_DIALOG_MODAL,
        "Annuler", GTK_RESPONSE_CANCEL,
        "Ajouter", GTK_RESPONSE_ACCEPT,
        NULL);
    
    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(content), grid);
    
    // Champs du formulaire
    GtkWidget* label_nom = gtk_label_new("Nom:");
    GtkWidget* entry_nom = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), label_nom, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nom, 1, 0, 1, 1);
    
    GtkWidget* label_prenom = gtk_label_new("Prénom:");
    GtkWidget* entry_prenom = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), label_prenom, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_prenom, 1, 1, 1, 1);
    
    GtkWidget* label_email = gtk_label_new("Email:");
    GtkWidget* entry_email = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), label_email, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_email, 1, 2, 1, 1);
    
    GtkWidget* label_tel = gtk_label_new("Téléphone:");
    GtkWidget* entry_tel = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), label_tel, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_tel, 1, 3, 1, 1);
    
    gtk_widget_show_all(dialog);
    
    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        const char* nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
        const char* prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
        const char* email = gtk_entry_get_text(GTK_ENTRY(entry_email));
        const char* tel = gtk_entry_get_text(GTK_ENTRY(entry_tel));
        
        // Validation
        if (strlen(nom) == 0 || strlen(prenom) == 0 || strlen(email) == 0) {
            GtkWidget* error = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                "Veuillez remplir tous les champs obligatoires");
            gtk_dialog_run(GTK_DIALOG(error));
            gtk_widget_destroy(error);
        } else {
            // Charger membres existants pour générer ID
            Membre* membres = NULL;
            int count = charger_membres(&membres);
            int new_id = 1;
            if (count > 0) {
                // Trouver ID max
                for (int i = 0; i < count; i++) {
                    if (membres[i].id >= new_id) new_id = membres[i].id + 1;
                }
            }
            
            // Ajouter au fichier
            FILE* f = fopen("data/membres.txt", "a");
            if (f) {
                fprintf(f, "%d,%s,%s,%s,%s,%s,1\n",
                        new_id, nom, prenom, email, tel, get_current_date());
                fclose(f);
                print_info("Membre ajouté avec succès");
                on_membres_refresh(NULL, NULL);
            }
            
            if (membres) free(membres);
        }
    }
    
    gtk_widget_destroy(dialog);
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
    
    // Utiliser le builder du dialogue s'il existe, sinon le builder principal
    GtkBuilder* active_builder = global_membres_builder ? global_membres_builder : builder;
    
    if (!active_builder) {
        print_error("Builder non disponible");
        return;
    }
    
    // Charger les membres
    Membre* membres = NULL;
    int count = charger_membres(&membres);
    printf("[DEBUG] Chargé %d membres\n", count);
    
    // Récupérer le ListStore
    GObject* obj = gtk_builder_get_object(active_builder, "membres_liststore");
    if (!obj) {
        print_error("ListStore 'membres_liststore' introuvable");
        free(membres);
        return;
    }
    
    GtkListStore* store = GTK_LIST_STORE(obj);
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
    char msg[100];
    snprintf(msg, sizeof(msg), "Membres chargés: %d", count);
    print_info(msg);
}

// Centres
void on_centres_add(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    if (current_role != ROLE_ADMIN) {
        show_permission_error("l'ajout de centres");
        return;
    }
    
    GtkWidget* dialog = gtk_dialog_new_with_buttons(
        "Ajouter un centre",
        NULL,
        GTK_DIALOG_MODAL,
        "Annuler", GTK_RESPONSE_CANCEL,
        "Ajouter", GTK_RESPONSE_ACCEPT,
        NULL);
    
    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(content), grid);
    
    GtkWidget* label_nom = gtk_label_new("Nom:");
    GtkWidget* entry_nom = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), label_nom, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nom, 1, 0, 1, 1);
    
    GtkWidget* label_adresse = gtk_label_new("Adresse:");
    GtkWidget* entry_adresse = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), label_adresse, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_adresse, 1, 1, 1, 1);
    
    GtkWidget* label_tel = gtk_label_new("Téléphone:");
    GtkWidget* entry_tel = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), label_tel, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_tel, 1, 2, 1, 1);
    
    GtkWidget* label_capacite = gtk_label_new("Capacité:");
    GtkWidget* entry_capacite = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_capacite), "100");
    gtk_grid_attach(GTK_GRID(grid), label_capacite, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_capacite, 1, 3, 1, 1);
    
    gtk_widget_show_all(dialog);
    
    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        const char* nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
        const char* adresse = gtk_entry_get_text(GTK_ENTRY(entry_adresse));
        const char* tel = gtk_entry_get_text(GTK_ENTRY(entry_tel));
        const char* capacite_str = gtk_entry_get_text(GTK_ENTRY(entry_capacite));
        
        if (strlen(nom) == 0 || strlen(adresse) == 0) {
            GtkWidget* error = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                "Veuillez remplir tous les champs obligatoires");
            gtk_dialog_run(GTK_DIALOG(error));
            gtk_widget_destroy(error);
        } else {
            int capacite = atoi(capacite_str);
            Centre* centres = NULL;
            int count = charger_centres(&centres);
            int new_id = 1;
            if (count > 0) {
                for (int i = 0; i < count; i++) {
                    if (centres[i].id >= new_id) new_id = centres[i].id + 1;
                }
            }
            
            FILE* f = fopen("data/centres.txt", "a");
            if (f) {
                fprintf(f, "%d,%s,%s,%s,%d\n", new_id, nom, adresse, tel, capacite);
                fclose(f);
                print_info("Centre ajouté avec succès");
                on_centres_refresh(NULL, NULL);
            }
            
            if (centres) free(centres);
        }
    }
    
    gtk_widget_destroy(dialog);
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
    
    // Utiliser le builder du dialogue s'il existe, sinon le builder principal
    GtkBuilder* active_builder = global_centres_builder ? global_centres_builder : builder;
    
    if (!active_builder) {
        print_error("Builder non disponible");
        return;
    }
    
    Centre* centres = NULL;
    int count = charger_centres(&centres);
    printf("[DEBUG] Chargé %d centres\n", count);
    
    GObject* obj = gtk_builder_get_object(active_builder, "centres_liststore");
    if (!obj) {
        print_error("ListStore 'centres_liststore' introuvable");
        free(centres);
        return;
    }
    
    GtkListStore* store = GTK_LIST_STORE(obj);
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
    char msg[100];
    snprintf(msg, sizeof(msg), "Centres chargés: %d", count);
    print_info(msg);
}

// Cours
void on_cours_add(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    if (current_role != ROLE_ADMIN) {
        show_permission_error("l'ajout de cours");
        return;
    }
    
    GtkWidget* dialog = gtk_dialog_new_with_buttons(
        "Ajouter un cours",
        NULL, GTK_DIALOG_MODAL,
        "Annuler", GTK_RESPONSE_CANCEL,
        "Ajouter", GTK_RESPONSE_ACCEPT,
        NULL);
    
    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(content), grid);
    
    GtkWidget* entry_nom = gtk_entry_new();
    GtkWidget* entry_type = gtk_entry_new();
    GtkWidget* entry_horaire = gtk_entry_new();
    GtkWidget* entry_capa = gtk_entry_new();
    GtkWidget* entry_centre = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_horaire), "Lun-Mer 10h-11h");
    gtk_entry_set_text(GTK_ENTRY(entry_capa), "20");
    gtk_entry_set_text(GTK_ENTRY(entry_centre), "1");
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nom:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nom, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Type:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_type, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Horaire:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_horaire, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Capacité max:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_capa, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Centre ID:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_centre, 1, 4, 1, 1);
    
    gtk_widget_show_all(dialog);
    
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        const char* nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
        const char* type = gtk_entry_get_text(GTK_ENTRY(entry_type));
        const char* horaire = gtk_entry_get_text(GTK_ENTRY(entry_horaire));
        int capa = atoi(gtk_entry_get_text(GTK_ENTRY(entry_capa)));
        int centre = atoi(gtk_entry_get_text(GTK_ENTRY(entry_centre)));
        
        if (strlen(nom) > 0) {
            Cours* arr = NULL;
            int count = charger_cours(&arr);
            int new_id = 1;
            for (int i = 0; i < count; i++) {
                if (arr[i].id >= new_id) new_id = arr[i].id + 1;
            }
            
            FILE* f = fopen("data/cours.txt", "a");
            if (f) {
                fprintf(f, "%d,%s,%s,%s,0,%d,0,%d\n", new_id, nom, type, horaire, capa, centre);
                fclose(f);
                print_info("Cours ajouté");
                on_cours_refresh(NULL, NULL);
            }
            if (arr) free(arr);
        }
    }
    gtk_widget_destroy(dialog);
}

// Fonction Member: S'inscrire à un cours
void on_cours_enroll(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (!can_access_member_features()) {
        show_permission_error("l'inscription aux cours");
        return;
    }
    
    if (!global_cours_builder) {
        print_error("Builder cours non disponible");
        return;
    }
    
    GtkTreeView* tree = GTK_TREE_VIEW(gtk_builder_get_object(global_cours_builder, "cours_treeview"));
    if (!tree) return;
    
    GtkTreeSelection* selection = gtk_tree_view_get_selection(tree);
    GtkTreeModel* model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gint id, capacite_max, inscrits;
        gchar* nom;
        gtk_tree_model_get(model, &iter, 0, &id, 1, &nom, 5, &capacite_max, 6, &inscrits, -1);
        
        if (inscrits >= capacite_max) {
            GtkWidget* error = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                "Ce cours est complet (%d/%d inscrits)", inscrits, capacite_max);
            gtk_dialog_run(GTK_DIALOG(error));
            gtk_widget_destroy(error);
            g_free(nom);
            return;
        }
        
        gchar* msg = g_strdup_printf("S'inscrire au cours %s ?\n\nPlaces: %d/%d", nom, inscrits + 1, capacite_max);
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "%s", msg);
        g_free(msg);
        
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        if (response == GTK_RESPONSE_YES) {
            Cours* cours = NULL;
            int count = charger_cours(&cours);
            
            for (int i = 0; i < count; i++) {
                if (cours[i].id == id) {
                    cours[i].inscrits++;
                    break;
                }
            }
            
            FILE* f = fopen("data/cours.txt", "w");
            if (f) {
                for (int i = 0; i < count; i++) {
                    fprintf(f, "%d,%s,%s,%s,%d,%d,%d,%d\n",
                            cours[i].id, cours[i].nom, cours[i].type,
                            cours[i].horaire, cours[i].entraineurId,
                            cours[i].capaciteMax, cours[i].inscrits,
                            cours[i].centreId);
                }
                fclose(f);
                print_info("Inscription réussie!");
                on_cours_refresh(NULL, NULL);
            }
            
            if (cours) free(cours);
        }
        
        g_free(nom);
    } else {
        GtkWidget* error = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez sélectionner un cours");
        gtk_dialog_run(GTK_DIALOG(error));
        gtk_widget_destroy(error);
    }
}

// Fonction Trainer: Rejoindre cours comme coach
void on_cours_join_as_trainer(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (!can_access_trainer_features()) {
        show_permission_error("rejoindre un cours comme entraîneur");
        return;
    }
    
    if (!global_cours_builder) {
        print_error("Builder cours non disponible");
        return;
    }
    
    GtkTreeView* tree = GTK_TREE_VIEW(gtk_builder_get_object(global_cours_builder, "cours_treeview"));
    if (!tree) return;
    
    GtkTreeSelection* selection = gtk_tree_view_get_selection(tree);
    GtkTreeModel* model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gint id, entraineur_id;
        gchar* nom;
        gtk_tree_model_get(model, &iter, 0, &id, 1, &nom, 4, &entraineur_id, -1);
        
        if (entraineur_id != 0) {
            GtkWidget* error = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                "Ce cours a déjà un entraîneur (ID: %d)", entraineur_id);
            gtk_dialog_run(GTK_DIALOG(error));
            gtk_widget_destroy(error);
            g_free(nom);
            return;
        }
        
        gchar* msg = g_strdup_printf("Rejoindre le cours %s comme entraîneur ?", nom);
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "%s", msg);
        g_free(msg);
        
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        if (response == GTK_RESPONSE_YES) {
            int trainer_id = 1;
            
            Cours* cours = NULL;
            int count = charger_cours(&cours);
            
            for (int i = 0; i < count; i++) {
                if (cours[i].id == id) {
                    cours[i].entraineurId = trainer_id;
                    break;
                }
            }
            
            FILE* f = fopen("data/cours.txt", "w");
            if (f) {
                for (int i = 0; i < count; i++) {
                    fprintf(f, "%d,%s,%s,%s,%d,%d,%d,%d\n",
                            cours[i].id, cours[i].nom, cours[i].type,
                            cours[i].horaire, cours[i].entraineurId,
                            cours[i].capaciteMax, cours[i].inscrits,
                            cours[i].centreId);
                }
                fclose(f);
                print_info("Vous avez rejoint le cours!");
                on_cours_refresh(NULL, NULL);
            }
            
            if (cours) free(cours);
        }
        
        g_free(nom);
    } else {
        GtkWidget* error = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez sélectionner un cours");
        gtk_dialog_run(GTK_DIALOG(error));
        gtk_widget_destroy(error);
    }
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
    
    if (current_role != ROLE_ADMIN) {
        show_permission_error("la suppression de cours");
        return;
    }
    
    if (!global_cours_builder) {
        print_error("Builder cours non disponible");
        return;
    }
    
    GtkTreeView* tree = GTK_TREE_VIEW(gtk_builder_get_object(global_cours_builder, "cours_treeview"));
    if (!tree) return;
    
    GtkTreeSelection* selection = gtk_tree_view_get_selection(tree);
    GtkTreeModel* model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gint id;
        gchar* nom;
        gtk_tree_model_get(model, &iter, 0, &id, 1, &nom, -1);
        
        gchar* msg = g_strdup_printf("Supprimer le cours %s (ID: %d) ?", nom, id);
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "%s", msg);
        g_free(msg);
        
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        if (response == GTK_RESPONSE_YES) {
            supprimer_cours(id);
            on_cours_refresh(NULL, NULL);
        }
        
        g_free(nom);
    } else {
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez sélectionner un cours");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_cours_refresh(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    GtkBuilder* active_builder = global_cours_builder ? global_cours_builder : builder;
    if (!active_builder) return;
    
    Cours* cours = NULL;
    int count = charger_cours(&cours);
    
    GtkListStore* store = GTK_LIST_STORE(gtk_builder_get_object(active_builder, "cours_liststore"));
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
    (void)widget; (void)data;
    
    if (current_role != ROLE_ADMIN) {
        show_permission_error("l'ajout d'entraîneurs");
        return;
    }
    
    GtkWidget* dialog = gtk_dialog_new_with_buttons(
        "Ajouter un entraîneur",
        NULL, GTK_DIALOG_MODAL,
        "Annuler", GTK_RESPONSE_CANCEL,
        "Ajouter", GTK_RESPONSE_ACCEPT,
        NULL);
    
    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(content), grid);
    
    GtkWidget* entry_nom = gtk_entry_new();
    GtkWidget* entry_prenom = gtk_entry_new();
    GtkWidget* entry_specialite = gtk_entry_new();
    GtkWidget* entry_tel = gtk_entry_new();
    GtkWidget* entry_centre = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_centre), "1");
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nom:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nom, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Prénom:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_prenom, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Spécialité:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_specialite, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Téléphone:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_tel, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Centre ID:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_centre, 1, 4, 1, 1);
    
    gtk_widget_show_all(dialog);
    
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        const char* nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
        const char* prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
        const char* spec = gtk_entry_get_text(GTK_ENTRY(entry_specialite));
        const char* tel = gtk_entry_get_text(GTK_ENTRY(entry_tel));
        int centre = atoi(gtk_entry_get_text(GTK_ENTRY(entry_centre)));
        
        if (strlen(nom) > 0 && strlen(prenom) > 0) {
            Entraineur* arr = NULL;
            int count = charger_entraineurs(&arr);
            int new_id = 1;
            for (int i = 0; i < count; i++) {
                if (arr[i].id >= new_id) new_id = arr[i].id + 1;
            }
            
            FILE* f = fopen("data/entraineurs.txt", "a");
            if (f) {
                fprintf(f, "%d,%s,%s,%s,%s,1,%d\n", new_id, nom, prenom, spec, tel, centre);
                fclose(f);
                print_info("Entraîneur ajouté");
                on_trainers_refresh(NULL, NULL);
            }
            if (arr) free(arr);
        }
    }
    gtk_widget_destroy(dialog);
}

void on_trainers_edit(GtkWidget* widget, gpointer data) {
    print_info("Modifier entraîneur");
}

void on_trainers_delete(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (current_role != ROLE_ADMIN) {
        show_permission_error("la suppression d'entraîneurs");
        return;
    }
    
    if (!global_entraineurs_builder) {
        print_error("Builder entraineurs non disponible");
        return;
    }
    
    GtkTreeView* tree = GTK_TREE_VIEW(gtk_builder_get_object(global_entraineurs_builder, "entraineurs_treeview"));
    if (!tree) return;
    
    GtkTreeSelection* selection = gtk_tree_view_get_selection(tree);
    GtkTreeModel* model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gint id;
        gchar* nom;
        gchar* prenom;
        gtk_tree_model_get(model, &iter, 0, &id, 1, &nom, 2, &prenom, -1);
        
        gchar* msg = g_strdup_printf("Supprimer l'entraîneur %s %s (ID: %d) ?", prenom, nom, id);
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "%s", msg);
        g_free(msg);
        
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        if (response == GTK_RESPONSE_YES) {
            supprimer_entraineur(id);
            on_trainers_refresh(NULL, NULL);
        }
        
        g_free(nom);
        g_free(prenom);
    } else {
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez sélectionner un entraîneur");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_trainers_refresh(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    GtkBuilder* active_builder = global_entraineurs_builder ? global_entraineurs_builder : builder;
    if (!active_builder) {
        print_error("Builder entraineurs non disponible");
        return;
    }
    
    GtkTreeView* tree = GTK_TREE_VIEW(gtk_builder_get_object(active_builder, "entraineurs_treeview"));
    if (!tree) {
        print_error("TreeView entraineurs_treeview introuvable");
        return;
    }
    
    GtkListStore* store = GTK_LIST_STORE(gtk_tree_view_get_model(tree));
    if (!store) {
        print_error("ListStore introuvable");
        return;
    }
    
    gtk_list_store_clear(store);
    
    Entraineur* entraineurs = NULL;
    int count = charger_entraineurs(&entraineurs);
    
    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, entraineurs[i].id,
            1, entraineurs[i].nom,
            2, entraineurs[i].prenom,
            3, entraineurs[i].specialite,
            4, entraineurs[i].telephone,
            5, entraineurs[i].disponible ? "Oui" : "Non",
            6, entraineurs[i].centreId,
            -1);
    }
    
    free(entraineurs);
    print_info("Entraîneurs rechargés");
}

// Equipment
void on_equipment_add(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (current_role != ROLE_ADMIN) {
        show_permission_error("l'ajout d'équipements");
        return;
    }
    
    GtkWidget* dialog = gtk_dialog_new_with_buttons(
        "Ajouter un équipement",
        NULL, GTK_DIALOG_MODAL,
        "Annuler", GTK_RESPONSE_CANCEL,
        "Ajouter", GTK_RESPONSE_ACCEPT,
        NULL);
    
    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(content), grid);
    
    GtkWidget* entry_nom = gtk_entry_new();
    GtkWidget* entry_type = gtk_entry_new();
    GtkWidget* entry_qte = gtk_entry_new();
    GtkWidget* entry_centre = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_qte), "1");
    gtk_entry_set_text(GTK_ENTRY(entry_centre), "1");
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nom:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nom, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Type:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_type, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Quantité:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_qte, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Centre ID:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_centre, 1, 3, 1, 1);
    
    gtk_widget_show_all(dialog);
    
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        const char* nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
        const char* type = gtk_entry_get_text(GTK_ENTRY(entry_type));
        int qte = atoi(gtk_entry_get_text(GTK_ENTRY(entry_qte)));
        int centre = atoi(gtk_entry_get_text(GTK_ENTRY(entry_centre)));
        
        if (strlen(nom) > 0) {
            Equipement* arr = NULL;
            int count = charger_equipements(&arr);
            int new_id = 1;
            for (int i = 0; i < count; i++) {
                if (arr[i].id >= new_id) new_id = arr[i].id + 1;
            }
            
            FILE* f = fopen("data/equipements.txt", "a");
            if (f) {
                fprintf(f, "%d,%s,%s,%d,1,%d\n", new_id, nom, type, qte, centre);
                fclose(f);
                print_info("Équipement ajouté");
                on_equipment_refresh(NULL, NULL);
            }
            if (arr) free(arr);
        }
    }
    gtk_widget_destroy(dialog);
}

void on_equipment_edit(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    if (current_role != ROLE_ADMIN) {
        show_permission_error("la modification d'équipements");
        return;
    }
    print_info("Modifier équipement - Formulaire Edit à implémenter");
}

// Fonction Trainer: Réserver équipement
void on_equipment_reserve(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (!can_access_trainer_features()) {
        show_permission_error("la réservation d'équipements");
        return;
    }
    
    if (!global_equipements_builder) {
        print_error("Builder equipements non disponible");
        return;
    }
    
    GtkTreeView* tree = GTK_TREE_VIEW(gtk_builder_get_object(global_equipements_builder, "equipements_treeview"));
    if (!tree) return;
    
    GtkTreeSelection* selection = gtk_tree_view_get_selection(tree);
    GtkTreeModel* model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gint id, quantite, disponible;
        gchar* nom;
        gtk_tree_model_get(model, &iter, 0, &id, 1, &nom, 3, &quantite, 4, &disponible, -1);
        
        if (!disponible || quantite == 0) {
            GtkWidget* error = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                "Cet équipement n'est pas disponible");
            gtk_dialog_run(GTK_DIALOG(error));
            gtk_widget_destroy(error);
            g_free(nom);
            return;
        }
        
        gchar* msg = g_strdup_printf("Réserver %s ?\n\nQuantité disponible: %d", nom, quantite);
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "%s", msg);
        g_free(msg);
        
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        if (response == GTK_RESPONSE_YES) {
            Equipement* arr = NULL;
            int count = charger_equipements(&arr);
            
            for (int i = 0; i < count; i++) {
                if (arr[i].id == id) {
                    arr[i].disponible = 0; // Marquer comme réservé
                    break;
                }
            }
            
            FILE* f = fopen("data/equipements.txt", "w");
            if (f) {
                for (int i = 0; i < count; i++) {
                    fprintf(f, "%d,%s,%s,%d,%d,%d\n",
                            arr[i].id, arr[i].nom, arr[i].type,
                            arr[i].quantite, arr[i].disponible,
                            arr[i].centreId);
                }
                fclose(f);
                print_info("Équipement réservé!");
                on_equipment_refresh(NULL, NULL);
            }
            
            if (arr) free(arr);
        }
        
        g_free(nom);
    } else {
        GtkWidget* error = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez sélectionner un équipement");
        gtk_dialog_run(GTK_DIALOG(error));
        gtk_widget_destroy(error);
    }
}

void on_equipment_delete(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (current_role != ROLE_ADMIN) {
        show_permission_error("la suppression d'équipements");
        return;
    }
    
    if (!global_equipements_builder) {
        print_error("Builder equipements non disponible");
        return;
    }
    
    GtkTreeView* tree = GTK_TREE_VIEW(gtk_builder_get_object(global_equipements_builder, "equipements_treeview"));
    if (!tree) return;
    
    GtkTreeSelection* selection = gtk_tree_view_get_selection(tree);
    GtkTreeModel* model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gint id;
        gchar* nom;
        gtk_tree_model_get(model, &iter, 0, &id, 1, &nom, -1);
        
        gchar* msg = g_strdup_printf("Supprimer l'équipement %s (ID: %d) ?", nom, id);
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "%s", msg);
        g_free(msg);
        
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        if (response == GTK_RESPONSE_YES) {
            supprimer_equipement(id);
            on_equipment_refresh(NULL, NULL);
        }
        
        g_free(nom);
    } else {
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez sélectionner un équipement");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_equipment_refresh(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    GtkBuilder* active_builder = global_equipements_builder ? global_equipements_builder : builder;
    if (!active_builder) {
        print_error("Builder equipements non disponible");
        return;
    }
    
    GtkTreeView* tree = GTK_TREE_VIEW(gtk_builder_get_object(active_builder, "equipements_treeview"));
    if (!tree) {
        print_error("TreeView equipements_treeview introuvable");
        return;
    }
    
    GtkListStore* store = GTK_LIST_STORE(gtk_tree_view_get_model(tree));
    if (!store) {
        print_error("ListStore introuvable");
        return;
    }
    
    gtk_list_store_clear(store);
    
    Equipement* equipements = NULL;
    int count = charger_equipements(&equipements);
    
    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, equipements[i].id,
            1, equipements[i].nom,
            2, equipements[i].type,
            3, equipements[i].quantite,
            4, equipements[i].disponible ? "Oui" : "Non",
            5, equipements[i].centreId,
            -1);
    }
    
    free(equipements);
    print_info("Équipements rechargés");
}

// Events
void on_events_add(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (current_role != ROLE_ADMIN) {
        show_permission_error("l'ajout d'événements");
        return;
    }
    
    GtkWidget* dialog = gtk_dialog_new_with_buttons(
        "Ajouter un événement",
        NULL, GTK_DIALOG_MODAL,
        "Annuler", GTK_RESPONSE_CANCEL,
        "Ajouter", GTK_RESPONSE_ACCEPT,
        NULL);
    
    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(content), grid);
    
    GtkWidget* entry_nom = gtk_entry_new();
    GtkWidget* entry_desc = gtk_entry_new();
    GtkWidget* entry_date = gtk_entry_new();
    GtkWidget* entry_centre = gtk_entry_new();
    GtkWidget* entry_capa = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_date), "2024-12-01");
    gtk_entry_set_text(GTK_ENTRY(entry_centre), "1");
    gtk_entry_set_text(GTK_ENTRY(entry_capa), "50");
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nom:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nom, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Description:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_desc, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Date (YYYY-MM-DD):"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_date, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Centre ID:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_centre, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Capacité max:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_capa, 1, 4, 1, 1);
    
    gtk_widget_show_all(dialog);
    
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        const char* nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
        const char* desc = gtk_entry_get_text(GTK_ENTRY(entry_desc));
        const char* date = gtk_entry_get_text(GTK_ENTRY(entry_date));
        int centre = atoi(gtk_entry_get_text(GTK_ENTRY(entry_centre)));
        int capa = atoi(gtk_entry_get_text(GTK_ENTRY(entry_capa)));
        
        if (strlen(nom) > 0) {
            Evenement* arr = NULL;
            int count = charger_evenements(&arr);
            int new_id = 1;
            for (int i = 0; i < count; i++) {
                if (arr[i].id >= new_id) new_id = arr[i].id + 1;
            }
            
            FILE* f = fopen("data/evenements.txt", "a");
            if (f) {
                fprintf(f, "%d,%s,%s,%s,%d,%d,0\n", new_id, nom, desc, date, centre, capa);
                fclose(f);
                print_info("Événement ajouté");
                on_events_refresh(NULL, NULL);
            }
            if (arr) free(arr);
        }
    }
    gtk_widget_destroy(dialog);
}

void on_events_edit(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    if (current_role != ROLE_ADMIN) {
        show_permission_error("la modification d'événements");
        return;
    }
    print_info("Modifier événement - Formulaire Edit à implémenter");
}

// Fonction Member/Trainer: S'inscrire à un événement
void on_events_enroll(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (!global_evenements_builder) {
        print_error("Builder evenements non disponible");
        return;
    }
    
    GtkTreeView* tree = GTK_TREE_VIEW(gtk_builder_get_object(global_evenements_builder, "evenements_treeview"));
    if (!tree) return;
    
    GtkTreeSelection* selection = gtk_tree_view_get_selection(tree);
    GtkTreeModel* model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gint id, capacite_max, inscrits;
        gchar* nom;
        gtk_tree_model_get(model, &iter, 0, &id, 1, &nom, 5, &capacite_max, 6, &inscrits, -1);
        
        if (inscrits >= capacite_max) {
            GtkWidget* error = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                "Cet événement est complet (%d/%d inscrits)", inscrits, capacite_max);
            gtk_dialog_run(GTK_DIALOG(error));
            gtk_widget_destroy(error);
            g_free(nom);
            return;
        }
        
        gchar* msg = g_strdup_printf("S'inscrire à l'événement %s ?\n\nPlaces: %d/%d", nom, inscrits + 1, capacite_max);
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "%s", msg);
        g_free(msg);
        
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        if (response == GTK_RESPONSE_YES) {
            Evenement* arr = NULL;
            int count = charger_evenements(&arr);
            
            for (int i = 0; i < count; i++) {
                if (arr[i].id == id) {
                    arr[i].inscrits++;
                    break;
                }
            }
            
            FILE* f = fopen("data/evenements.txt", "w");
            if (f) {
                for (int i = 0; i < count; i++) {
                    fprintf(f, "%d,%s,%s,%s,%d,%d,%d\n",
                            arr[i].id, arr[i].nom, arr[i].description,
                            arr[i].date, arr[i].centreId,
                            arr[i].capaciteMax, arr[i].inscrits);
                }
                fclose(f);
                print_info("Inscription réussie!");
                on_events_refresh(NULL, NULL);
            }
            
            if (arr) free(arr);
        }
        
        g_free(nom);
    } else {
        GtkWidget* error = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez sélectionner un événement");
        gtk_dialog_run(GTK_DIALOG(error));
        gtk_widget_destroy(error);
    }
}

void on_events_delete(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    if (current_role != ROLE_ADMIN) {
        show_permission_error("la suppression d'événements");
        return;
    }
    
    if (!global_evenements_builder) {
        print_error("Builder evenements non disponible");
        return;
    }
    
    GtkTreeView* tree = GTK_TREE_VIEW(gtk_builder_get_object(global_evenements_builder, "evenements_treeview"));
    if (!tree) return;
    
    GtkTreeSelection* selection = gtk_tree_view_get_selection(tree);
    GtkTreeModel* model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gint id;
        gchar* nom;
        gtk_tree_model_get(model, &iter, 0, &id, 1, &nom, -1);
        
        gchar* msg = g_strdup_printf("Supprimer l'événement %s (ID: %d) ?", nom, id);
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "%s", msg);
        g_free(msg);
        
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        if (response == GTK_RESPONSE_YES) {
            supprimer_evenement(id);
            on_events_refresh(NULL, NULL);
        }
        
        g_free(nom);
    } else {
        GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez sélectionner un événement");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_events_refresh(GtkWidget* widget, gpointer data) {
    (void)widget; (void)data;
    
    GtkBuilder* active_builder = global_evenements_builder ? global_evenements_builder : builder;
    if (!active_builder) {
        print_error("Builder evenements non disponible");
        return;
    }
    
    GtkTreeView* tree = GTK_TREE_VIEW(gtk_builder_get_object(active_builder, "evenements_treeview"));
    if (!tree) {
        print_error("TreeView evenements_treeview introuvable");
        return;
    }
    
    GtkListStore* store = GTK_LIST_STORE(gtk_tree_view_get_model(tree));
    if (!store) {
        print_error("ListStore introuvable");
        return;
    }
    
    gtk_list_store_clear(store);
    
    Evenement* evenements = NULL;
    int count = charger_evenements(&evenements);
    
    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, evenements[i].id,
            1, evenements[i].nom,
            2, evenements[i].description,
            3, evenements[i].date,
            4, evenements[i].centreId,
            5, evenements[i].capaciteMax,
            6, evenements[i].inscrits,
            -1);
    }
    
    free(evenements);
    print_info("Événements rechargés");
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
