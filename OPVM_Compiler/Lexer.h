//
//  Lexer.h
//  OPVM_Compiler
//
//  Created by Seán Hargadon on 31/10/2014.
//  Copyright (c) 2014 Seán Hargadon. All rights reserved.
//

#ifndef __OPVM_Compiler__Lexer__
#define __OPVM_Compiler__Lexer__

#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

enum tokens {
    //values
    TOKEN_REG, TOKEN_ACCESS, TOKEN_LABEL, TOKEN_STRING, TOKEN_NUM,
    
    //statements
    TOKEN_MOVE, TOKEN_CMP, TOKEN_BRANCH, TOKEN_CALL, TOKEN_CONDITION,
    
    //ops
    TOKEN_ADD, TOKEN_MUL, TOKEN_SUB, TOKEN_DIV, TOKEN_AND, TOKEN_OR, TOKEN_NOT, TOKEN_XOR, TOKEN_LSHIFT, TOKEN_RSHIFT,
    
    //conditions
    TOKEN_AL, TOKEN_EQ, TOKEN_NE, TOKEN_GT, TOKEN_LT, TOKEN_GE, TOKEN_LE,
    
    //symbols
    TOKEN_COMMENT, TOKEN_LBRACK, TOKEN_RBRACK, TOKEN_LQUOTE, TOKEN_RQUOTE, TOKEN_COLON, TOKEN_COMMA, TOKEN_ARG,
    
    //keywords
    TOKEN_PROC, TOKEN_RET, TOKEN_PUSH, TOKEN_POP, TOKEN_EXTERN,
    
    TOKEN_EOF, TOKEN_NEWLINE, TOKEN_NONE, TOKEN_ERROR
};


struct Token {
    
    std::string str;
    int value;
    int offset;
    tokens tok;
    
    Token()
    {
        this->tok = TOKEN_NONE;
    }
    
    Token(tokens tok, std::string str)
    {
        this->tok = tok;
        this->str = str;
    }
    Token(tokens tok, int value)
    {
        this->tok = tok;
        this->value = value;
    }
    Token(tokens tok, std::string str, int reg)
    {
        this->tok = tok;
        this->value = reg;
        this->str = str;
    }
    Token(tokens tok, std::string str, int reg, int offset)
    {
        this->tok = tok;
        this->value = reg;
        this->str = str;
        this->offset = offset;
    }
};


class Lexer {
private:
    std::string match_newline;
    std::string match_whitespace;
    std::string match_reg;
    std::string match_access;
    std::string match_comment;
    std::string match_pipe;
    std::string match_cmp;
    std::string match_add;
    std::string match_sub;
    std::string match_mul;
    std::string match_div;
    std::string match_branch;
    std::string match_call;
    std::string match_string;
    std::string match_colon;
    std::string match_comma;
    std::string match_proc;
    std::string match_arg;
    std::string match_ret;
    std::string match_push;
    std::string match_pop;
    std::string match_extern;
    std::string match_AL;
    std::string match_EQ;
    std::string match_NE;
    std::string match_GT;
    std::string match_LT;
    std::string match_GE;
    std::string match_LE;
    std::string match_label;
    std::string match_binary;
    std::string match_hex;
    std::string match_dec;
    std::string match_error;


    std::vector<std::string> getStringTokens(std::string str);
    std::vector<Token> tokens;
    
    static std::vector<std::string> readFile(std::string file);
    void tokenise(std::vector<std::string> assembly);
    
    char getCurrentChar(int current, std::string str);
    
public:
    Lexer();
    Lexer(std::string file);
    std::vector<Token> getTokens();
    
};


#endif /* defined(__OPVM_Compiler__Lexer__) */
