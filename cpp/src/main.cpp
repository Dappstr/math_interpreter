#include <iostream>
#include <cctype>
#include <string>

//Representing what kind of token 
enum NODE_TYPE {
    NODE_TYPE_INST = 0, // Instruction such as ADD, SUB, MUL, DIV, MOD
    NODE_TYPE_NUM, //Node is a numerical literal which will come after an instruction
    NODE_TYPE_VAR, // Node will be a variable declaration
    NODE_TYPE_FUN, //Node will be a function call (such as "print")
    NODE_TYPE_EXP //Node will be an expression (such as 2 + 3)
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
    std::vector<Node*> m_children;

    ~Node() {
        for(auto child : m_children) {
            delete child;
        }
    }
};

//Will bind an identifier to a value
//Such as a = 5
//'a' will be the identifer, 5 is the value
struct Binding {
    std::string id;
    int value;
};

std::vector<std::string> valid_ids;
std::vector<Binding*> bindings;

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

int find_id(const std::string& id) {
    int i = 0;
    for(auto _id : valid_ids) {
        if(_id == id) {
            return i;
        }
        else {
            ++i;
        }
    }
    return -1;
}

//Parse instruction if instruction is found
Node* parse_inst(std::vector<std::string> tokens) {
  
    Node* root_node = new Node;
    root_node->m_type = NODE_TYPE_INST;

    if(tokens[0] == "ADD")  {
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

    Node* left = new Node;
    left->m_type = NODE_TYPE_NUM;
    try {
        left->value = std::stoi(tokens[1]);
    }
    catch(...) {
        int indx = find_id(tokens[1]);
        if(indx != -1) {
           left->value = bindings.at(indx)->value; 
        }
        else {
            std::cout << "Unexpected input (value x)\n";
        }
    }

    Node* right = new Node;
    right->m_type = NODE_TYPE_NUM;
    try {
        right->value = std::stoi(tokens[2]);
    }
    catch(...) {
        int indx = find_id(tokens[2]);
        if(indx != -1) {
            right->value = bindings.at(indx)->value;
        }
        else {
            std::cout << "Unexpected input (value y)\n";
        }
    }
    root_node->m_children.push_back(left);
    root_node->m_children.push_back(right);
    return root_node;
}

//Parse variable if variable declaration is found
Node* parse_var(std::vector<std::string> tokens) {
    Node* root_node = new Node;
    root_node->m_type = NODE_TYPE_VAR;

    Binding* bind = new Binding;
    if(tokens[1] == "=") {
        std::string id = tokens[0];
        bind->id = id;
        valid_ids.push_back(id);
        bind->id = id;
        try {
            int value = std::stoi(tokens[2]);
            bind->value = value;
            bindings.push_back(bind);
        }
        catch(...) {
            std::cout << "Unexpected value during variable declaration\n";
        }
    }
    return root_node;
}

Node* parse_fun(std::vector<std::string> tokens) {
    Node* root_node = new Node;
    root_node->m_type = NODE_TYPE_FUN;

    if(tokens.size() < 2) {
        std::cout << "Invalid function call\n";
        return root_node;
    }

    std::string id = tokens.at(1);
    int indx = find_id(id);
    if(indx == -1) {
        std::cout << "Invalid id\n";
    }
    else {
        std::cout << bindings.at(indx)->value << '\n';
    }
    return root_node;
}

Node* parse_expr(std::vector<std::string> tokens) {
    Node* root_node = new Node;
    root_node->m_type = NODE_TYPE_EXP;

    int x = stoi(tokens[0]);
    int y = stoi(tokens[2]);
    char op = tokens[1].at(0);

    Node* left = new Node;
    left->m_type = NODE_TYPE_NUM;
    left->value = x;

    Node* right = new Node;
    right->m_type = NODE_TYPE_NUM;
    right->value = y;

    if(op == '+') {
        root_node->instruction = INST_ADD;
        root_node->m_children.push_back(left);
        root_node->m_children.push_back(right);
    }
    else if(op == '-') {
        root_node->instruction = INST_SUB;
        root_node->m_children.push_back(left);
        root_node->m_children.push_back(right);
    }
    else if(op == '*') {
        root_node->instruction = INST_MUL;
        root_node->m_children.push_back(left);
        root_node->m_children.push_back(right);
    }
    else if(op == '/') {
        root_node->instruction = INST_DIV;
        root_node->m_children.push_back(left);
        root_node->m_children.push_back(right);
    }
    else if(op == '%') {
        root_node->instruction = INST_MOD;
        root_node->m_children.push_back(left);
        root_node->m_children.push_back(right);
    }
    else {
        std::cout << "Unknown expression type\n\n";
    }

    return root_node;
}

Node* parse(const std::vector<std::string> tokens) {
    std::string command = tokens.at(0);
    if(command == "print") {
        return parse_fun(tokens);
    }
    else if(tokens.size() == 3) {
        std::string var = tokens.at(1);
        if(var[0] == '=') {
            return parse_var(tokens);
        }
        else if(command == "ADD" || command == "SUB" || command == "MUL" || command == "DIV" || command == "MOD") {
            return parse_inst(tokens);
        } 

        else {
            return parse_expr(tokens);
        }
    
    }
    else {
        std::cout << "Could not parse tokens\n";
        return nullptr;
    }
}

int evaluate(Node* node) {
    if(node->m_type == NODE_TYPE_NUM) {
        return node->value;
    }
    else if(node->m_type == NODE_TYPE_INST) {
        int left_val = evaluate(node->m_children.at(0));
        int right_val = evaluate(node->m_children.at(1));

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

    else if(node->m_type == NODE_TYPE_EXP) {
        int left_val = evaluate( node->m_children.at(0)); 
        int right_val = evaluate(node->m_children.at(1)); 
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
                if(root->m_type != NODE_TYPE_FUN) {
                    int result = evaluate(root);
                    std::cout << "RESULT: " << result << "\n\n";
                }
                delete root;
            }
            else {
                std::cout << "Unrecognized input or invalid command.\n";
            }
             
        }
    }
    return EXIT_SUCCESS;
}
