#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum NODE_TYPE {
    NODE_TYPE_INST = 0,
    NODE_TYPE_NUM,
    NODE_TYPE_VAR,
    NODE_TYPE_FUN,
    NODE_TYPE_EXP

} NODE_TYPE;

typedef enum INSTRUCTION {
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV,
    INST_MOD
} INSTRUCTION;

typedef struct Node {
    NODE_TYPE m_type;
    union {
        INSTRUCTION instruction;
        int value;
    };
    struct Node** children;

} Node;

typedef struct Binding {
    const char* id;
    int value;
} Binding;

char** valid_ids = NULL;
int valid_ids_size = 0;

Binding* bindings;
int bindings_size = 0;

#define INPUT_SIZE 256
#define MAX_TOKENS 3

char tokens[MAX_TOKENS][INPUT_SIZE];
int num_tokens;

int main(void) {

}
