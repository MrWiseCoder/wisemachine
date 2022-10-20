#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

#include "wisemachine.h"


Wm wm = {0};

void inst_push(Wm* wm, const Word* op) {
    wm->stack[wm->stack_size++] = *op;
}

void inst_plus(Wm* wm) {
    printf("WM stack_size before plus: %ld\n", wm->stack_size);
    Word op1 = wm->stack[--wm->stack_size];
    printf("Op1:%ld\n", op1);
    Word op2 = wm->stack[--wm->stack_size];
    printf("Op2:%ld\n", op2);
    Word r = op1 + op2;
    printf("%ld+%ld=%ld\n", op1, op2, r);
    wm->stack[wm->stack_size++] = r;
    printf("WM stack_size after plus: %ld\n", wm->stack_size);
}

void inst_minus(Wm* wm) {
    printf("WM stack_size before plus: %ld\n", wm->stack_size);
    Word op1 = wm->stack[--wm->stack_size];
    printf("Op1:%ld\n", op1);
    Word op2 = wm->stack[--wm->stack_size];
    printf("Op2:%ld\n", op2);
    Word r = op1 - op2;
    printf("%ld-%ld=%ld\n", op1, op2, r);
    wm->stack[wm->stack_size++] = r;
    printf("WM stack_size after plus: %ld\n", wm->stack_size);
}

void inst_div(Wm* wm) {
    printf("WM stack_size before plus: %ld\n", wm->stack_size);
    Word op1 = wm->stack[--wm->stack_size];
    printf("Op1:%ld\n", op1);
    Word op2 = wm->stack[--wm->stack_size];
    printf("Op2:%ld\n", op2);
    Word r = op1 / op2;
    printf("%ld/%ld=%ld\n", op1, op2, r);
    wm->stack[wm->stack_size++] = r;
    printf("WM stack_size after plus: %ld\n", wm->stack_size);
}

void inst_mult(Wm* wm) {
    printf("WM stack_size before plus: %ld\n", wm->stack_size);
    Word op1 = wm->stack[--wm->stack_size];
    printf("Op1:%ld\n", op1);
    Word op2 = wm->stack[--wm->stack_size];
    printf("Op2:%ld\n", op2);
    Word r = op1 * op2;
    printf("%ld*%ld=%ld\n", op1, op2, r);
    wm->stack[wm->stack_size++] = r;
    printf("WM stack_size after plus: %ld\n", wm->stack_size);
}

void inst_pow(Wm* wm) {
    printf("WM stack_size before plus: %ld\n", wm->stack_size);
    Word op1 = wm->stack[--wm->stack_size];
    printf("Op1:%ld\n", op1);
    Word op2 = wm->stack[--wm->stack_size];
    printf("Op2:%ld\n", op2);
    Word r = pow(op1, op2);
    printf("%ld**%ld=%ld\n", op1, op2, r);
    wm->stack[wm->stack_size++] = r;
    printf("WM stack_size after plus: %ld\n", wm->stack_size);
}

void wm_dump_stack(Wm* wm) {
    printf("Stack size: %ld\n", wm->stack_size);
    printf("Stack: [\n");
    for (int i = 0; i < wm->stack_size; i++) {
        printf("\t%ld,\n", wm->stack[i]);
    }
    printf(" ]\n");
}

void inst_as_cstr(Instruction* inst) {
    switch (inst->inst_kind) {
        case PUSH:
            printf("Instruction { operand=%ld,  kind=%s }\n", inst->operand, "PUSH");
            break;
        case PLUS:
            printf("Instruction { operand=%ld,  kind=%s }\n", inst->operand, "PLUS");
            break;
        case MINUS: break;
        case DIV: break;
        case MULT: break;
        case POW: break;
        case JMP: break;
        case HALT: break;
        default: break;
    }

}

void inst_print(Instruction* inst) {
    /** printf("Instruction: %ld, %d \n", inst->operand, inst->inst_kind); */
    inst_as_cstr(inst);
}

Err execute_instructions(Wm* wm, Instruction* inst) {
    inst_print(inst);
    switch (inst->inst_kind) {
        case PUSH:
            if (wm->stack_size + 1 > WM_STACK_SIZE) {
                return STACK_OVERFLOW;
            }
            inst_push(wm, &inst->operand);
            wm->stack_pointer++;
            break;
        case PLUS:
            if (wm->stack_size - 2 < 0) {
                return STACK_UNDERFLOW;
            }
            inst_plus(wm);
            wm->stack_pointer++;
            break;
        case MINUS:
            if (wm->stack_size - 2 < 0) {
                return STACK_UNDERFLOW;
            }
            inst_minus(wm);
            wm->stack_pointer++;
            break;
        case DIV:
            if (wm->stack_size - 2 < 0) {
                return STACK_UNDERFLOW;
            }
            inst_div(wm);
            wm->stack_pointer++;
            break;
        case MULT:
            if (wm->stack_size - 2 < 0) {
                return STACK_UNDERFLOW;
            }
            inst_mult(wm);
            wm->stack_pointer++;
            break;
        case POW:
            if (wm->stack_size - 2 < 0) {
                return STACK_UNDERFLOW;
            }
            inst_pow(wm);
            wm->stack_pointer++;
            break;
        default:
            return ILLEGAL_INSTRUCTION;
            break;
    }
    return NO_ERR;
}

