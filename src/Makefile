
linux:
	# $(pkg-config --cflags --libs pangocairo fontconfig)
	cc -O2 -std=gnu11 -fPIC -Wall -shared -o libcairo.so libcairo.c -pthread -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/fribidi -I/usr/include/harfbuzz -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -lpangocairo-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lharfbuzz -lcairo -lfontconfig -lfreetype -llua

macos:
	cc -O2 -std=gnu99 -fPIC -Wall -dynamiclib -o libcairo.dylib libcairo.c -I/usr/local/Cellar/pango/1.50.14/include/pango-1.0 -I/usr/local/Cellar/harfbuzz/7.1.0/include/harfbuzz -I/usr/local/Cellar/pango/1.50.14/include/pango-1.0 -I/usr/local/Cellar/glib/2.74.6/include -I/usr/local/Cellar/fribidi/1.0.12/include/fribidi -I/usr/local/Cellar/harfbuzz/7.1.0/include/harfbuzz -I/usr/local/Cellar/graphite2/1.3.14/include -I/usr/local/Cellar/cairo/1.16.0_5/include/cairo -I/usr/local/Cellar/glib/2.74.6/include -I/usr/local/Cellar/glib/2.74.6/include/glib-2.0 -I/usr/local/Cellar/glib/2.74.6/lib/glib-2.0/include -I/usr/local/opt/gettext/include -I/usr/local/Cellar/pcre2/10.42/include -I/usr/local/Cellar/pixman/0.42.2/include/pixman-1 -I/usr/local/Cellar/libxcb/1.15_1/include -I/usr/local/Cellar/libxrender/0.9.11/include -I/usr/local/Cellar/libxext/1.3.5/include -I/usr/local/Cellar/libx11/1.8.4/include -I/usr/local/Cellar/libxcb/1.15_1/include -I/usr/local/Cellar/libxau/1.0.11/include -I/usr/local/Cellar/libxdmcp/1.1.4/include -I/usr/local/Cellar/fontconfig/2.14.2/include -I/usr/local/opt/freetype/include/freetype2 -I/usr/local/Cellar/libpng/1.6.39/include/libpng16 -I/usr/local/Cellar/xorgproto/2022.2/include -I/Library/Developer/CommandLineTools/SDKs/MacOSX12.sdk/usr/include/ffi -L/usr/local/Cellar/pango/1.50.14/lib -L/usr/local/Cellar/glib/2.74.6/lib -L/usr/local/opt/gettext/lib -L/usr/local/Cellar/harfbuzz/7.1.0/lib -L/usr/local/Cellar/cairo/1.16.0_5/lib -L/usr/local/Cellar/fontconfig/2.14.2/lib -L/usr/local/opt/freetype/lib -lpangocairo-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lintl -lharfbuzz -lcairo -lfontconfig -lfreetype -llua

install:
	mkdir -p /usr/local/lib/lua/5.4/	# just for ensuring that the dest dir exists
	mkdir -p /usr/local/share/lua/5.4/	# just for ensuring that the dest dir exists
	mv libcairo.so /usr/local/lib/lua/5.4/
	cp cairo.lua /usr/local/share/lua/5.4/

install-macos:
	mkdir -p /usr/local/lib/lua/5.4/	# just for ensuring that the dest dir exists
	mkdir -p /usr/local/share/lua/5.4/	# just for ensuring that the dest dir exists
	mv libcairo.dylib /usr/local/lib/lua/5.4/
	cp cairo.lua /usr/local/share/lua/5.4/
test:
	cd ../test && lua test-libc.lua -v > README
