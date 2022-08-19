
#include <iostream>
#include "Parser.hh"

int main(int argc, char* argv[]) {
    if(argc >= 2) {
        Parser parser(argv[1]);
        parser.printTokens();
        parser.ParseExpression();
    } else {
        std::cout << "NHK" << std::endl;
        return -1;
    }
    return 0;
}
