#!/bin/bash

echo "========================================="
echo "  🏋️ GESTION DES SALLES DE SPORT"
echo "  Compilation et Vérification Finale"
echo "========================================="
echo ""

# Vérifier la structure
echo "📁 Vérification de la structure du projet..."
missing=0

if [ ! -d "src" ]; then echo "❌ Répertoire src/ manquant"; missing=1; else echo "✓ src/"; fi
if [ ! -d "ui" ]; then echo "❌ Répertoire ui/ manquant"; missing=1; else echo "✓ ui/"; fi
if [ ! -d "data" ]; then echo "❌ Répertoire data/ manquant"; missing=1; else echo "✓ data/"; fi

# Vérifier les fichiers Glade
glades=("main" "membres" "entraineurs" "cours" "equipements" "centres" "evenements")
echo ""
echo "🎨 Vérification des interfaces Glade..."
for glade in "${glades[@]}"; do
    if [ -f "ui/${glade}.glade" ]; then
        echo "✓ ui/${glade}.glade"
    else
        echo "❌ ui/${glade}.glade manquant"
        missing=1
    fi
done

if [ $missing -eq 1 ]; then
    echo ""
    echo "❌ Structure incomplète. Vérifiez les fichiers manquants."
    exit 1
fi

echo ""
echo "🧹 Nettoyage..."
make clean 2>&1 | grep -v "No rule" || true

echo ""
echo "🔨 Compilation..."
if make; then
    echo ""
    echo "✅ COMPILATION RÉUSSIE!"
    echo ""
    
    # Statistiques
    echo "📊 Statistiques du projet:"
    echo "  Lignes de code C:"
    find src -name "*.c" -exec wc -l {} + | tail -n 1 | awk '{print "    "$1" lignes"}'
    echo "  Fichiers source: $(ls src/*.c | wc -l)"
    echo "  Fichiers header: $(ls src/*.h | wc -l)"
    echo "  Interfaces Glade: $(ls ui/*.glade | wc -l)"
    echo ""
    
    # Taille de l'exécutable
    echo "  Taille de l'application:"
    ls -lh app | awk '{print "    "$5" ("$9")"}'
    echo ""
    
    # Modules
    echo "✨ Modules disponibles:"
    echo "  1. 👥 Gestion des Membres"
    echo "  2. 🏋️  Gestion des Entraîneurs"
    echo "  3. 📚 Gestion des Cours Sportifs"
    echo "  4. 🎽 Gestion des Équipements"
    echo "  5. 🏢 Gestion des Centres"
    echo "  6. 📅 Gestion des Événements"
    echo ""
    
    # Identifiants
    echo "🔐 Identifiants de test:"
    echo "  ┌─────────────────┬──────────┬──────────┬─────────────────┐"
    echo "  │ Rôle            │ Username │ Password │ Accès           │"
    echo "  ├─────────────────┼──────────┼──────────┼─────────────────┤"
    echo "  │ Administrateur  │ user     │ pass     │ Complet         │"
    echo "  │ Membre ⭐       │ member   │ member   │ Cours/Events    │"
    echo "  │ Entraîneur      │ trainer  │ trainer  │ Cours/Équip.    │"
    echo "  └─────────────────┴──────────┴──────────┴─────────────────┘"
    echo ""
    echo "  ⭐ = Présélectionné par défaut"
    echo ""
    
    # Données
    echo "💾 Données de test disponibles:"
    echo "  • 15 Membres      • 8 Entraîneurs"
    echo "  • 10 Cours        • 10 Équipements"
    echo "  • 5 Centres       • 6 Événements"
    echo ""
    
    # Instructions
    echo "🚀 Pour lancer l'application:"
    echo "  ./app"
    echo ""
    
    echo "📝 Fonctionnement:"
    echo "  1. Fenêtre de login s'affiche"
    echo "  2. Sélectionnez un rôle (Membre par défaut)"
    echo "  3. Entrez identifiants et cliquez Connexion"
    echo "  4. Tableau de bord s'affiche avec statistiques"
    echo "  5. Menu 'Modules' → Chaque module ouvre sa fenêtre"
    echo "  6. Menu 'Fichier' → Déconnexion ou Quitter"
    echo ""
    
    echo "⚠️  Note pour environnements sans GUI:"
    echo "  Dans un conteneur ou SSH sans X11, vous verrez:"
    echo "  'cannot open display' - c'est normal"
    echo "  L'application nécessite un environnement graphique."
    echo ""
    
    echo "✅ Projet prêt à l'emploi!"
    
else
    echo ""
    echo "❌ ERREUR DE COMPILATION"
    echo ""
    echo "Vérifiez:"
    echo "  1. Installation de gcc: gcc --version"
    echo "  2. Installation de GTK: pkg-config --modversion gtk+-3.0"
    echo "  3. make et pkg-config installés"
    echo ""
    echo "Installation des dépendances:"
    echo "  Ubuntu/Debian:"
    echo "    sudo apt-get install build-essential libgtk-3-dev pkg-config"
    echo "  Fedora/RHEL:"
    echo "    sudo dnf install gcc make gtk3-devel pkg-config"
    echo ""
    exit 1
fi
