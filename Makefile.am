AUTOMAKE_OPTIONS= foreign
bin_PROGRAMS= hosts
hosts_SOURCES= hosts.c
hosts_CFLAGS=-DPREFIX=\"${prefix}/\"
hostsbindir=\"${prefix}/\"

install-exec-hook:
	chown root ${prefix}/bin/hosts
	chmod u+s ${prefix}/bin/hosts
	mkdir -p ${prefix}/data
	ln -s ${prefix}/bin/hosts /usr/local/bin/hosts
