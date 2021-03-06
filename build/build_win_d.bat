@echo off

pushd build
pushd debug
pushd win
del *.exe /s

set proj_name=rpg
set proj_root_dir=C:/work/c/rpg

set glfw_lib=C:/work/c/libraries/glfw/lib-static-ucrt

set flags=^
-std=c17 ^
-w

:: Include directories
set inc=^
	-I%proj_root_dir%/external^
	-IC:/work/c/libraries/glfw/include

:: Source files
set src=^
	%proj_root_dir%/src/main.c^
	%proj_root_dir%/src/kaneda/shader.c^
	%proj_root_dir%/src/kaneda/mem_debug.c^
	%proj_root_dir%/src/kaneda/file.c^
	%proj_root_dir%/src/kaneda/log.c

set libs=^
	-lkernel32 ^
	-luser32^
	-lshell32^
	-lgdi32^
	-lAdvapi32^
	-lXinput

:: Build
gcc -O0 %inc% %src% %flags% %libs% -lm -o %proj_name%

popd