sinclude scripts/env.mk

W_FLAGS		= -Wall -Werror=implicit-function-declaration -Wno-unused-function -Werror=return-type -Wno-unused-but-set-variable -Wno-unused-variable
X_CFLAGS	+= -std=gnu11 -O3 -g -ggdb $(W_FLAGS)

X_INCDIRS	+= include
SRC			+= src/*.c src/platform/*.c src/bind/*.c  examples/*.c

# stb
X_INCDIRS	+= lib/stb/include
SRC			+= lib/stb/*.c

# pulogovg
X_INCDIRS	+= lib/plutovg/include lib/plutovg/3rdparty/software
SRC			+= lib/plutovg/source/*.c lib/plutovg/3rdparty/software/*.c

# pulogosvg
X_INCDIRS	+= lib/plutosvg/include
SRC			+= lib/plutosvg/source/*.c

# flex
X_INCDIRS	+= lib/flex/include
SRC			+= lib/flex/*.c

# QuickJS
X_INCDIRS	+= lib/QuickJS
SRC			+= lib/QuickJS/

# hashmap
X_INCDIRS	+= lib/hashmap/include
SRC			+= lib/hashmap/src/*.c

# pqueue
X_INCDIRS	+= lib/pqueue
SRC			+= lib/pqueue/*.c

X_CFLAGS	+=
X_LDFLAGS	+= -lm -lprofiler -ldl -lpthread -lX11

NAME		:= meui

define CUSTOM_TARGET_CMD
echo [OUTPUT] $@
$(LD) -r -T class.lds -o $@.o $(X_OBJS)
$(CC) -o $@ $@.o $(X_LDFLAGS)
endef