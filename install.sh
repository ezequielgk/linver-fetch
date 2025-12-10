#!/bin/sh
# install.sh — Instala linver desde GitHub
set -e
REPO_URL="https://raw.githubusercontent.com/ezequielgk/linver-fetch/refs/heads/main/linver.cpp"
BIN_NAME="linver"
INSTALL_PATH="/usr/local/bin/$BIN_NAME"

echo "📥 Descargando $BIN_NAME..."
if ! wget -q -O "$BIN_NAME.cpp" "$REPO_URL"; then
    echo "❌ Error: No se pudo descargar el código fuente."
    exit 1
fi

echo "🛠️  Compilando $BIN_NAME..."
if ! g++ -std=c++17 -O2 -s -o "$BIN_NAME" "$BIN_NAME.cpp"; then
    echo "❌ Error: Falló la compilación. ¿Tienes g++ instalado?"
    echo "💡 Instala g++ con:"
    echo "   Debian/Ubuntu: sudo apt install build-essential"
    echo "   Arch: sudo pacman -S gcc"
    echo "   Fedora: sudo dnf install gcc-c++"
    exit 1
fi

echo "🚀 Instalando en $INSTALL_PATH..."
if ! sudo install -m755 "$BIN_NAME" "$INSTALL_PATH"; then
    echo "❌ Error: No se pudo instalar. ¿Tienes permisos de sudo?"
    exit 1
fi

rm -f "$BIN_NAME" "$BIN_NAME.cpp"
echo "✅ ¡Instalación completada!"
echo "Ejecuta 'linver' para ver tu sistema con estilo."
echo ""
echo "Opciones útiles:"
echo "  linver --help        → Mostrar ayuda"
echo "  linver --list-logos  → Ver todos los logos disponibles"
