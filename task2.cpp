#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to convert infix expression to postfix notation
string infixToPostfix(const string& infix) {
    string postfix;             // Resultant postfix expression
    vector<char> operators;     // Vector used as a stack to store operators
    vector<int> precedence(256, 0); // Precedence table for operators
    precedence['|'] = 1;        // OR operator precedence
    precedence['&'] = 2;        // AND operator precedence
    precedence['!'] = 3;        // NOT operator precedence

    for (char c : infix) {
        if (c == ' ') continue; // Skip whitespace
        if (c >= 'A' && c <= 'Z') { // Operand (variable like A, B, C)
            postfix += c;       // Add operand directly to postfix
        } else if (c == '(') {  // Left parenthesis
            operators.push_back(c); // Push to the stack
        } else if (c == ')') {  // Right parenthesis
            // Pop operators from the stack until a '(' is encountered
            while (!operators.empty() && operators.back() != '(') {
                postfix += operators.back();
                operators.pop_back();
            }
            if (!operators.empty()) operators.pop_back(); // Pop the '('
        } else { // An operator
            // Pop operators with higher or equal precedence and add them to postfix
            while (!operators.empty() && precedence[operators.back()] >= precedence[c]) {
                postfix += operators.back();
                operators.pop_back();
            }
            operators.push_back(c); // Push the current operator to the stack
        }
    }

    // Append remaining operators in the stack to postfix
    while (!operators.empty()) {
        postfix += operators.back();
        operators.pop_back();
    }

    return postfix;
}

// Function to evaluate a logical expression in postfix form
bool evaluateExpression(const string& expr, bool A, bool B, bool C) {
    vector<bool> stack; // Vector used as a stack to evaluate postfix expressions

    for (char c : expr) {
        if (c == 'A') {
            stack.push_back(A); // Push value of A
        } else if (c == 'B') {
            stack.push_back(B); // Push value of B
        } else if (c == 'C') {
            stack.push_back(C); // Push value of C
        } else if (c == '!') {
            // Perform NOT operation on the top of the stack
            bool val = stack.back();
            stack.pop_back();
            stack.push_back(!val);
        } else if (c == '&') {
            // Perform AND operation on the top two elements of the stack
            bool val1 = stack.back(); stack.pop_back();
            bool val2 = stack.back(); stack.pop_back();
            stack.push_back(val1 && val2);
        } else if (c == '|') {
            // Perform OR operation on the top two elements of the stack
            bool val1 = stack.back(); stack.pop_back();
            bool val2 = stack.back(); stack.pop_back();
            stack.push_back(val1 || val2);
        }
    }

    // The final result of the expression is at the top of the stack
    return stack.back();
}

// Function to generate all input combinations for A, B, C
vector<vector<bool>> generateInputs() {
    vector<vector<bool>> inputs;
    // Generate all 8 combinations of 3 boolean variables (2^3 = 8)
    for (int i = 0; i < 8; ++i) {
        inputs.push_back({
            (i & 4) != 0, // Bit 2 (A)
            (i & 2) != 0, // Bit 1 (B)
            (i & 1) != 0  // Bit 0 (C)
        });
    }
    return inputs;
}

// Function to evaluate and compare two circuits
void analyzeCircuits(const string& circuit1, const string& circuit2) {
    auto inputs = generateInputs(); // Generate all combinations of A, B, C
    vector<bool> results1, results2; // Store results for each circuit

    // Evaluate and print truth table for Circuit 1
    cout << "\nTruth Table for Circuit 1:\n";
    cout << "A B C | Result\n";
    cout << "----------------\n";
    for (const auto& vars : inputs) {
        bool result = evaluateExpression(circuit1, vars[0], vars[1], vars[2]);
        results1.push_back(result); // Store result
        cout << vars[0] << " " << vars[1] << " " << vars[2] << " | " << result << endl;
    }

    // Evaluate and print truth table for Circuit 2
    cout << "\nTruth Table for Circuit 2:\n";
    cout << "A B C | Result\n";
    cout << "----------------\n";
    for (const auto& vars : inputs) {
        bool result = evaluateExpression(circuit2, vars[0], vars[1], vars[2]);
        results2.push_back(result); // Store result
        cout << vars[0] << " " << vars[1] << " " << vars[2] << " | " << result << endl;
    }

    // Compare results of the two circuits
    bool equivalent = results1 == results2;
    cout << "\nThe two circuits are " << (equivalent ? "equivalent." : "not equivalent.") << endl;

    // Analyze satisfiability and tautology for each circuit
    auto checkCircuit = [](const vector<bool>& results) -> string {
        bool satisfiable = false; // True if at least one input combination is true
        bool tautology = true;    // True if all input combinations are true
        for (bool res : results) {
            satisfiable |= res;
            tautology &= res;
        }
        if (!satisfiable) return "Unsatisfiable";
        if (tautology) return "Tautology";
        return "Satisfiable";
    };

    string status1 = checkCircuit(results1);
    string status2 = checkCircuit(results2);

    cout << "\nCircuit 1 is: " << status1 << endl;
    cout << "Circuit 2 is: " << status2 << endl;

    if (status1 == "Unsatisfiable" || status1 == "Tautology") {
        cout << "Modify Circuit 1 and run the program again " << endl;
    }
    if (status2 == "Unsatisfiable" || status2 == "Tautology") {
        cout << "Modify Circuit 2 and run the program again" << endl;
    }
}

int main() {

    cout << "Welcome to Task 2: Logic Circuit Equivalence Checker!" << endl;
    cout << "This program supports the following operators:" << endl;
    cout << " - AND: &" << endl;
    cout << " - OR: |" << endl;
    cout << " - NOT: !" << endl;
    cout << "Use only A , B , C as input *case sensitive*" << endl;
    cout << "Use parentheses () to specify the order of evaluation if needed." << endl;


    string infix1, infix2;
    cout << "Enter first circuit expression (infix, e.g., (A & B) | (!C)): ";
    getline(cin, infix1);
    // infix1 = "( (A & B) | (!C) ) & ( (A & B) | (!C) )"; // Sample circuit 1
    cout << "Enter second circuit expression (infix, e.g., (A & B) & (!C)): ";
    getline(cin, infix2);
    // infix2 = "( ( (A & B) | (!C) ) & ( (A & B) | (!C) ) ) | !( ( (A & B) | (!C) ) & ( (A & B) | (!C) ) )"; // Sample circuit 2

    // Convert infix expressions to postfix
    string circuit1 = infixToPostfix(infix1);
    string circuit2 = infixToPostfix(infix2);

    cout << "\nConverted Circuit 1 to Postfix: " << circuit1 << endl;
    cout << "Converted Circuit 2 to Postfix: " << circuit2 << endl;

    // Analyze and compare the circuits
    analyzeCircuits(circuit1, circuit2);

    return 0;
}
