CC=arm-linux-gcc
watchdogd:watchdog.c
	$(CC) -o watchdogd watchdog.c -I../include -L../ -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
clean:
	rm -r watchdogd
