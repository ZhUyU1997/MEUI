# SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
#
# SPDX-License-Identifier: MIT

sinclude scripts/env.mk

# i386 platform
# CC			:= gcc -m32
# LD			:= ld -m elf_i386
# export CC LD

W_FLAGS		= -Wall -Werror=implicit-function-declaration -Wno-unused-function \
				-Werror=return-type -Wno-unused-but-set-variable -Wno-unused-variable \
				-Werror=incompatible-pointer-types
X_CFLAGS	+= -std=gnu11 -O3 -g -ggdb $(W_FLAGS)

X_INCDIRS	+= include
SRC			+= src/*.c src/element/*.c src/bind/*.c src/bind/element/*.c src/cache/*.c

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

X_DEFINES	+= _GNU_SOURCE CONFIG_VERSION=\"$(shell cat $(obj)/lib/QuickJS/VERSION)\" alloca=__builtin_alloca
SRC			+= lib/QuickJS/quickjs.c lib/QuickJS/libregexp.c lib/QuickJS/libunicode.c lib/QuickJS/cutils.c

# hashmap
X_INCDIRS	+= lib/hashmap/include
SRC			+= lib/hashmap/src/*.c

# pqueue
X_INCDIRS	+= lib/pqueue
SRC			+= lib/pqueue/*.c

X_CFLAGS	+=
X_LDFLAGS	+= -lm -lpthread
# X_LDFLAGS	+= -ltcmalloc
# X_CFLAGS	+= -march=native
# X_DEFINES	+= PROFILE
# X_LDFLAGS	+= -lprofiler

SRC			+= src/platform/x11/*.c
SRC			+= lib/QuickJS/quickjs-libc.c
X_LDFLAGS	+= -ldl -lX11 -lXext

# SRC			+= src/platform/sdl2/*.c
# SRC			+= lib/QuickJS/quickjs-libc.c
# X_LDFLAGS	+= -ldl -lSDL2

# SRC			+= src/platform/sdl2-core/*.c
# X_LDFLAGS	+= -lSDL2

NAME		:= meui