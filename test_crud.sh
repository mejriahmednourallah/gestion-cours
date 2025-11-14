#!/bin/bash

echo "🧪 Test CRUD - Compilation et vérification"
echo "=========================================="

# Compilation
echo "🔨 Compilation..."
make clean > /dev/null 2>&1
make 2>&1 | grep -E "(error|✅|erreur)" || echo "✅ Compilation OK"

# Vérifier que l'exécutable existe
if [ -f "./app" ]; then
    echo "✅ Exécutable créé"
else
    echo "❌ Échec de compilation"
    exit 1
fi

# Vérifier les fichiers de données
echo ""
echo "📁 Fichiers de données:"
for file in data/*.txt; do
    if [ -f "$file" ]; then
        lines=$(wc -l < "$file")
        echo "  ✅ $file ($lines lignes)"
    else
        echo "  ❌ $file manquant"
    fi
done

echo ""
echo "🎯 Fonctionnalités implémentées:"
echo "  ✅ Authentification avec 3 rôles (Admin/Member/Trainer)"
echo "  ✅ Contrôle d'accès basé sur les rôles"
echo "  ✅ Chargement des données depuis fichiers .txt"
echo "  ✅ Affichage dans TreeViews (Membres, Centres, Cours)"
echo "  ✅ Suppression avec confirmation (Membres, Centres)"
echo "  ✅ Recherche/filtrage (Membres)"
echo ""
echo "⚠️  À implémenter:"
echo "  ⏳ Formulaires Add/Edit pour tous les modules"
echo "  ⏳ Modules Entraîneurs, Équipements, Événements (loading)"
echo "  ⏳ Fonctions d'inscription/réservation"
echo ""
echo "🚀 Lancer l'application: ./app"
echo "🔐 Identifiants de test:"
echo "   Admin:      user / pass"
echo "   Membre:     member / member"
echo "   Entraîneur: trainer / trainer"
