dir=`pwd -P`
.PHONY :build clean install uninstall
build:
	make -C ./src 
clean:
	make -C ./src  clean
install :
	ln -sf ${dir}/bin/is_sos /usr/bin/
	ln -sf ${dir}/lib/libsos.so /usr/lib/
uninstall:
	-rm /usr/lib/libsos.so
	-rm /usr/bin/is_sos
