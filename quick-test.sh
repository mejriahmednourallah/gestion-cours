#!/bin/bash

echo "======================================="
echo "  COMPILATION ET TEST RAPIDE"
echo "======================================="
echo ""

# Nettoyer
echo "🧹 Nettoyage..."
rm -f src/*.o app 2>/dev/null

# Compiler
echo "🔨 Compilation..."
make 2>&1 | grep -E "(error|warning|✓)" || echo "⚠️  Vérifiez les erreurs de compilation"

# Vérifier si compilé
if [ -f "app" ]; then
    echo ""
    echo "✅ Compilation réussie!"
    echo ""
    echo "📋 Changements appliqués:"
    echo "  ✓ Page d'accueil avec dashboard statistiques"
    echo "  ✓ Affichage de 6 cartes de statistiques"
    echo "  ✓ gtk_widget_show_all() pour afficher tous les widgets"
    echo "  ✓ Logs de débogage améliorés"
    echo ""
    echo "🔐 TEST DE CONNEXION:"
    echo "  Username: user"
    echo "  Password: pass"
    echo "  Rôle: Administrateur (ou laissez Membre par défaut)"
    echo ""
    echo "📊 APRÈS CONNEXION, VOUS DEVRIEZ VOIR:"
    echo "  • Titre 'Bienvenue!'"
    echo "  • 6 cartes statistiques colorées"
    echo "  • Membres: 15 | Entraîneurs: 8 | Cours: 10"
    echo "  • Centres: 5 | Équipements: 10 | Événements: 6"
    echo "  • Menu 'Modules' pour naviguer"
    echo "  • Menu 'Fichier' avec Déconnexion/Quitter"
    echo ""
    echo "⚠️  Note: Dans ce conteneur, vous verrez 'cannot open display'"
    echo "   C'est normal - testez sur une machine avec GUI"
    echo ""
    ls -lh app
    echo ""
else
    echo ""
    echo "❌ Échec de la compilation"
    echo "Essayez manuellement: make clean && make"
    exit 1
fi
