#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>  // For system()

int main() {
    system("clear"); // Use "cls" for Windows

    std::ifstream inputFile("music.txt");
    if (!inputFile) {
        std::cerr << "Error: Could not open input.txt" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string content = buffer.str();
    inputFile.close();

    std::vector<std::vector<std::string>> listOfLists;
    std::vector<std::string> currentList;
    std::string token;
    bool inString = false;

    for (size_t i = 0; i < content.size(); ++i) {
        char c = content[i];

        if (c == '"' || c == '\'') {
            inString = !inString;
            continue;
        }

        if (inString) {
            token += c;
        } else if (std::isalnum(c) || c == '.' || c == '-' || c == '_') {
            token += c;
        } else {
            if (!token.empty()) {
                currentList.push_back(token);
                token.clear();
            }
            if (c == ']') {
                if (!currentList.empty()) {
                    listOfLists.push_back(currentList);
                    currentList.clear();
                }
            }
        }
    }

    // Print result
    std::cout << "Parsed list of lists:\n";
    for (const auto& row : listOfLists) {
        std::cout << "[";
        for (size_t i = 0; i < row.size(); ++i) {
            std::cout << '"' << row[i] << '"';
            if (i < row.size() - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }

    return 0;
}
