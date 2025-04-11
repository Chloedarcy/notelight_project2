#include <iostream>
#include <fstream>
#include <cstdlib>  // For system()

int main() {
    // Clear the screen before every print
    system("clear"); // Use "cls" for Windows instead of "clear"
    
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Error: Could not open input.txt" << std::endl;
        return 1;
    }

    std::string line;
    while (getline(inputFile, line)) {
        std::cout << line << std::endl;
    }
    inputFile.close();
    
    return 0;
}

