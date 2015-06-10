boot.o:
	i686-elf-as boot.s -o boot.o
	
kernel.o:
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	
link:
	i686-elf-gcc -T linker.ld -o ../bin/pracos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

# Main commands
all: boot.o kernel.o link

clean:
	rm *~; rm *.o;
