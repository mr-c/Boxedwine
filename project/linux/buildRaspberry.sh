#!/bin/bash
mkdir -p bin
sh buildPocoLib.sh
gcc -std=c++17 -O2 -Wall -Wno-psabi -Wno-psabi -Wno-delete-incomplete -Wno-unused-result -Wno-unknown-pragmas -Wno-unused-local-typedefs -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -I../../include -I../../lib/poco/Net/include -I../../lib/poco/Crypto/include -I../../lib/poco/Util/include -I../../lib/poco/Foundation/include -I../../lib/poco/NetSSL_OpenSSL/include -I../../lib/glew/include -I../../lib/imgui ../../lib/imgui/imgui.cpp ../../lib/pugixml/src/*.cpp ../../lib/imgui/imgui_draw.cpp ../../lib/imgui/imgui_widgets.cpp ../../lib/imgui/examples/imgui_impl_opengl3.cpp ../../lib/imgui/examples/imgui_impl_sdl.cpp ../../lib/imgui/addon/imguitinyfiledialogs.cpp ../../source/sdl/*.cpp ../../source/sdl/singleThreaded/*.cpp ../../lib/glew/src/glew.cpp ../../source/ui/*.cpp ../../source/ui/controls/*.cpp ../../source/ui/data/*.cpp ../../source/ui/opengl/*.cpp ../../source/ui/utils/*.cpp ../../platform/sdl/*.cpp ../../platform/linux/*.cpp ../../source/emulation/cpu/*.cpp ../../source/emulation/cpu/common/*.cpp ../../source/emulation/cpu/normal/*.cpp ../../source/emulation/softmmu/*.cpp ../../source/io/*.cpp ../../source/kernel/*.cpp ../../source/kernel/devs/*.cpp ../../source/kernel/proc/*.cpp ../../source/kernel/sys/*.cpp ../../source/kernel/loader/*.cpp ../../source/util/*.cpp ../../source/opengl/sdl/*.cpp ../../source/opengl/*.cpp -o bin/boxedwine -L./linux_build/lib -lPocoNetSSL -lPocoNet -lPocoCrypto -lPocoUtil -lPocoFoundation -lssl -lcrypto -lpthread -lm -lz -lminizip -DBOXEDWINE_RECORDER -DBOXEDWINE_ZLIB -DBOXEDWINE_HAS_SETJMP -DSDL2=1 "-DGLH=<SDL_opengl.h>" `sdl2-config --cflags --libs` -lGL -lstdc++ -lstdc++fs -DSIMDE_SSE2_NO_NATIVE -DPOCO_UTIL_NO_JSONCONFIGURATION -DPOCO_UTIL_NO_XMLCONFIGURATION -DBOXEDWINE_POSIX -DBOXEDWINE_OPENGL_SDL