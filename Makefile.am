AUTOMAKE_OPTIONS= foreign
bin_PROGRAMS= hosts
hosts_SOURCES= hosts.c
hosts_CFLAGS=-DPREFIX=\"${prefix}/\"

install-exec-hook:
	mkdir -p ${prefix}/data
	chmod u+s ${prefix}/bin/hosts
	ln -s ${prefix}/bin/hosts /usr/local/bin/hosts
