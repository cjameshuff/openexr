require 'mkmf'

if enable_config('debug')
    puts '[INFO] debug build configuration enabled.'
    CONFIG['debugflags'] << ' -ggdb3 -O0'
end

# $CFLAGS += " -std=c99"
$warnflags = ''
CONFIG['warnflags'] = ''

# $INCFLAGS += " -I../../../../ext/clibs/include"
# $INCFLAGS += " -I../../../../ext/clibs/include/OpenEXR"
$INCFLAGS += " -Iclibs/include"
$INCFLAGS += " -Iclibs/include/OpenEXR"

$srcs = [
    'rb_openexr.cpp',
    'rb_vec2.cpp',
    'rb_vec3.cpp',
    'rb_vec4.cpp',
    'rb_mat44.cpp',
    'rb_quat.cpp',
    'rb_color4.cpp'
]
EXRLIBDIR = "clibs/lib/"
EXRLIBS = %w[libIlmImf.a libImath.a libIex.a libIlmThread.a libHalf.a].map {|f| EXRLIBDIR + f}

$objs = EXRLIBS + $srcs.map {|f| File.basename(f, ".*") + ".o"}

dir_config('openexr_native')

create_makefile('openexr_native')
