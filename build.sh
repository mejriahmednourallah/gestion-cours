#!/bin/bash
# Script de compilation et test

echo "🔨 Compilation de l'application..."
make clean
make

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Compilation réussie!"
    echo ""
    echo "📋 Résumé des corrections appliquées:"
    echo "  ✓ Syntaxe GTK3 corrigée dans ui/main.glade"
    echo "  ✓ Flag -rdynamic ajouté au Makefile"
    echo "  ✓ Tous les callbacks exportés (static retiré)"
    echo "  ✓ ComboBox avec items individuels"
    echo "  ✓ Rôle par défaut: Membre (index 1)"
    echo "  ✓ Initialisation programmatique du combo box"
    echo ""
    echo "🔐 Identifiants disponibles:"
    echo "  ┌─────────────────┬──────────┬──────────┐"
    echo "  │ Rôle            │ Username │ Password │"
    echo "  ├─────────────────┼──────────┼──────────┤"
    echo "  │ Administrateur  │ user     │ pass     │"
    echo "  │ Membre          │ member   │ member   │ ← PAR DÉFAUT"
    echo "  │ Entraîneur      │ trainer  │ trainer  │"
    echo "  └─────────────────┴──────────┴──────────┘"
    echo ""
    echo "📝 Note: Le combo box 'Rôle' devrait maintenant:"
    echo "  1. Afficher 'Membre' par défaut"
    echo "  2. Permettre de sélectionner d'autres rôles"
    echo "  3. Être cliquable et fonctionnel"
    echo ""
    echo "🚀 Pour tester l'application:"
    echo "  ./app"
    echo ""
else
    echo ""
    echo "❌ Erreur de compilation"
    exit 1
fi
