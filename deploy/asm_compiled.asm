format ELF64 executable
LINE_MAX equ 1024
segment readable executable
include "./include/linux.inc"
include "./include/utils.inc"
entry _start
_start:
    mov rbp, rsp
    sub rsp, 48
    mov rdi, 1
    mov rsi, input_label
    mov rdx, 15
    mov rax, 1
    syscall
    read 0, line, LINE_MAX
    mov rdi, line
    call strlen
    mov rdi, line
    mov rsi, rax
    call parse_uint
    mov qword [rbp - 8], rax
    mov rax, qword [rbp - 8]
    mov rbx, rax
    mov rax, 2
    sub rbx, rax
    mov rax, rbx
    mov qword [rbp - 16], rax
    mov rax, 0
    mov qword [rbp - 24], rax
    mov rax, 1
    mov qword [rbp - 32], rax
    mov rax, 0
    mov qword [rbp - 40], rax
    mov rax, qword [rbp - 40]
    mov rdx, rax
    mov rax, 1
    cmp rdx, rax
    setl al
    and al, 1
    movzx rax, al
    test rax, rax
    jz .endif0
    jmp .start
.endif0:
.comma:
    mov rdi, 1
    mov rsi, comma
    mov rdx, 1
    mov rax, 1
    syscall
    mov rax, qword [rbp - 40]
    mov rdx, rax
    mov rax, 0
    cmp rdx, rax
    setg al
    and al, 1
    movzx rax, al
    test rax, rax
    jz .endif1
    jmp .loop
.endif1:
.start:
    mov rax, qword [rbp - 24]
    mov rdi, 1
    mov rsi, rax
    call write_uint
    mov rdi, 1
    mov rsi, comma
    mov rdx, 1
    mov rax, 1
    syscall
    mov rax, qword [rbp - 32]
    mov rdi, 1
    mov rsi, rax
    call write_uint
    mov rdi, 1
    mov rsi, comma
    mov rdx, 1
    mov rax, 1
    syscall
.loop:
    mov rax, qword [rbp - 24]
    mov rdx, rax
    mov rax, qword [rbp - 32]
    add rax, rdx
    mov qword [rbp - 48], rax
    mov rax, qword [rbp - 48]
    mov rdi, 1
    mov rsi, rax
    call write_uint
    mov rax, qword [rbp - 32]
    mov qword [rbp - 24], rax
    mov rax, qword [rbp - 48]
    mov qword [rbp - 32], rax
    mov rax, qword [rbp - 40]
    mov rdx, rax
    mov rax, 1
    add rax, rdx
    mov qword [rbp - 40], rax
    mov rax, qword [rbp - 40]
    mov rdx, rax
    mov rax, qword [rbp - 16]
    cmp rdx, rax
    setl al
    and al, 1
    movzx rax, al
    test rax, rax
    jz .endif2
    jmp .comma
.endif2:
    add rsp, 48
    mov rdi, 1
    mov rsi, newline_label
    mov rdx, 1
    mov rax, 1
    syscall
    mov rax, 60
    xor rdi, rdi
    syscall
segment readable writeable
line rb LINE_MAX
comma db ',',0
newline_label: db 0x0A ,0
input_label: db "Input: ",0
