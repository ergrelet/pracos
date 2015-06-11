#list of c files
objects := $(patsubst %.c,%.o,$(wildcard src/*.c))

#rule to transform c files into o files
%.o : %.c
	i686-elf-gcc -Iinclude -c $^ -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	
boot:
	i686-elf-as src/boot.s -o src/boot.o

#c files -> o files
files : $(objects)

#linking
link:
	i686-elf-gcc -T src/linker.ld -o bin/pracos.bin -ffreestanding -O2 -nostdlib src/boot.o $(objects) -lgcc

# Whole process
all: boot files link

clean:
	rm *~; rm *.o; cd src; rm *~; rm *.o;
