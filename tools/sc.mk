CC=arm-linux-gcc
sysconf:sysconf.c
	$(CC) -o sysconf sysconf.c -I../include -L../ -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
clean:
	rm -r sysconf
