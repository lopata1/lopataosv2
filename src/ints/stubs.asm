global timer_isr_stub
extern timer_isr
timer_isr_stub:
    pusha

    call timer_isr

    mov al, 0x20
    out 0x20, al

    popa
    iret


global keyboard_isr_stub
extern keyboard_isr
keyboard_isr_stub:
    pusha

    call keyboard_isr

    mov al, 0x20
    out 0x20, al

    popa
    iret