set shell := ["bash", "-c"]
set windows-shell := ["pwsh.exe", "-NoLogo", "-Command"]

# Runs build recipe
default: build

# Build the project to an executable
build:
    cmake --build ./build --target taskerplusplus

