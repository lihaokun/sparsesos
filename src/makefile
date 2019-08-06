CXX=g++
MOSEKlib=-lmosek64 -lfusion64
CFLAGS=-O3 -flto -pthread 
LPATHS=-L../lib  '-Wl,-rpath=$$ORIGIN/../lib' -Wl,-rpath,../lib
dir=`pwd -P`
ifdef mosek9
	MOSEKIPATHS=-I../mosek/9.0/tools/platform/linux64x86/h -I../mosek/9.0/tools/platform/linux64x86/include -D mosek_v=9
#	MOSEKLPATHS=-L../mosek/9.0/tools/platform/linux64x86/bin -Wl,-rpath-link,../mosek/9.0/tools/platform/linux64x86/bin '-Wl,-rpath=$$ORIGIN/../mosek/9.0/tools/platform/linux64x86/bin'
else
	MOSEKIPATHS=-I../mosek/8/tools/platform/linux64x86/h -I../mosek/8/tools/platform/linux64x86/include
#	MOSEKLPATHS=-L../mosek/8/tools/platform/linux64x86/bin -Wl,-rpath-link,../mosek/8/tools/platform/linux64x86/bin '-Wl,-rpath=$$ORIGIN/../mosek/8/tools/platform/linux64x86/bin'
endif

objects=polynomial.o  sos.o sos_mosek.o is_sos.o   libsos.so  is_sos#sparsesos.o
is_sos: is_sos.o libsos.so
	install libsos.so ../lib
	$(CXX) $(CFLAGS) is_sos.o -o is_sos  $(LPATHS) -lsos
	install is_sos ../bin
libsos.so:polynomial.o sos_mosek.o sos.o
	$(CXX) $(CFLAGS) polynomial.o sos_mosek.o  sos.o -shared  -fPIC -o libsos.so $(LPATHS) $(MOSEKlib)
is_sos.o: is_sos.cpp sparsesos.h polynomial/polynomial.hpp sos.hpp  sos_mosek.hpp
	$(CXX) $(CFLAGS) is_sos.cpp -c 
sos.o: sos.cpp polynomial/polynomial.hpp sos.hpp
	$(CXX) $(CFLAGS) sos.cpp -c
sos_mosek.o: sos_mosek.cpp sos.hpp sos_mosek.hpp polynomial/polynomial.hpp
	$(CXX) $(CFLAGS) $(MOSEKIPATHS) sos_mosek.cpp -c 	
#sparsesos.o: sparsesos.cpp sparsesos.h polynomial/polynomial.hpp
#	$(CXX) $(CFLAGS) sparsesos.cpp -c
polynomial.o: polynomial/polynomial.cpp polynomial/polynomial.hpp
	$(CXX) $(CFLAGS) polynomial/polynomial.cpp -c
.PHONY : clean install uninstall
clean :
	-rm  $(objects)
	-rm ../bin/is_sos
	-rm ../lib/libsos.so

install :
	ln -sf ${dir}/../bin/is_sos /usr/bin/
	ln -sf ${dir}/../lib/libsos.so /usr/lib/
uninstall:
	-rm /usr/lib/libsos.so
	-rm /usr/bin/is_sos
