#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

Binding* bindings;
int bindings_size = 0;

#define MAX_TOKENS 3

char* tokens[MAX_TOKENS];
int num_tokens;

void tokenize(char* source) {
    //Reset the amount of tokens per parse
    num_tokens = 0;
    char* token = strtok(source, " ");
    while(token != NULL && num_tokens < MAX_TOKENS) {
        tokens[num_tokens] = strdup(token);
        token = strtok(NULL, " ");
        num_tokens++;
    }
}

int find_id(const char* id) {
    for(int i = 0; i < bindings_size; ++i) {
        if(strcmp(id, bindings[i].id) == 0) {
            return i;
        }
    }
    return -1;
}

Node* parse_var(char** tokens) {
    Node* root_node = malloc(sizeof(Node));
    assert(root_node && "Failed to allocate root node for parsing a variable");

    root_node->m_type = NODE_TYPE_VAR;

    char* id = malloc(strlen(tokens[0]) + 1);
    assert(id && "Could not allocate memory for identifier");
    strcpy(id, tokens[0]);
    int value = atoi(tokens[2]);
    bindings_size++;
    bindings = realloc(bindings, (sizeof(Binding) * bindings_size));
    assert(bindings && "Failed to reallocate bindings array");

    bindings[bindings_size - 1].id = id;
    bindings[bindings_size -1].value = value;

    return root_node;
}

Node* parse_func(char** tokens) {
    Node* root_node = malloc(sizeof(Node));
    root_node->m_type = NODE_TYPE_FUN;

    char* search_id = strdup(tokens[1]);

    int indx = find_id(search_id);
    
    if(indx != -1) {
        printf("%d\n", bindings[indx].value);
    }
    
    else {
        printf("Unknown identifier\n");
    }

    free(search_id);
    return root_node;
}

void parse(char** tokens) {
    if(num_tokens < 3) {
        if(strcmp(tokens[0], "print") == 0) {
            parse_func(tokens);
        }
    }
    else if(num_tokens == 3) {
        if(strcmp(tokens[1], "=") == 0) {
            parse_var(tokens);
        }
    }
}

//TODO
void print_help() {
    printf("todo\n");
}

#define INPUT_SIZE 256

int main(void) {
    while(1) {
        printf("Math> ");
        char input[INPUT_SIZE];
        fgets(input, INPUT_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0';
        if(strcmp(input, "help") == 0) {
            print_help();
        }
        else if(strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
           return EXIT_SUCCESS; 
        }
        else {
            tokenize(input);
            parse(tokens);
        }
    }
}
