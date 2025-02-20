#include <iostream> 
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <algorithm>

using namespace std;

#define DEBUG_MODE false  // Set to true for debugging output

void analyzeCode(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file." << endl;
        return;
    }
    
    string line;
    int lineNumber = 0;

    // Regex patterns
    regex semicolonCheck(R"(^\s*(?!#|for|while|if|else|switch|return|do|case|default|namespace|class|struct|void|int|char|float|double|bool|template)[a-zA-Z0-9_]+.*[^;{}]\s*$)");
    regex indentationCheck(R"(^\s{5,})");  // Checks if line starts with 5+ spaces
    regex deepNestingCheck(R"((?:\{[^{}]*\{){3,})");  // Detects nesting beyond 2 levels
    regex longFunctionCheck(R"((?:\{[^{}]*\}){16,})");  // Detects functions longer than 15 lines
    regex varDeclaration(R"(\b(int|float|double|char|bool)\s+([a-zA-Z_][a-zA-Z0-9_]*(?:\s*,\s*[a-zA-Z_][a-zA-Z0-9_]*)*)\s*;)");

    unordered_map<string, int> declaredVars; // Stores variable name and line number
    unordered_set<string> usedVars;

    cout << "{\n  \"issues\": [\n";

    bool firstIssue = true;
    
    while (getline(file, line)) {
        lineNumber++;
        smatch match;

        // Detect missing semicolon
        if (regex_match(line, semicolonCheck)) {
            if (!firstIssue) cout << ",\n";
            cout << "    {\"line\": " << lineNumber << ", \"message\": \"Possible missing semicolon.\"}";
            firstIssue = false;
        }

        // Detect variable declarations
        if (regex_search(line, match, varDeclaration)) {
            string vars = match[2]; // Extracts "x, y, z"
            stringstream ss(vars);
            string varName;
            
            while (getline(ss, varName, ',')) {
                varName.erase(remove_if(varName.begin(), varName.end(), ::isspace), varName.end());  // Trim spaces
                declaredVars[varName] = lineNumber; // Store line number of declaration
                if (DEBUG_MODE) cout << "[DEBUG] Declared: " << varName << " at line " << lineNumber << endl;
            }
        }

        // Detect variable usage
        for (const auto& var : declaredVars) {
            if (line.find(var.first) != string::npos) {
                usedVars.insert(var.first);
                if (DEBUG_MODE) cout << "[DEBUG] Used: " << var.first << " at line " << lineNumber << endl;
            }
        }

        // Detect indentation issues
        if (regex_match(line, indentationCheck)) {
            if (!firstIssue) cout << ",\n";
            cout << "    {\"line\": " << lineNumber << ", \"message\": \"Poor indentation detected.\"}";
            firstIssue = false;
        }
    }

    // Report unused variables
    for (const auto& var : declaredVars) {
        if (usedVars.find(var.first) == usedVars.end()) {  // If variable was never used
            if (!firstIssue) cout << ",\n";
            cout << "    {\"line\": " << var.second << ", \"message\": \"Potential unused variable: " << var.first << "\"}";
            firstIssue = false;
        }
    }

    cout << "\n  ]\n}" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    
    analyzeCode(argv[1]);
    return 0;
}

/*   g++ -std=c++17 cpp_analyzer.cpp -o analyzer
./analyzer test.cpp
*/