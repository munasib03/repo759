#include <iostream>
#include <string>
#include <cstdio>

int main (int argc, char* argv[]) {
    
    int n = std::stoi(argv[1]);

    for (int i = 0; i <= n; i++) {
        printf("%d", i);

        if (i != n) {
            printf(" ");
        }
    }

    printf("\n");

    for (int i = n; i >= 0; i--) {
        std::cout << i;

        if (i != 0) {
            std::cout << " ";
        }
    }

    std::cout << "\n";

    return 0;
}
