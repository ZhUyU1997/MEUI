sinclude scripts/env.mk

X_CFLAGS	+= -std=gnu11 -O3 -g -ggdb


X_INCDIRS	+= include
SRC			+= src/*.c

# pulogovg
X_INCDIRS	+= lib/plutovg/include lib/plutovg/3rdparty/software lib/plutovg/3rdparty/stb
SRC			+= lib/plutovg/source/*.c lib/plutovg/3rdparty/software/*.c

# pulogosvg
X_INCDIRS	+= lib/plutosvg/include
SRC			+= lib/plutosvg/source/*.c

# flex
X_INCDIRS	+= lib/flex/include
SRC			+= lib/flex/*.c

X_LIBS		+= m

NAME		:= meui