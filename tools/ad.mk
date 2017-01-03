CC=arm-linux-gcc
alarmd:alarmd.c
	$(CC) -o alarmd alarmd.c -I../include -L../ -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
clean:
	rm -r alarmd
