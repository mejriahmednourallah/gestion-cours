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
    
    // Update labels
    GtkLabel* user_label = GTK_LABEL(gtk_builder_get_object(builder, "current_user_label"));
    GtkLabel* role_label = GTK_LABEL(gtk_builder_get_object(builder, "current_role_label"));
    
    char user_text[100];
    char role_text[100];
    const char* role_names[] = {"Administrateur", "Membre", "Entraîneur"};
    
    snprintf(user_text, sizeof(user_text), "Utilisateur: %s", current_user);
    snprintf(role_text, sizeof(role_text), "Rôle: %s", role_names[current_role]);
    
    gtk_label_set_text(user_label, user_text);
    gtk_label_set_text(role_label, role_text);
    
    // Hide login window and show main window
    gtk_widget_hide(GTK_WIDGET(login_window));
    gtk_widget_show(GTK_WIDGET(main_window));
    
    print_success("Authentification réussie");
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
    print_info("Accès au module Membres");
    GtkNotebook* notebook = GTK_NOTEBOOK(gtk_builder_get_object(builder, "content_notebook"));
    gtk_notebook_set_current_page(notebook, 1); // Membres tab is at index 1
}

void on_menu_centres(GtkWidget* widget, gpointer data) {
    print_info("Accès au module Centres");
    GtkNotebook* notebook = GTK_NOTEBOOK(gtk_builder_get_object(builder, "content_notebook"));
    gtk_notebook_set_current_page(notebook, 2);
}

void on_menu_courses(GtkWidget* widget, gpointer data) {
    print_info("Accès au module Cours");
    GtkNotebook* notebook = GTK_NOTEBOOK(gtk_builder_get_object(builder, "content_notebook"));
    gtk_notebook_set_current_page(notebook, 3);
}

void on_menu_trainers(GtkWidget* widget, gpointer data) {
    print_info("Accès au module Entraîneurs");
    GtkNotebook* notebook = GTK_NOTEBOOK(gtk_builder_get_object(builder, "content_notebook"));
    gtk_notebook_set_current_page(notebook, 4);
}

void on_menu_equipment(GtkWidget* widget, gpointer data) {
    print_info("Accès au module Équipements");
    GtkNotebook* notebook = GTK_NOTEBOOK(gtk_builder_get_object(builder, "content_notebook"));
    gtk_notebook_set_current_page(notebook, 5);
}

void on_menu_events(GtkWidget* widget, gpointer data) {
    print_info("Accès au module Événements");
    GtkNotebook* notebook = GTK_NOTEBOOK(gtk_builder_get_object(builder, "content_notebook"));
    gtk_notebook_set_current_page(notebook, 6);
}

// ============= Module CRUD Stubs =============

// Membres
void on_membres_add(GtkWidget* widget, gpointer data) {
    print_info("Ajouter membre");
    ajouter_membre();
}

void on_membres_edit(GtkWidget* widget, gpointer data) {
    print_info("Modifier membre");
}

void on_membres_delete(GtkWidget* widget, gpointer data) {
    print_info("Supprimer membre");
}

void on_membres_search(GtkWidget* widget, gpointer data) {
    print_info("Rechercher membre");
}

void on_membres_refresh(GtkWidget* widget, gpointer data) {
    print_info("Rafraîchir membres");
    afficher_tous_membres();
}

// Centres
void on_centres_add(GtkWidget* widget, gpointer data) {
    print_info("Ajouter centre");
    ajouter_centre();
}

void on_centres_edit(GtkWidget* widget, gpointer data) {
    print_info("Modifier centre");
}

void on_centres_delete(GtkWidget* widget, gpointer data) {
    print_info("Supprimer centre");
}

void on_centres_refresh(GtkWidget* widget, gpointer data) {
    print_info("Rafraîchir centres");
    afficher_tous_centres();
}

// Cours
void on_cours_add(GtkWidget* widget, gpointer data) {
    print_info("Ajouter cours");
    ajouter_cours();
}

void on_cours_edit(GtkWidget* widget, gpointer data) {
    print_info("Modifier cours");
}

void on_cours_delete(GtkWidget* widget, gpointer data) {
    print_info("Supprimer cours");
}

void on_cours_refresh(GtkWidget* widget, gpointer data) {
    print_info("Rafraîchir cours");
    afficher_tous_cours();
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
