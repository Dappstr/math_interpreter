#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum NODE_TYPE {
    NODE_TYPE_INST = 0,
    NODE_TYPE_NUM
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
    struct Node* m_left;
    struct Node* m_right;

} Node;

#define INPUT_SIZE 256
#define MAX_TOKENS 3

char tokens[MAX_TOKENS][INPUT_SIZE];
int num_tokens;

void tokenize(char* input) {
    num_tokens = 0;
    char* token = strtok(input, " ");
    while(token != NULL) {
        strcpy(tokens[num_tokens++], token);
        token = strtok(NULL, " ");
    }
}

Node* parse() {
    if(num_tokens < 3) {
        printf("Cannot parse less than 3 tokens");
        return NULL;
    }

    Node* root_node = malloc(sizeof(Node));
    root_node->m_type = NODE_TYPE_INST;
    
    if(strcmp(tokens[0], "ADD") == 0 || strcmp(tokens[0], "add") == 0) {
        root_node->instruction = INST_ADD;
    }

    else if(strcmp(tokens[0], "SUB") == 0 || strcmp(tokens[0], "sub") == 0) {
        root_node->instruction = INST_SUB;
    }

    else if(strcmp(tokens[0], "MUL") == 0 || strcmp(tokens[0], "mul") == 0) {
        root_node->instruction = INST_MUL;
    }

    else if(strcmp(tokens[0], "DIV") == 0 || strcmp(tokens[0], "div") == 0) {
        root_node->instruction = INST_DIV;
    }

    else if(strcmp(tokens[0], "MOD") == 0 || strcmp(tokens[0], "mnod") == 0) {
        root_node->instruction = INST_MOD;
    }

    root_node->m_left = malloc(sizeof(Node));
    root_node->m_left->m_type = NODE_TYPE_NUM;
    root_node->m_left->value = atoi(tokens[1]);

    root_node->m_right = malloc(sizeof(Node));
    root_node->m_right->m_type = NODE_TYPE_NUM;
    root_node->m_right->value = atoi(tokens[2]);
    
    return root_node;
}

int evaluate(Node* node) {
    if(node->m_type == NODE_TYPE_NUM) {
        return node->value;
    }

    else if(node->m_type == NODE_TYPE_INST) {
        int left_val = evaluate(node->m_left);
        int right_val = evaluate(node->m_right);
        
        switch(node->instruction) {
            case INST_ADD:
                return left_val + right_val;
            case INST_SUB:
                return left_val - right_val;
            case INST_MUL:
                return left_val * right_val;
            case INST_DIV:
                return left_val / right_val;
            case INST_MOD:
                return left_val % right_val;
        }
    }
    return 0;
}

void print_help() {
    printf("Help:\n\tADD x y for addition\n\tSUB x y for subtraction\n\tMUL x y for multiplication\n\tDIV x y for division\n\tMOD x y for modulus\n\t\"quit\" to exit\n\n");
}

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
            Node* root = parse();
            if(root) {
                int result = evaluate(root);
                printf("RESULT: %d\n", result);
            }
        }
    }
    
    return EXIT_SUCCESS;
}
