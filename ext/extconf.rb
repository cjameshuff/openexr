require 'mkmf'

if enable_config('debug')
    puts '[INFO] debug build configuration enabled.'
    CONFIG['debugflags'] << ' -ggdb3 -O0'
end

# $CFLAGS += " -std=c99"
$INCFLAGS += " -I../../../../ext/clibs/include"
$INCFLAGS += " -I../../../../ext/clibs/include/OpenEXR"

$srcs = ['rb_openexr.cpp', 'rb_vec2.cpp', 'rb_vec3.cpp', 'rb_vec4.cpp', 'rb_color4.cpp']
EXRLIBDIR = "../../../../ext/clibs/lib/"
EXRLIBS = %w[libIlmImf.a libImath.a libIex.a libIlmThread.a libHalf.a].map {|f| EXRLIBDIR + f}

$objs = EXRLIBS + ['rb_openexr.o', 'rb_vec2.o', 'rb_vec3.o', 'rb_vec4.o', 'rb_color4.o']

dir_config('openexr_native')

create_makefile('openexr_native')
