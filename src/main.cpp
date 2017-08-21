#include <iostream>

void printArgs(int argc, char** argv) {
    for(int i = 0; i < argc; i++) {
        std::cout << "arg #" << i << " is " << argv[i] << std::endl;
    }
}


int main(int argc, char** argv) {
    printArgs(argc, argv);
    return 0;
}