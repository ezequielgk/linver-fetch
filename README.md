# linver

Un fetch ligero, elegante y minimalista escrito en C++ que muestra información del sistema junto con el logo ASCII de tu distribución Linux.

Diseñado con limpieza visual en mente: sin bordes innecesarios, colores ANSI bien equilibrados y soporte para las distribuciones más populares.


> *Nota: Puedes añadir una imagen de ejemplo en tu repositorio si lo deseas.*

---

## ✨ Características

- Logo ASCII específico por distribución (embebido, sin archivos externos).
- Información del sistema: kernel, uptime, arquitectura, usuario, host.
- Soporte para `--help` y `--list-logos`.
- Autocontenido: solo un binario, nada más.

---

## 📦 Instalación

### Requisitos previos

Debes tener instalado en tu sistema:

- `g++` (versión compatible con C++17)
- `wget` o `curl`
- `sudo` (para instalar en `/usr/local/bin`)

#### Instalar dependencias por distribución

| Distribución      | Comando                                             |
|-------------------|-----------------------------------------------------|
| **Debian/Ubuntu** | `sudo apt install build-essential wget`            |
| **Arch Linux**    | `sudo pacman -S gcc wget`                          |
| **Fedora**        | `sudo dnf install gcc-c++ wget`                    |
| **openSUSE**      | `sudo zypper install gcc-c++ wget`                 |

---

### Instalar con un solo comando

Ejecuta en tu terminal:

```bash
wget -qO- https://raw.githubusercontent.com/ezequielgk/linver-fetch/refs/heads/main/install.sh | sh
