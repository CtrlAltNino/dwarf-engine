#!/bin/bash
set -e

# 1. Configuration
CONTAINER_NAME="dwarf-dev"
IMAGE="registry.fedoraproject.org/fedora-toolbox:43"

# 2. Create the Distrobox if it doesn't exist
if ! distrobox list | grep -q "$CONTAINER_NAME"; then
    echo "Creating Distrobox: $CONTAINER_NAME..."
    distrobox create --image "$IMAGE" --name "$CONTAINER_NAME" --yes
fi

# 3. Define the setup and build commands
# We use 'distrobox enter' with '--' to execute these inside the container
echo "Installing dependencies and building engine inside $CONTAINER_NAME..."

distrobox enter "$CONTAINER_NAME" --bash -c "
    # Install the 'Missing Link' headers and tools
sudo dnf install -y \
        cmake ninja-build gcc-c++ \
        llvm clang clang-tools-extra lldb \
        libstdc++-devel libstdc++-static \
        mesa-libGL-devel libX11-devel libXext-devel \
        libXcursor-devel libXi-devel libXinerama-devel \
        libXrandr-devel libXScrnSaver-devel libXxf86vm-devel \
        wayland-devel libxkbcommon-devel dbus-devel \
        pipewire-devel pulseaudio-libs-devel \
        git curl zip unzip tar pkgconfig
"