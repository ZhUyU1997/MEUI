option("backend")
    set_default("x11")
    if is_plat("windows", "macosx") then
        set_default("sdl2-core")
    end 

target("meui")
    set_kind("binary")
    set_rundir("$(projectdir)")

    if is_mode("release") then
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end

    set_languages("gnu11")

    add_cflags("-Wall -Werror=implicit-function-declaration -Wno-unused-function \
				-Werror=return-type  -Wno-unused-variable \
				-Werror=incompatible-pointer-types")
    add_includedirs("include")
    add_files("src/*.c")
    add_files("src/element/*.c")
    add_files("src/bind/*.c")
    add_files("src/bind/element/*.c")
    add_files("src/cache/*.c")
    add_files("src/utils/*.c")
    -- stb
    add_includedirs("lib/stb/include")
    add_files("lib/stb/*.c")
    -- pulogovg
    add_includedirs("lib/plutovg/include")
    add_includedirs("lib/plutovg/3rdparty/software")
    add_files("lib/plutovg/source/*.c")
    add_files("lib/plutovg/3rdparty/software/*.c")
    -- pulogosvg
    add_includedirs("lib/plutosvg/include")
    add_files("lib/plutosvg/source/*.c")
    -- flex
    add_includedirs("lib/flex/include")
    add_files("lib/flex/*.c")
    -- QuickJS
    add_includedirs("lib/QuickJS")
    add_files("lib/QuickJS/quickjs.c")
    add_files("lib/QuickJS/libregexp.c")
    add_files("lib/QuickJS/libunicode.c")
    add_files("lib/QuickJS/cutils.c")
    add_defines("_GNU_SOURCE", "CONFIG_VERSION=\"2021-03-27\"", "alloca=__builtin_alloca")
    -- hashmap
    add_includedirs("lib/hashmap/include")
    add_files("lib/hashmap/src/*.c")
    -- pqueue
    add_includedirs("lib/pqueue")
    add_files("lib/pqueue/*.c")

    add_links("pthread", "m")

    if get_config("backend") == "x11" then
        add_files("lib/QuickJS/quickjs-libc.c")
        add_files("src/platform/x11/*.c")
        add_links("dl", "X11", "Xext")
    end
    if get_config("backend") == "sdl2" then
        add_files("lib/QuickJS/quickjs-libc.c")
        add_files("src/platform/sdl2/*.c")
        add_links("dl", "lSDL2")
    end
    if get_config("backend") == "test" then
        add_files("src/platform/test/*.c")
        add_links("dl")
    end
    if get_config("backend") == "sdl2-core" then
        add_files("src/platform/sdl2-core/*.c")
        add_links("lSDL2")
    end

    add_options("backend")