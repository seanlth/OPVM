//
//  Parser.h
//  OPVM_Compiler
//
//  Created by Seán Hargadon on 31/10/2014.
//  Copyright (c) 2014 Seán Hargadon. All rights reserved.
//

#ifndef __OPVM_Compiler__Parser__
#define __OPVM_Compiler__Parser__

#include <stdio.h>
#include "Lexer.h"
#include <vector>


enum InstructionType {
    INSTRUCTION_BRANCH, INSTRUCTION_CALL, INSTRUCTION_RET, INSTRUCTION_PROC, INSTRUCTION_COMPARE, INSTRUCTION_MOVE, INSTRUCTION_LABEL, INSTRUCTION_PUSH, INSTRUCTION_POP, INSTRUCTION_EXTERN
};

enum OperandType {
    OPERAND_REG, OPERAND_ACCESS, OPERAND_NUM, OPERAND_STRING, OPERAND_LABEL, OPERAND_NONE
};

struct Operand {
    int value;
    int offset;
    std::string str;
    OperandType type;
    
    Operand() { this->type = OPERAND_NONE; this->str = ""; }
    
};


struct Instruction {
    Operand dest;
    Operand src1;
    Operand src2;
    
    std::vector<Operand> list;
    
    std::string label;
    
    InstructionType type;
    Token op;
    Token condition;
    
    Instruction() { this->op = Token(); this->condition = Token(); }
};

class Parser {
private:
    std::vector<std::string> includes;
    std::vector<std::string> externs;
    std::vector<std::string> keywords;
    std::vector<Instruction> instructions;
    Instruction currentInstruction;

    std::vector<Token> tokens;
    int currentToken;
    
    std::ofstream f;
    std::string file;
    
    void parseStatements();
    void parseStatementList();
    void parseStatement();
    void parsePrimary();
    void parseLeftPrimary();
    void parseListPrimary();
    void parseOperation();
    void parseCondition();
    void parseOptional();
    void parseLabelValue();
    void parseLabel();
    void parseCall();
    void parseBranch();
    void parsePush();
    void parsePop();
    void parseList();
    void parseExtern();



    
    
    Token getCurrentToken(int currentToken);
    void writeInstruction(std::string instruction);
    void writeInstruction(Instruction instruction);
    bool isExtern(std::string label);
    
    
    std::string instructionMoveToString(Instruction instruction);
    std::string instructionCompareToString(Instruction instruction);
    std::string instructionLabelToString(Instruction instruction);
    std::string instructionPushToString(Instruction instruction);
    std::string instructionPopToString(Instruction instruction);

public:
    Parser();
    Parser(std::string input, std::string output);

    
    void parse();
    
};

#endif /* defined(__OPVM_Compiler__Parser__) */
