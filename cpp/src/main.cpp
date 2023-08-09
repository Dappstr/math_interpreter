#include <iostream>

//Representing what kind of token 
enum NODE_TYPE {
    NODE_TYPE_INST = 0, // Instruction such as ADD, SUB, MUL, DIV, MOD
    NODE_TYPE_NUM //Token is a numerical literal which will come after an instruction
};

enum INSTRUCTION {
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV,
    INST_MOD
};

struct Node {
    NODE_TYPE m_type;
    union {
        INSTRUCTION instruction;
        int value;
    };
    Node* m_left = nullptr;
    Node* m_right = nullptr;
};

std::vector<std::string> tokenize(const std::string& str) {
    std::vector<std::string> tokens;
    std::string token;

    for(char c: str) {
        if(c == ' ') {
            if(!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        }
        else {
            token += c;
        }
    }
        if(!token.empty()) {
            tokens.push_back(token);
        }

    return tokens;
}

Node* parse(const std::vector<std::string> tokens) {
    if(tokens.size() < 3) {
        std::cout << "Cannot parse less than 3 tokens\n";
        return nullptr;
    }

    Node* root_node = new Node;
    root_node->m_type = NODE_TYPE_INST;

    if(tokens[0] == "ADD") {
        root_node->instruction = INST_ADD;
    }

    else if(tokens[0] == "SUB") {
        root_node->instruction = INST_SUB;
    }
    
    else if(tokens[0] == "MUL") {
        root_node->instruction = INST_MUL;
    }

    else if(tokens[0] == "DIV") {
        root_node->instruction = INST_DIV;
    }

    else if (tokens[0] == "MOD") {
        root_node->instruction = INST_MOD;
    }

    root_node->m_left = new Node;
    root_node->m_left->m_type = NODE_TYPE_NUM;
    root_node->m_left->value = std::stoi(tokens[1]);

    root_node->m_right = new Node;
    root_node->m_right->m_type = NODE_TYPE_NUM;
    root_node->m_right->value = std::stoi(tokens[2]);

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
    std::cout << "Help:\n";
    std::cout << "\tADD x y for addition\n"
              << "\tSUB x y for subtraction\n"
              << "\tMUL x y for multiplication\n"
              << "\tDIV x y for division\n"
              << "\tMOD x y for modulus\n"
              << "\t\"quit\" to exit\n\n";
}

int main() {
    std::string input;

    std::cout << "Type \"help\" to show help\n";
    while(true) {
        std::cout << "Math> ";
        std::getline(std::cin, input);
        if(input == "help"){
            print_help();
        }
        else if(input == "quit" || input == "exit") {
            return EXIT_SUCCESS;
        }

        else {
            std::vector<std::string> tokens = tokenize(input);
            Node* root = parse(tokens);
            if(root) {
                int result = evaluate(root);
                std::cout << "RESULT: " << result << '\n';
            }
            else {
                std::cout << "Unrecognized input or invalid command.\n";
            }
    
        }
    }
    return EXIT_SUCCESS;
}
