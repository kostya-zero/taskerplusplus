set shell := ["bash", "-c"]
set windows-shell := ["pwsh.exe", "-NoLogo", "-Command"]

binaryPath := if os() == "windows" { '../build/kvdb.exe' } else { '../build/kvdb' }

# Runs build recipe
default: build-debug

# Build the project to an executable
build-debug:
    cmake  --build ./build --target taskerplusplus

run *ARGS:
    ./build/debug/taskerplusplus $ARGS
