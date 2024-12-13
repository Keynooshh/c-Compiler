format ELF64 executable
LINE_MAX equ 1024
segment readable executable
include "./include/linux.inc"
include "./include/utils.inc"
entry _start
_start:
    mov rbp, rsp
    sub rsp, 56
    mov rax, 4
    mov qword [rbp - 8], rax
    mov rax, qword [rbp - 8]
    mov rbx, rax
    mov rax, 1
    sub rbx, rax
    mov rax, rbx
    mov qword [rbp - 16], rax
    mov rax, qword [rbp - 16]
    mov rdi, 1
    mov rsi, rax
    call write_uint
    mov rax, 0
    mov qword [rbp - 24], rax
    mov rax, 1
    mov qword [rbp - 32], rax
    mov rax, 2
    mov qword [rbp - 40], rax
    mov rax, 2
    mov rbx, rax
    mov rax, 2
    sub rbx, rax
    mov rax, rbx
    mov qword [rbp - 48], rax
    mov rax, qword [rbp - 48]
    mov rdi, 1
    mov rsi, rax
    call write_uint
    mov rdi, 1
    mov rsi, comma
    mov rdx, 1
    mov rax, 1
    syscall
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
    mov qword [rbp - 56], rax
    mov rax, qword [rbp - 56]
    mov rdi, 1
    mov rsi, rax
    call write_uint
    mov rax, qword [rbp - 32]
    mov qword [rbp - 24], rax
    mov rax, qword [rbp - 56]
    mov qword [rbp - 32], rax
    mov rax, qword [rbp - 40]
    mov rdx, rax
    mov rax, qword [rbp - 16]
    cmp rdx, rax
    setl al
    and al, 1
    movzx rax, al
    test rax, rax
    jz .endif0
    jmp .end
.endif0:
    mov rdi, 1
    mov rsi, comma
    mov rdx, 1
    mov rax, 1
    syscall
    mov rax, qword [rbp - 40]
    mov rdx, rax
    mov rax, 1
    add rax, rdx
    mov qword [rbp - 40], rax
    mov rax, qword [rbp - 40]
    mov rdx, rax
    mov rax, qword [rbp - 8]
    cmp rdx, rax
    setl al
    and al, 1
    movzx rax, al
    test rax, rax
    jz .endif1
    jmp .loop
.endif1:
.end:
    add rsp, 56
    mov rax, 60
    xor rdi, rdi
    syscall
segment readable writeable
line rb LINE_MAX
comma db ','
