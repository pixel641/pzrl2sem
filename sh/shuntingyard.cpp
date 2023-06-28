#include <iostream>
#include <sstream>
#include <string>
#include <stack>

int operator_precedence(char oper) {
    if (oper == '^') {
        return 3;
    }else if ((oper == '/') || (oper == '*')) {
        return 2;
    }else if ((oper == '-') || (oper == '+')) {
        return 1;
    }
    return -1;
}

std::string infixToPostfix(const std::string & infix) {
    const std::string operators = "-+/*^";

    std::stringstream infixStream(infix);
    std::stringstream result;

    char symbol;

    std::stack<char> stack;
    
    while (infixStream >> symbol) {
        if (std::isdigit(symbol)) {
            result << symbol << ' ';
        }
        size_t id = operators.find(symbol);
        if (id != std::string::npos) {
            int curr_prec = operator_precedence(symbol);
            while ((!stack.empty()) && 
                ((curr_prec < operator_precedence(stack.top())) 
                || ((curr_prec == operator_precedence(stack.top())) 
                && (symbol != '^')))) {
                result << stack.top() << ' ';
                stack.pop();
            }
            stack.push(symbol);
        }
        if (symbol == '(') {
            stack.push(symbol);
        }
        if (symbol == ')') {
            while ((!stack.empty()) && ('(' != stack.top())) {
                result << stack.top() << ' ';
                stack.pop();
            }
            stack.pop();
        }
    }
    while (!stack.empty()) {
        result << stack.top() << ' ';
        stack.pop();
    }
    return result.str();
}

int main() {
    std::string str;
    std::getline(std::cin, str);
    std::string infix{"3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3"};
    std::cout << infixToPostfix(str) << '\n';
    return 0;
}