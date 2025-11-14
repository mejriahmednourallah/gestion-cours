#!/bin/bash
# Script de vérification et recompilation

echo "========================================="
echo "  DIAGNOSTIC ET RECOMPILATION"
echo "========================================="
echo ""

# Vérifier la version GLIBC du système
echo "📋 Informations système:"
echo -n "  Système: "
uname -s
echo -n "  Architecture: "
uname -m
echo -n "  Version GLIBC disponible: "
ldd --version | head -n1
echo ""

# Vérifier si l'app existe et sa compatibilité
if [ -f "app" ]; then
    echo "⚠️  Exécutable 'app' existant détecté"
    echo -n "  Compilé avec GLIBC: "
    strings app | grep "GLIBC_" | sort -u | tail -n1
    echo ""
    echo "❌ PROBLÈME: L'exécutable a été compilé dans un autre environnement"
    echo "   Il nécessite GLIBC_2.34 mais votre système a une version plus ancienne"
    echo ""
    echo "🔧 SOLUTION: Recompiler sur CE système"
    echo ""
    
    read -p "Voulez-vous supprimer l'ancien exécutable et recompiler? (o/N) " -n 1 -r
    echo ""
    if [[ ! $REPLY =~ ^[OoYy]$ ]]; then
        echo "Opération annulée."
        exit 1
    fi
    
    echo ""
    echo "🧹 Suppression de l'ancien exécutable..."
    rm -f app src/*.o
fi

echo "🔨 RECOMPILATION pour votre système..."
echo ""

# Nettoyer complètement
make clean 2>/dev/null

# Compiler
if make; then
    echo ""
    echo "✅ COMPILATION RÉUSSIE!"
    echo ""
    echo "📋 Vérification du nouvel exécutable:"
    ls -lh app
    echo ""
    echo -n "  Compilé avec GLIBC: "
    strings app | grep "GLIBC_" | sort -u | tail -n1
    echo ""
    echo "🎯 L'application devrait maintenant fonctionner sur CE système"
    echo ""
    echo "🔐 Identifiants de test:"
    echo "  ┌─────────────────┬──────────┬──────────┐"
    echo "  │ Rôle            │ Username │ Password │"
    echo "  ├─────────────────┼──────────┼──────────┤"
    echo "  │ Administrateur  │ user     │ pass     │"
    echo "  │ Membre          │ member   │ member   │"
    echo "  │ Entraîneur      │ trainer  │ trainer  │"
    echo "  └─────────────────┴──────────┴──────────┘"
    echo ""
    echo "▶️  Lancer avec: ./app"
    echo ""
else
    echo ""
    echo "❌ ERREUR DE COMPILATION"
    echo ""
    echo "Vérifiez que vous avez les dépendances:"
    echo "  • gcc"
    echo "  • make"
    echo "  • gtk+-3.0 (libgtk-3-dev)"
    echo "  • pkg-config"
    echo ""
    echo "Installation des dépendances (selon votre système):"
    echo ""
    echo "Ubuntu/Debian:"
    echo "  sudo apt-get update"
    echo "  sudo apt-get install build-essential libgtk-3-dev pkg-config"
    echo ""
    echo "Fedora/RHEL:"
    echo "  sudo dnf install gcc make gtk3-devel pkg-config"
    echo ""
    echo "Arch Linux:"
    echo "  sudo pacman -S base-devel gtk3 pkg-config"
    echo ""
    exit 1
fi
