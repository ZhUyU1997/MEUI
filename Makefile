sinclude scripts/env.mk

X_CFLAGS	+= -std=gnu11 -O3 -g -ggdb -Wall


X_INCDIRS	+= include
SRC			+= src/*.c

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

X_LIBS		+= m

NAME		:= meui