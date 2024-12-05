#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <map>
#include <stack>
#include <cctype>
#include <functional>

using namespace std;

// Function to convert infix to postfix
string infixToPostfix(const string& infix) {
    map<char, int> precedence = {
        {'|', 1},  // OR
        {'&', 2},  // AND
        {'!', 3}   // NOT
    };
    stack<char> operators;
    string postfix;

    for (char c : infix) {
        if (isspace(c)) continue; // Skip whitespace
        if (isalnum(c)) {         // Operand (variable)
            postfix += c;
        } else if (c == '(') {    // Left parenthesis
            operators.push(c);
        } else if (c == ')') {    // Right parenthesis
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            operators.pop(); // Pop the '('
        } else { // Operator
            while (!operators.empty() && precedence[operators.top()] >= precedence[c]) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(c);
        }
    }

    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}

// Helper function to evaluate a logical expression
bool evaluateExpression(string expr, const map<char, bool>& vars) {
    vector<bool> stack;
    for (char c : expr) {
        if (c == 'A' || c == 'B' || c == 'C') { // Variables
            stack.push_back(vars.at(c));
        } else if (c == '!') { // NOT
            bool val = stack.back();
            stack.pop_back();
            stack.push_back(!val);
        } else if (c == '&') { // AND
            bool val1 = stack.back();
            stack.pop_back();
            bool val2 = stack.back();
            stack.pop_back();
            stack.push_back(val1 && val2);
        } else if (c == '|') { // OR
            bool val1 = stack.back();
            stack.pop_back();
            bool val2 = stack.back();
            stack.pop_back();
            stack.push_back(val1 || val2);
        }
    }
    return stack.back();
}

// Helper function to generate all input combinations for 3 variables
vector<map<char, bool>> generateInputs() {
    vector<map<char, bool>> inputs;
    for (int i = 0; i < 8; ++i) {
        bitset<3> binary(i); // 3 inputs
        map<char, bool> inputSet = {
            {'A', binary[2]},
            {'B', binary[1]},
            {'C', binary[0]}
        };
        inputs.push_back(inputSet);
    }
    return inputs;
}

// Main function to evaluate circuits
void analyzeCircuits(string circuit1, string circuit2) {
    auto inputs = generateInputs();
    vector<bool> results1, results2;

    // Evaluate and print truth table for Circuit 1
    cout << "\nTruth Table for Circuit 1: " << circuit1 << endl;
    cout << "A B C | Result\n";
    cout << "----------------\n";
    for (auto& vars : inputs) {
        for (auto& var : vars) {
            cout << var.second << " ";
        }
        bool result = evaluateExpression(circuit1, vars);
        results1.push_back(result);
        cout << "| " << result << endl;
    }

    // Evaluate and print truth table for Circuit 2
    cout << "\nTruth Table for Circuit 2: " << circuit2 << endl;
    cout << "A B C | Result\n";
    cout << "----------------\n";
    for (auto& vars : inputs) {
        for (auto& var : vars) {
            cout << var.second << " ";
        }
        bool result = evaluateExpression(circuit2, vars);
        results2.push_back(result);
        cout << "| " << result << endl;
    }

    // Check equivalence
    bool equivalent = results1 == results2;
    cout << "\nThe two circuits are " << (equivalent ? "equivalent." : "not equivalent.") << endl;

    // Check satisfiability and tautology
    auto checkCircuit = [](vector<bool>& results) -> string {
        bool satisfiable = false;
        bool tautology = true;
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

    // Modify unsatisfiable or tautological circuits
    auto modifyCircuit = [](string circuit) -> string {
        for (char& c : circuit) {
            if (c == '&') { c = '|'; break; }
            else if (c == '|') { c = '&'; break; }
        }
        return circuit;
    };

    if (status1 == "Unsatisfiable" || status1 == "Tautology") {
        circuit1 = modifyCircuit(circuit1);
        cout << "Modified Circuit 1 to: " << circuit1 << endl;
    }
    if (status2 == "Unsatisfiable" || status2 == "Tautology") {
        circuit2 = modifyCircuit(circuit2);
        cout << "Modified Circuit 2 to: " << circuit2 << endl;
    }
}

int main() {
    string infix1, infix2;
    cout << "Enter first circuit expression (infix, e.g., (A & B) | (!C)): ";
    // getline(cin, infix1);
    infix1 = "( (A & B) | (!C) ) & ( (A & B) | (!C) )"  ;
    cout << "Enter second circuit expression (infix, e.g., (A & B) | (!C)): ";
    // getline(cin, infix2);
    infix2 = "(A & B) | (!C)" ;

    string circuit1 = infixToPostfix(infix1);
    string circuit2 = infixToPostfix(infix2);

    cout << "\nConverted Circuit 1 "<< infix1<<" to Postfix: " << circuit1 << endl;
    cout << "Converted Circuit 2 "<< infix2 <<" to Postfix: " << circuit2 << endl;

    analyzeCircuits(circuit1, circuit2);

    return 0;
}
