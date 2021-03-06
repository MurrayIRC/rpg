.PHONY: all clean
include .env
export

PROJECT_NAME       ?= rpg

# Build mode for project: debug or release
BUILD_MODE            ?= debug
BUILD_DIR = build/$(BUILD_MODE)

# No uname.exe on MinGW!, but OS=Windows_NT on Windows!
# ifeq ($(UNAME),Msys) -> Windows
ifeq ($(OS),Windows_NT)
	PLATFORM_OS=WINDOWS
else
	UNAMEOS=$(shell uname)
	ifeq ($(UNAMEOS),Linux)
		PLATFORM_OS=LINUX
	endif
	ifeq ($(UNAMEOS),Darwin)
		PLATFORM_OS=OSX
	endif
endif

ifeq ($(PLATFORM_OS),WINDOWS)
	EXT = .exe
endif

# Define default C compiler: gcc
# NOTE: define g++ compiler if using C++
CC = clang

ifeq ($(PLATFORM_OS),OSX)
	# OSX default compiler
	CC = clang
endif

# Define default make program: Mingw32-make
MAKE = mingw32-make

ifeq ($(PLATFORM_OS),LINUX)
	MAKE = make
endif
ifeq ($(PLATFORM_OS),OSX)
	MAKE = make
endif

# Define compiler flags:
#  -O1                  defines optimization level
#  -g                   include debug information on compilation
#  -s                   strip unnecessary data from build
#  -Wall                turns on most, but not all, compiler warnings
#  -std=c99             defines C language mode (standard C from 1999 revision)
#  -std=gnu99           defines C language mode (GNU C from 1999 revision)
#  -Wno-missing-braces  ignore invalid warning (GCC bug 53119)
#  -D_DEFAULT_SOURCE    use with -std=c99 on Linux and PLATFORM_WEB, required for timespec
CFLAGS += -O1 -Wall -Wextra -std=gnu89 -Wno-missing-braces

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g -D_DEBUG -D_CRT_SECURE_NO_WARNINGS
endif
ifeq ($(BUILD_MODE),release)
	CFLAGS += -s
endif

# Additional flags for compiler (if desired)
#CFLAGS += -Wextra -Wmissing-prototypes -Wstrict-prototypes
ifeq ($(PLATFORM_OS),OSX)
	CFLAGS += -objective-c
endif
ifeq ($(PLATFORM_OS),LINUX)
	CFLAGS += -D_DEFAULT_SOURCE
endif

# Define include paths for required headers
# NOTE: Several external required libraries (stb and others)
INCLUDE_PATHS = -I.

ifeq ($(PLATFORM_OS),WINDOWS)
	INCLUDE_PATHS += -I$(WIN_GLFW_INC) -I$(WIN_CGLM_INC)
endif
ifeq ($(PLATFORM_OS),OSX)
	INCLUDE_PATHS += -I$(PATH)
endif
ifeq ($(PLATFORM_OS),LINUX)
	# Reset everything.
	INCLUDE_PATHS = -isystem.
endif

# Define library paths containing required libs.
LDFLAGS = -L.

ifeq ($(PLATFORM_OS),LINUX)
	# Reset everything.
	# Precedence: immediately local, installed version, raysan5 provided libs
	LDFLAGS = -L.
endif

ifeq ($(PLATFORM_OS),WINDOWS)
	LDLIBS += -L$(WIN_CGLM_LIB) -lkernel32 -luser32 -lshell32 -lgdi32 -lAdvapi32 -lopengl32
endif
ifeq ($(PLATFORM_OS),LINUX)
	# Libraries for Debian GNU/Linux desktop compiling
	# NOTE: Required packages: libegl1-mesa-dev
	LDLIBS = -lm -lpthread -ldl -lrt

	# On X11 requires also below libraries
	LDLIBS += -lX11 -lXrandr -lXi -lGl
	# NOTE: It seems additional libraries are not required any more, latest GLFW just dlopen them
	#LDLIBS += -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor
endif
ifeq ($(PLATFORM_OS),OSX)
	# Libraries for OSX 10.9 desktop compiling
	# -lvulkan -lMoltenVK
	LDLIBS = -framework Foundation -framework CoreFoundation -framework CoreVideo -framework IOKit -framework Cocoa -framework Carbon -framework OpenGL
endif

# Define a recursive wildcard function
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Define all source files required
SRC_DIR = ./src
OBJ_DIR = ./obj

# Define all object files from source files
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
DEPENDS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(SOURCES))

MAKEFILE_PARAMS = $(PROJECT_NAME)

%.spv: %
	${WIN_GLSLC} $< -o $@

# Default target entry
all: $(MAKEFILE_PARAMS)
	$(MAKE) clean
	$(MAKE) $(MAKEFILE_PARAMS)

# Clean everything
clean:
ifeq ($(PLATFORM_OS),WINDOWS)
	del *.o *.exe /s
endif
ifeq ($(PLATFORM_OS),LINUX)
	find -type f -executable | xargs file -i | grep -E 'x-object|x-archive|x-sharedlib|x-executable' | rev | cut -d ':' -f 2- | rev | xargs rm -fv
endif
ifeq ($(PLATFORM_OS),OSX)
	find . -type f -perm +ugo+x -delete
	rm -f *.o
endif

# Project target defined by PROJECT_NAME
$(MAKEFILE_PARAMS): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@$(EXT) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS)

-include ($(DEPENDS))

# Compile source files
# NOTE: This pattern will compile every module defined on $(OBJS)
#%.o: %.c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	$(CC) $(CFLAGS) $(INCLUDE_PATHS) -MMD -MP -c $< -o $@