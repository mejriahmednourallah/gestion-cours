#!/bin/bash

echo "======================================"
echo "  Test de l'Application GTK"
echo "======================================"
echo ""

# Vérifier si l'app existe
if [ ! -f "app" ]; then
    echo "⚠️  L'application n'est pas compilée"
    echo "📝 Exécutez d'abord: chmod +x compile.sh && ./compile.sh"
    exit 1
fi

# Vérifier les fichiers requis
if [ ! -f "ui/main.glade" ]; then
    echo "❌ Fichier ui/main.glade introuvable"
    exit 1
fi

echo "📋 Vérifications:"
echo "  ✓ Application compilée: app"
echo "  ✓ Interface Glade: ui/main.glade"
echo "  ✓ Répertoire data: $([ -d "data" ] && echo "OK" || echo "Créé automatiquement")"
echo ""

echo "🔐 Identifiants de connexion:"
echo "  ┌─────────────────┬──────────┬──────────┐"
echo "  │ Rôle            │ Username │ Password │"
echo "  ├─────────────────┼──────────┼──────────┤"
echo "  │ Membre ⭐       │ member   │ member   │"
echo "  │ Administrateur  │ user     │ pass     │"
echo "  │ Entraîneur      │ trainer  │ trainer  │"
echo "  └─────────────────┴──────────┴──────────┘"
echo ""
echo "⭐ Le rôle 'Membre' est présélectionné par défaut"
echo ""

echo "📝 Le bouton Login devrait:"
echo "  1. Vérifier les identifiants"
echo "  2. Ouvrir la fenêtre principale"
echo "  3. Afficher le tableau de bord"
echo ""

echo "⚠️  Note: L'interface graphique ne peut pas s'afficher dans ce conteneur"
echo "   Vous verrez: 'cannot open display' - c'est normal"
echo "   Pour tester réellement, utilisez un environnement avec X11"
echo ""

read -p "Appuyez sur Entrée pour lancer ./app (Ctrl+C pour annuler)..."

./app
