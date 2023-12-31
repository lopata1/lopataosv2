[bits 16]
[org 0x7c00]


mov al, 'L'
mov ah, 0x0e
int 0x10

mov ax, 0x1000
mov es, ax
xor bx, bx

mov ah, 0x02
mov al, 0x50
mov ch, 0x00
mov cl, 0x02
mov dh, 0x00
;mov dl, 0x80
int 0x13

jmp skip

mov ax, 0x1000
mov es, ax
xor bx, bx

mov ah, 0x02
mov al, 0x0F
mov ch, 0x00
mov cl, 0x02
mov dh, 0x00
;mov dl, 0x80
int 0x13


load_kernel:
    mov ax, [current_time]
    mov cx, 0x2400
    mul cx

    add ax, 0x1000

    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, 0x12

    mov ch, [current_time]

    ;mov ch, 0x00
    ;mov cl, 0x02

    mov dh, 0x00
    ;mov dl, 0x80
    int 0x13

    mov ah, 0x01
    int 0x13

    inc word [current_time]
    mov ax, [current_time]
    cmp ax, [load_times] 
    jl load_kernel



skip:
mov ah, 0x01
int 0x13


mov al, 0x13
xor ah, ah
int 0x10


cli

in al, 0x92
or al, 2
out 0x92, al

mov ax, 0
mov ds, ax

lgdt [gdtr]
mov eax, cr0
or al, 1
mov cr0, eax

jmp 0x08:pmode_main


gdt:
    dq 0

    ;code
    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0

    ;data
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0


gdtr:
    dw gdtr - gdt - 1
    dd gdt

load_times dw 20
current_time dw 1

pmode_main:
    [bits 32]
    mov ax,10h
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    mov esp, 0x105000

    jmp 0x10000

times 510-($-$$) db 0
dw 0xaa55