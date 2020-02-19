COMPILER = clang
CLANGARAMS = -target x86_64 -Wall -Wextra -Werror -std=c11 -mcmodel=kernel -mno-red-zone -mno-mmx -mno-80387 -mno-sse -mno-sse2 -mno-avx -nostdlib -ffreestanding -g -c -fno-stack-protector
LDPARAMS = -nostdlib -g -m elf_x86_64 -z max-page-size=4096 -T

CFILES := $(wildcard C/*.c)
ASMFILES := $(wildcard asm/*.asm)

kernel.elf: linker.ld $(CFILES) $(ASMFILES)
	for i in $(ASMFILES); do \
		nasm -f elf64 $$i -o $$(echo $$i | awk -F'[_.]' '{print $$1}').o; \
	done

	mv asm/*.o bin/
	$(COMPILER) $(CLANGARAMS) $(CFILES)
	mv *.o bin/
	ld $(LDPARAMS) $< -o $@ $$(ls bin/*.o)

iso: kernel.elf
	cp kernel.elf iso/boot/
	grub-mkrescue --output=kernel.iso iso

run:
	qemu-system-x86_64 --cdrom kernel.iso

all:
	make
	make iso

debug:
	qemu-system-x86_64 --cdrom kernel.iso -s -S

clean: kernel.elf
	-rm kernel.elf kernel.iso