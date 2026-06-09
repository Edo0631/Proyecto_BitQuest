global contar_caracter
global validar_movimiento
global detectar_objeto
global calcular_puntaje
global contar_libres

section .text

; ---------------------------------
; contar_caracter
; ---------------------------------
contar_caracter:
    mov r9, 0
    mov rax, 0

.loop1:
    cmp rax, rdx
    jge .fin1

    mov bl, [rcx + rax]
    cmp bl, r8b
    jne .skip1
    inc r9

.skip1:
    inc rax
    jmp .loop1

.fin1:
    mov rax, r9
    ret


; ---------------------------------
; validar_movimiento
; ---------------------------------
validar_movimiento:
    mov rax, r8
    imul rax, rdx
    add rax, r9

    mov bl, [rcx + rax]

    cmp bl, '#'
    je .bloq

    mov rax, 1
    ret

.bloq:
    mov rax, 0
    ret


; ---------------------------------
; detectar_objeto
; ---------------------------------
detectar_objeto:
    ; 5to parámetro (objeto)
    mov r10b, [rsp + 40]
    cmp bl, r10b

    mov rax, r8
    imul rax, rdx
    add rax, r9

    mov bl, [rcx + rax]

    cmp bl, r10b
    je .ok

    mov rax, 0
    ret

.ok:
    mov rax, 1
    ret


; ---------------------------------
; calcular_puntaje
; ---------------------------------
calcular_puntaje:
    
    mov rax, rcx
    imul rax, 100 ;monedas *100

    mov r9, rdx
    imul r9, 2
    sub rax, r9   ; restar pasos * 2

    mov r9, r8
    imul r9, 500
    add rax, r9   ; sumar niveles * 500

    ret


; ---------------------------------
; contar_libres
; ---------------------------------
contar_libres:
    mov r9, 0
    mov rax, 0

.loop2:
    cmp rax, rdx
    jge .fin2

    mov bl, [rcx + rax]
    cmp bl, '.'
    jne .skip2

    inc r9

.skip2:
    inc rax
    jmp .loop2

.fin2:
    mov rax, r9
    ret
