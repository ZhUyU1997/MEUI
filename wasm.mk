X_CFLAGS	+= -std=gnu11 -O3 -g -ggdb $(W_FLAGS)

X_INCDIRS	+= include
SRC			+= src/*.c src/element/*.c src/bind/*.c src/bind/element/*.c src/cache/*.c src/utils/*.c

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


SRC			+= src/platform/sdl2-core-wasm/*.c
X_LDFLAGS	+= -lSDL2

X_OUTPUT	= meui.wasm

# OPTIMIZE	= -DCONFIG_VERSION=\"1.0.0\" -DEMSCRIPTEN -lm -Oz -Wall --llvm-lto 1 -fno-exceptions

X_SOURCE	:=	$(foreach f,$(filter-out %/, $(SRC)),$(wildcard $(f)))
X_CPPFLAGS	:= $(patsubst %, -I %, $(foreach d,$(X_INCDIRS),$(wildcard $(d))))

# CFLAGS	= --target=wasm32-unknown-wasi -O0 \
# 		 $(X_CPPFLAGS) \
# 		 -Wl,--import-memory \
# 		-Wl,--export-all -Wl,--no-entry \
# 		-DCONFIG_VERSION=\"1.0.0\" -nostartfiles -DEMSCRIPTEN -D_WASI_EMULATED_SIGNAL
# all:
# 	clang $(CFLAGS) -o $(X_OUTPUT) $(X_SOURCE)

CFLAGS	= $(X_CPPFLAGS) -DCONFIG_VERSION=\"1.0.0\" \
	-s LLD_REPORT_UNDEFINED \
	-s STRICT_JS=1 \
	-s SINGLE_FILE=0 \
	-s EXPORT_ES6=1 \
	-s ASSERTIONS \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s MODULARIZE=1 \
	-s EXIT_RUNTIME=0 \
	-s EXPORTED_FUNCTIONS="['_js_cancel_main_loop', '_main']" \
	-s EXPORTED_RUNTIME_METHODS="['FS', 'callMain', 'abort', 'ccall', 'cwrap']" \
	-DEMSCRIPTEN -lm -Oz -Wall --llvm-lto 1 -fno-exceptions -s USE_SDL=2
all:
	mkdir -p browser/gen
	mkdir -p browser/public
	emcc $(CFLAGS) -o browser/gen/meui.js $(X_SOURCE)
	rm -rf browser/public/*
	cp res browser/public -r
	cp framework/polyfill.js browser/gen -r