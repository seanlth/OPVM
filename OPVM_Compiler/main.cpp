//
//  main.cpp
//  OPVM_Compiler
//
//  Created by Seán Hargadon on 16/05/2014.
//  Copyright (c) 2014 Seán Hargadon. All rights reserved.
//

#include <iostream>
#include "Parser.h"
#include <regex>
#include <string.h>


int main(int argc, const char * argv[])
{
    std::string src = "";
    std::string dest = "";
    
    std::string reg = "[a-zA-F0-9]+\\.opvm";
    
    bool compile = true;
    
    if (argc == 1) {
        printf("\n");
        printf("Usage: opvm-as [-S] src [-o dest] \n");
        printf("\n");
        exit(1);
    }
    else if (strcmp(argv[1], "-S") == 0) {
        compile = false;
        if ( argc >= 3 ) {
            src = argv[2];
            if (argc == 5 && strcmp(argv[3], "-o") == 0) {
                dest = argv[4];
            }
            else {
                dest = "a.out";
            }
        }
        else {
            printf("\n");
            printf("Usage: opvm-as [-S] src [-o dest] \n");
            printf("\n");
            exit(1);
        }
    }
    else if (std::regex_match(argv[1], std::regex(reg))) {
        if ( argc >= 2 ) {
            src = argv[1];
            if (argc == 4 && strcmp(argv[2], "-o") == 0) {
                dest = argv[3];
            }
            else {
                dest = "a.out";
            }
        }
        else {
            printf("\n");
            printf("Usage: opvm-as [-S] src [-o dest] \n");
            printf("\n");
            exit(1);
        }

    }
    else {
        printf("\n");
        printf("Usage: opvm-as [-S] src [-o dest] \n");
        printf("\n");
        exit(1);
    }
    
    std::string oldDest = dest;

    Parser parser = Parser(src, dest);

    if (compile) {
        dest += ".c";
        std::string command = "clang " + dest + " -o " + oldDest + " 2> /dev/null";
        
        std::cout << command << std::endl;
        
        system(command.c_str());
    }
    
    //Parser parser = Parser("/Users/Seanlth/Projects/OPVM_Compiler/OPVM_Compiler/code1.opvm", "/Users/Seanlth/Projects/OPVM_Compiler/code");
    
    return 0;
}

