#!/bin/bash

cd ./build/debug
rm -rf osx
mkdir osx
cd osx

proj_name=rpg
proj_root_dir=/Users/murray/work/c/rpg
#$(pwd)/../../..

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
	${proj_root_dir}/src/gl.c
	${proj_root_dir}/src/kaneda/app.c
	${proj_root_dir}/src/kaneda/window.c
	${proj_root_dir}/src/kaneda/shader.c
	${proj_root_dir}/src/kaneda/renderer.c
	${proj_root_dir}/src/kaneda/mem_debug.c
	${proj_root_dir}/src/kaneda/file.c
	${proj_root_dir}/src/kaneda/log.c
)

fworks=(
	-lglfw
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