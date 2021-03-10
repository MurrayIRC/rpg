#!/bin/bash

cd ./build/debug
rm -rf osx
mkdir osx
cd osx

proj_name=rpg
proj_root_dir=$(pwd)/../../..

flags=(
	-std=c17 -x objective-c -O0 -w 
)

# Include directories
inc=(
	-I ${proj_root_dir}/external
)

# Source files
src=(
	${proj_root_dir}/src/main.c
	${proj_root_dir}/src/app.c
	${proj_root_dir}/src/window.c
	${proj_root_dir}/src/shader.c
	${proj_root_dir}/src/k_mem_debug.c
	${proj_root_dir}/src/file.c
	${proj_root_dir}/src/log.c
)

fworks=(
	-lglfw
	-lglew
	-framework OpenGL
	-framework CoreFoundation 
	-framework CoreVideo 
	-framework IOKit 
	-framework Cocoa 
	-framework Carbon
)

# Build
clang ${flags[*]} ${fworks[*]} ${inc[*]} ${src[*]} -o ${proj_name}

cd ..