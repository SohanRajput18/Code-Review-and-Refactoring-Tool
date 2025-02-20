#include <iostream>
#include <fstream>
using namespace std;

void longFunction() {  
    int x, y, z;  // Unused variables
    for (int i = 0; i < 10; i++) {  
        if (i % 2 == 0) {  
            if (i % 3 == 0) {  
                if (i % 5 == 0) {  
cout << "Deeply nested code" << endl  // Missing semicolon
                }  
            }  
        }  
    }  
}

int main() {  
    int a   // Missing semicolon
    cout << "Hello, World!" << endl;  

    longFunction();  

    return 0  
}  