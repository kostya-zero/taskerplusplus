set shell := ["bash", "-c"]
set windows-shell := ["pwsh.exe", "-NoLogo", "-Command"]

# Runs build recipe
default: build-debug

# Build the project to an executable
build-debug:
    cmake  --build ./build --target taskerplusplus

run *ARGS:
    ./build/taskerplusplus $ARGS
