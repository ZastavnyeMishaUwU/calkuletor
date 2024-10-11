#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <sstream>

using namespace std;

const double PI = 3.14159265358979323846;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    case '^': return pow(a, b);
    }
    return 0;
}

double evaluate(const string& tokens) {
    stack<double> values;
    stack<char> ops;

    for (size_t i = 0; i < tokens.length(); i++) {
        if (isspace(tokens[i])) continue;

        if (isdigit(tokens[i]) || tokens[i] == '.') {
            double val = 0;
            double decimalPlace = 1;
            bool isDecimal = false;
            while (i < tokens.length() && (isdigit(tokens[i]) || tokens[i] == '.')) {
                if (tokens[i] == '.') {
                    isDecimal = true;
                }
                else {
                    if (isDecimal) {
                        decimalPlace *= 10;
                        val = val + (tokens[i] - '0') / decimalPlace;
                    }
                    else {
                        val = val * 10 + (tokens[i] - '0');
                    }
                }
                i++;
            }
            values.push(val);
            i--;
        }
        else if (tokens[i] == '(') {
            ops.push(tokens[i]);
        }
        else if (tokens[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(val1, val2, op));
            }
            ops.pop();
        }
        else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(tokens[i])) {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(val1, val2, op));
            }
            ops.push(tokens[i]);
        }
    }

    while (!ops.empty()) {
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperation(val1, val2, op));
    }

    return values.top();
}

double evaluateFunction(const string& func, double value) {

    if (func == "sin") return sin(value * PI / 180);
    if (func == "cos") return cos(value * PI / 180);
    if (func == "tan") return tan(value * PI / 180);
    return 0;
}

int main() {
    setlocale(LC_CTYPE, "ukr");
    string expression;
    cout << "Введіть вираз (використовуйте ^ для степеня, sqrt() для кореня, sin(), cos(), tan() для тригонометричних функцій): ";
    getline(cin, expression);


    size_t pos = 0;
    while ((pos = expression.find("sqrt(", pos)) != string::npos) {
        size_t endPos = expression.find(')', pos);
        if (endPos != string::npos) {
            string inner = expression.substr(pos + 5, endPos - pos - 5);
            double value = evaluate(inner);
            expression.replace(pos, endPos - pos + 1, to_string(sqrt(value)));
        }
    }


    string funcs[] = { "sin", "cos", "tan" };
    for (const string& func : funcs) {
        pos = 0;
        while ((pos = expression.find(func + "(", pos)) != string::npos) {
            size_t endPos = expression.find(')', pos);
            if (endPos != string::npos) {
                string inner = expression.substr(pos + func.length() + 1, endPos - pos - func.length() - 1);
                double value = evaluate(inner);
                expression.replace(pos, endPos - pos + 1, to_string(evaluateFunction(func, value)));
            }
        }
    }

    double result = evaluate(expression);
    cout << "Результат: " << result << endl;

    return 0;
}
