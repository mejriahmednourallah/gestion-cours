#!/bin/bash

# Installation script for Gestion des Centres Sportifs

set -e

echo "=================================="
echo "Installation - Gestion des Centres Sportifs"
echo "=================================="
echo ""

# Detect Linux distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo $ID
    elif [ -f /etc/redhat-release ]; then
        echo "rhel"
    elif [ -f /etc/debian_version ]; then
        echo "debian"
    else
        echo "unknown"
    fi
}

DISTRO=$(detect_distro)

echo "[1/4] Détection de la distribution: $DISTRO"
echo ""

case $DISTRO in
    ubuntu|debian)
        echo "[2/4] Installation des dépendances (Debian/Ubuntu)..."
        sudo apt-get update -qq
        sudo apt-get install -y build-essential libgtk-3-dev pkg-config
        ;;
    fedora)
        echo "[2/4] Installation des dépendances (Fedora)..."
        sudo dnf install -y gcc gtk3-devel pkg-config
        ;;
    rhel|centos)
        echo "[2/4] Installation des dépendances (RHEL/CentOS)..."
        sudo yum groupinstall -y "Development Tools"
        sudo yum install -y gtk3-devel pkg-config
        ;;
    *)
        echo "[2/4] Distribution inconnue. Installez manuellement:"
        echo "  - gcc"
        echo "  - libgtk-3-dev (ou gtk3-devel)"
        echo "  - pkg-config"
        exit 1
        ;;
esac

echo ""
echo "[3/4] Compilation du projet..."
cd "$(dirname "$0")"
make clean
make

echo ""
echo "[4/4] Vérification de l'installation..."
if [ -f ./app ] || [ -f ./app-test ]; then
    echo "✓ Compilation réussie!"
else
    echo "✗ Erreur lors de la compilation"
    exit 1
fi

echo ""
echo "=================================="
echo "Installation terminée!"
echo "=================================="
echo ""
echo "Pour lancer l'application:"
echo "  ./app"
echo ""
echo "Identifiants de test:"
echo "  Admin:     user / pass"
echo "  Membre:    member / member"
echo "  Entraîneur: trainer / trainer"
echo ""
