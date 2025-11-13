#!/bin/bash
# Script de recompilation rapide

echo "🧹 Nettoyage..."
rm -f src/*.o app

echo "🔨 Compilation avec -rdynamic..."
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0 2>/dev/null || echo ""` -c src/main.c -o src/main.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0 2>/dev/null || echo ""` -c src/membres.c -o src/membres.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0 2>/dev/null || echo ""` -c src/entraineurs.c -o src/entraineurs.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0 2>/dev/null || echo ""` -c src/cours.c -o src/cours.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0 2>/dev/null || echo ""` -c src/equipements.c -o src/equipements.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0 2>/dev/null || echo ""` -c src/centres.c -o src/centres.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0 2>/dev/null || echo ""` -c src/evenements.c -o src/evenements.o
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0 2>/dev/null || echo ""` -c src/utils.c -o src/utils.o

echo "🔗 Linkage..."
gcc -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0 2>/dev/null || echo ""` -o app src/main.o src/membres.o src/entraineurs.o src/cours.o src/equipements.o src/centres.o src/evenements.o src/utils.o `pkg-config --libs gtk+-3.0 2>/dev/null || echo ""`

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Compilation réussie!"
    echo ""
    echo "🔐 Identifiants:"
    echo "  Membre (défaut): member / member"
    echo "  Admin: user / pass"
    echo "  Entraîneur: trainer / trainer"
    echo ""
    echo "▶️  Lancez avec: ./app"
else
    echo "❌ Erreur de compilation"
    exit 1
fi