void err_print(Wm* wm, Err e) {
    switch (e) {
        case NO_ERR:
            wm_dump_stack(wm);
            break;
        case STACK_OVERFLOW:
            fprintf(stderr, "[Error] Stack over flow\n");
            exit(1);
            break;
        case STACK_UNDERFLOW:
            fprintf(stderr, "[Error] Stack under flow\n");
            exit(2);
            break;
        case ILLEGAL_INSTRUCTION:
            fprintf(stderr, "[Error] Illegal \n");
            exit(3);
            break;
        default:
            fprintf(stderr, "[Error] Unreachable \n");
            exit(4);
            break;
    }
}

void load_program(Wm* wm, const Instruction* prog, size_t prog_size) {
    for (int i = 0; i < prog_size; i++) {
        wm->program[wm->program_size++] = prog[i];
    }
}

void load_program_from_disk(Wm* wm, const char* filepath) {
    /** printf("%s\n", __FUNCTION__); */
    FILE* fp = fopen(filepath, "rb");
    if(fp == NULL) {
        perror("fp == NULL");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    long size_f = ftell(fp);
    long size2 = sizeof(wm->program[0]);
    /** printf("Size ftell %ld\n", size_f); */
    /** printf("Size single element of program %ld\n", size2); */
    fseek(fp, 0, SEEK_SET);
    long prog_size = fread(wm->program, size2, size_f / size2, fp);
    wm->program_size = prog_size;
    /** printf("prog_size %ld\n", prog_size); */
    /** printf("prog 0 operand %ld\n", wm->program[0].operand); */
    /** printf("prog 1 operand %ld\n", wm->program[1].operand); */
    if(ferror(fp)) {
        perror("Read error");
        exit(1);
    }
    fclose(fp);
}

void save_program(const char* filepath, const Instruction* prog, size_t prog_size) {
    /** printf("%s\n", __FUNCTION__); */
    FILE* fp = fopen(filepath, "wb");
    if(fp == NULL) {
        perror("fp == NULL");
        exit(1);
    }
    fwrite(prog, sizeof(prog[0]), prog_size, fp);
    if(ferror(fp)) {
        perror("Write error");
        exit(1);
    }
    fclose(fp);
}

void pass(const char* f) {
    fprintf(stderr, "Not implemented yet: '%s'\n", f);
}

void print_prog(Instruction* prog, size_t prog_size) {
    for (int i = 0; i < prog_size; i++) {
        /** printf("Prog[%d]: %ld\n", i, prog[i].operand); */
        inst_print(&prog[i]);
    }
}

Err execute_program(Wm* wm) {
    printf("\t -> Stack Capacity: %d\n", WM_STACK_SIZE);
    /** printf("\t -> Program: \n"); */
    print_prog(wm->program, wm->program_size);
    for(int i = 0; !wm->halt && i < wm->program_size; i++) {
        Err e = execute_instructions(wm, &wm->program[i]);
        err_print(wm, e);
        if (e != NO_ERR) {
            return e;
        }
    }
    return NO_ERR;
}

Instruction program[] = {
    { .operand=15, .inst_kind=PUSH },
    { .operand=4,  .inst_kind=PUSH },
    {              .inst_kind=PLUS }
};

Instruction program2[] = {
    INST_PUSH(3),
    INST_PUSH(2),
    INST_PLUS()
};

void test_program() {
    Word op1 = 12;
    Word op2 = 32;
    wm_dump_stack(&wm);
    inst_push(&wm, &op1);
    inst_push(&wm, &op2);
    inst_plus(&wm);
    wm_dump_stack(&wm);
}

void test_program2() {
    /** assert(WM_STACK_SIZE == wm.stack_size); */
    /** printf("\t -> Stack Capacity: %d\n", WM_STACK_SIZE); */
    /** for (int i = 0; i < 3; i++) { */
    /**     Err e = execute_instructions(&program[i]); */
    /**     err_print(e); */
    /** } */
    /** load_program(&wm, program, 3); */
    load_program(&wm, program2, 3);
    execute_program(&wm);
}

void test_program3() {
    /** save_program("./deneme.wm", program2, 3); */
    save_program("./deneme.wm", program, 3);
    load_program_from_disk(&wm, "./deneme.wm");
    execute_program(&wm);
}
