#!/bin/bash

# Launch script for Gestion des Centres Sportifs

cd "$(dirname "$0")"

# Check if app exists
if [ ! -f ./app ] && [ ! -f ./app-test ]; then
    echo "Application non compilée. Compilation en cours..."
    make clean
    make
    echo ""
fi

# Launch the app
if [ -f ./app ]; then
    echo "Lancement de l'application (GTK3)..."
    ./app
elif [ -f ./app-test ]; then
    echo "Lancement en mode test (sans GTK3)..."
    ./app-test
else
    echo "Erreur: Application non trouvée"
    exit 1
fi
