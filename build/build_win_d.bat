@echo off

pushd build
pushd debug
pushd win
del *.exe /s

set proj_name=rpg
set proj_root_dir=C:/work/c/rpg

set glew_lib=C:/work/c/libraries/glew/lib/Release/x64
set glfw_lib=C:/work/c/libraries/glfw/lib-static-ucrt

set flags=^
-std=c17 ^
-w

:: Include directories
set inc=^
	-I${proj_root_dir}/external^
	-IC:/work/c/libraries/glfw/include^
	-IC:/work/c/libraries/glew/include

:: Source files
set src=^
	%proj_root_dir%/src/main.c^
	%proj_root_dir%/src/kaneda/app.c^
	%proj_root_dir%/src/kaneda/window.c^
	%proj_root_dir%/src/kaneda/shader.c^
	%proj_root_dir%/src/kaneda/mem_debug.c^
	%proj_root_dir%/src/kaneda/math.c^
	%proj_root_dir%/src/kaneda/file.c^
	%proj_root_dir%/src/kaneda/log.c

set libs=^
	-lopengl32^
	-lkernel32 ^
	-luser32^
	-lshell32^
	-lgdi32^
	-lAdvapi32^
	-L%glew_lib%^
	-L%glfw_lib%^
	-lglew32^
	-lglfw3

:: Build
gcc -O0 %inc% %src% %flags% %libs% -lm -o %proj_name%

popd