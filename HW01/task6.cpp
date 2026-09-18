#include <iostream>
#include <string>

int main (int argc, char* argv[]) {
    
    int n = std::stoi(argv[1]);

    for (int i = 0; i <= n; i++) {
        std::cout << i;

        if (i != n) {
            std::cout << " ";
        }
    }

    std::cout << "\n";

    for (int i = n; i >= 0; i--) {
        std::cout << i;

        if (i != 0) {
            std::cout << " ";
        }
    }

    std::cout << "\n";

    return 0;
}
