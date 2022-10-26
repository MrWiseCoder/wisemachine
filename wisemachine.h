#ifndef WISEMACHINE_H
#define WISEMACHINE_H

#define INST_PUSH(val) { .operand=val, .inst_kind=PUSH }
#define INST_PLUS() { .inst_kind=PLUS }

#define WM_STACK_SIZE 1024
#define WM_PROGRAM_SIZE 10

typedef int64_t Word;

typedef enum {
    NOP,
    PUSH,
    PLUS,
    MINUS,
    DIV,
    MULT,
    POW,
    JMP,
    JMP_IF,
    HALT
} INST_KIND;

typedef struct {
    Word operand;
    INST_KIND inst_kind;
} Instruction;

struct _wisemachine {
    Word stack[WM_STACK_SIZE];
    size_t stack_size;

    Instruction program[WM_PROGRAM_SIZE];
    size_t program_size;

    Word stack_pointer;

    int halt;
};

typedef struct _wisemachine Wm;

typedef enum {
    NO_ERR,
    STACK_OVERFLOW,
    STACK_UNDERFLOW,
    ILLEGAL_INSTRUCTION,
    ILLEGAL_MEMORY_ACCESS
} Err;


void inst_push(Wm*, const Word*);
void inst_plus(Wm*);
void inst_print(Instruction*);

Err execute_instructions(Wm*, Instruction*);
Err execute_program(Wm*);
void load_program(Wm*, const Instruction*, size_t);
void load_program_from_disk(Wm*, const char*);
void save_program(const char*, const Instruction*, size_t);
void err_print(Wm*, Err);
void pass(const char*);

void wm_dump_stack(Wm*);
void test_program();
void test_program2();
void test_program3();

#endif
