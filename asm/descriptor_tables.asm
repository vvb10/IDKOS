global idt_flush
idt_flush:
    lidt [rdi]
    ; sti renable after making APIC driver
    ret

global gdt_flush
gdt_install:
	lgdt [rdi]

gdt_flush:
	push rax
	mov rax, rsp
	push 0
	push rax
	pushf
	push 8
	push flush_done
	iretq

flush_done:
	xor rax, rax
	mov ax, 0
	mov ds, ax
	mov ss, ax
	mov es, ax
	pop rax
	ret