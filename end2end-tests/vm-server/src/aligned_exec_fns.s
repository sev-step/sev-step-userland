.align 4096
.globl simple_cache_target_lfence_asm
simple_cache_target_lfence_asm:
    nop
    nop
    # rdi is addr of lookup table
    mov rax, 42
    lfence
    mov rdx, [rdi + (1* 64)]
    lfence
    mov rdx, [rdi + (5* 64)]
    lfence
    mov rdx, [rdi + (14* 64)]
    lfence
    ret

.align 4096

.align 4096
.globl simple_cache_target_asm
simple_cache_target_asm:
    nop
    nop
    # rdi is addr of lookup table
    mov rax, 42
    mov rdx, [rdi + (1* 64)]
    mov rdx, [rdi + (5* 64)]
    mov rdx, [rdi + (14* 64)]
    ret

.align 4096


.align 4096
.globl eval_cache_target_asm
eval_cache_target_asm:
    nop
    nop
    # rdi is addr of lookup table
    mov rax, 42

    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]

    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]

    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]

    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]

    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    mov rdx, [rdi + ( 1 * 64)]
    mov rdx, [rdi + (15 * 64)]
    ret
.align 4096



.align 4096
.globl eval_cache_target_lfence_asm
eval_cache_target_lfence_asm:
    nop
    nop
    # rdi is addr of lookup table
    mov rax, 42

    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence

    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence

    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence

    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence

    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    mov rdx, [rdi + ( 1 * 64)]
    lfence
    mov rdx, [rdi + (15 * 64)]
    lfence
    ret
.align 4096

# make sure our function starts on a fresh page
.align 4096
.globl nop_slide_asm
nop_slide_asm:
# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

# 100 nops
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop


    ret
    # make sure no other code is placed on the remainder of this page
    .align 4096
