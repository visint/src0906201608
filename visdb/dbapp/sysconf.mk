ARMCC=arm-linux-gcc

sysconf:sysconf.c
	$(ARMCC) -o sysconf sysconf.c -I../include -L../ -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3

clean:
	rm -r sysconf
