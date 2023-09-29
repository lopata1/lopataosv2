global memcpy
global memset

memcpy:
    push ebp
    mov ebp, esp

    mov ecx, 320*200

    mov esi, [ebp+8]
    mov edi, [ebp+12]

    rep movsb

    mov esp, ebp
    pop ebp
    ret

memset:
    push ebp
    mov ebp, esp

    mov ecx, 320*200

    mov edi, [ebp+8]
    mov al, [ebp+12]

    rep stosb

    mov esp, ebp
    pop ebp
    ret