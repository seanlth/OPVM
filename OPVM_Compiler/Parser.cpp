//
//  Parser.cpp
//  OPVM_Compiler
//
//  Created by Seán Hargadon on 31/10/2014.
//  Copyright (c) 2014 Seán Hargadon. All rights reserved.
//

#include "Parser.h"


Parser::Parser()
{

    this->currentToken = 0;
}

Parser::Parser(std::string input, std::string output)
{
    this->keywords = {"auto", "break", "case", "char" "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while, ret" };
    
    
    Lexer lex = Lexer(input);
    this->tokens = lex.getTokens();
    this->file = output;
    
    f.open(this->file);
    
    if (!f.is_open()) {
        std::cout << "Error opening file" << std::endl;
        exit(-1);
    }
    
    parse();
    
    for (int i = 0; i < this->includes.size(); i++) {
        writeInstruction("#include <" + this->includes[i] + ".h>");
    }
    
    writeInstruction("int _C = 0;");
    writeInstruction("unsigned long long R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14 = 0;");
    writeInstruction("unsigned long long R15 = 3;");
    writeInstruction("unsigned long long MEM[1000];\n");
    
    for (int i = 0; i < this->instructions.size(); i++) {
        writeInstruction(this->instructions[i]);
    }
    
    f.close();
}

Token Parser::getCurrentToken(int currentToken)
{
    Token tok;
    if (currentToken >= 0 && currentToken < this->tokens.size()) {
        tok = this->tokens[currentToken];
    }
    else if ( currentToken >= this->tokens.size() ) {
        return Token(TOKEN_EOF, "");
    }
    
    return tok;
}

bool Parser::isExtern(std::string label)
{
    for (int i = 0; i < this->externs.size(); i++) {
        if ( label.compare(this->externs[i]) == 0) {
            return true;
        }
    }
    return false;
}


std::string Parser::instructionMoveToString(Instruction instruction)
{
    std::string instructionString = "";
    
    switch (instruction.dest.type) {
        case OPERAND_REG:
            instructionString += "R" + std::to_string(instruction.dest.value) + " = ";
            break;
        case OPERAND_ACCESS:
            instructionString += "MEM[ R" + std::to_string(instruction.dest.value) + " + " + std::to_string(instruction.dest.offset) + " ] = ";
            break;
        default:
            break;
    }
    
    switch (instruction.src1.type) {
        case OPERAND_REG:
            instructionString += "R" + std::to_string(instruction.src1.value);
            break;
        case OPERAND_ACCESS:
            instructionString += "MEM[ R" + std::to_string(instruction.src1.value) + " + " + std::to_string(instruction.src1.offset) + " ]";
            break;
        case OPERAND_NUM:
            instructionString += std::to_string(instruction.src1.value);
            break;
        case OPERAND_LABEL:
            instructionString += instruction.src1.str;
        default:
            break;
    }
    if ( instruction.op.tok != TOKEN_NONE ) {
        
        instructionString += " " + instruction.op.str + " ";
        
        switch (instruction.src2.type) {
            case OPERAND_REG:
                instructionString += "R" + std::to_string(instruction.src2.value);
                break;
            case OPERAND_ACCESS:
                instructionString += "MEM[ R" + std::to_string(instruction.src2.value) + " + " + std::to_string(instruction.src2.offset) + " ]";
                break;
            case OPERAND_NUM:
                instructionString += std::to_string(instruction.src2.value) + "";
                break;
            case OPERAND_LABEL:
                instructionString += instruction.src1.str;
            default:
                break;
        }
    }
    return instructionString;
}

std::string Parser::instructionCompareToString(Instruction instruction)
{
    std::string instructionString = "_C = ";
    
    switch (instruction.dest.type) {
        case OPERAND_REG:
            instructionString += "R" + std::to_string(instruction.dest.value);
            break;
        case OPERAND_ACCESS:
            instructionString += "MEM[ R" + std::to_string(instruction.dest.value) + " + " + std::to_string(instruction.dest.offset) + " ]";
            break;
        default:
            break;
    }

    
    instructionString += " - ";
    
    
    switch (instruction.src1.type) {
        case OPERAND_REG:
            instructionString += "R" + std::to_string(instruction.src1.value);
            break;
        case OPERAND_ACCESS:
            instructionString += "MEM[ R" + std::to_string(instruction.src1.value) + " + " + std::to_string(instruction.src1.offset) + " ]";
            break;
        case OPERAND_NUM:
            instructionString += std::to_string(instruction.src1.value) + "";
            break;
        default:
            break;
    }
    
    return instructionString;
}

std::string Parser::instructionLabelToString(Instruction instruction)
{
    std::string instructionString = "";
    
    instructionString += instruction.label + ":; ";
    if (instruction.dest.type == OPERAND_STRING) {
        instructionString += "char* " + instruction.label + "Val = " + instruction.dest.str + ";";
    }
    else if (instruction.dest.type == OPERAND_NUM) {
        instructionString += "int " + instruction.label + "Val = " + std::to_string( instruction.dest.value ) + ";";
    }
    
    return instructionString;
}

std::string Parser::instructionPushToString(Instruction instruction)
{
    std::string instructionString = "";
    
    for (int i = 0; i < instruction.list.size(); i++) {
        instructionString += "MEM[ R15++ ] = ";
        
        switch (instruction.list[i].type) {
            case OPERAND_REG:
                instructionString += "R" + std::to_string(instruction.list[i].value) + ";";
                break;
            case OPERAND_ACCESS:
                instructionString += "MEM[ R" + std::to_string(instruction.list[i].value) + " + " + std::to_string(instruction.list[i].offset) + " ];";
                break;
            case OPERAND_NUM:
                instructionString += std::to_string(instruction.list[i].value) + ";";
                break;
            case OPERAND_STRING:
                instructionString += "(unsigned long long)" + instruction.list[i].str + ";";
                break;

            default:
                break;
        }
        if ( i < instruction.list.size() - 1 ) {
            instructionString += "\n";
        }
    }
    return instructionString;
}

std::string Parser::instructionPopToString(Instruction instruction)
{
    std::string instructionString = "";
    
    for (int i = 0; instruction.list.size(); i++) {
        instructionString += "MEM[ R15++ ] = ";
        
        switch (instruction.list[i].type) {
            case OPERAND_REG:
                instructionString += "R" + std::to_string(instruction.dest.value) + ";";
                break;
            case OPERAND_ACCESS:
                instructionString += "MEM[ R" + std::to_string(instruction.dest.value) + " + " + std::to_string(instruction.dest.offset) + " ];";
                break;
            case OPERAND_NUM:
                instructionString += std::to_string(instruction.src1.value);
                break;
                
            default:
                break;
        }
        
    }
    return instructionString;
}


void Parser::writeInstruction(std::string instruction)
{
    f << instruction << std::endl;
}

void Parser::writeInstruction(Instruction instruction)
{
    bool no_semi_colon = false;
    std::string ins = "";
    
    if ( instruction.type == INSTRUCTION_MOVE ) {
        ins += instructionMoveToString(instruction);
    }
    else if ( instruction.type == INSTRUCTION_COMPARE ) {
        ins += instructionCompareToString(instruction);
    }
    else if ( instruction.type == INSTRUCTION_LABEL ) {
        ins += instructionLabelToString(instruction);
    }
    else if ( instruction.type == INSTRUCTION_PROC ) {
        if (instruction.label.compare("main") == 0) {
            ins += "int " + instruction.label + "(int argc, char* argv[]) {";
        }
        else {
            ins += "int " + instruction.label + "() {";
        }
        no_semi_colon = true;
    }
    else if ( instruction.type == INSTRUCTION_RET ) {
        ins += "int __obp = MEM[R15 - 1];\n";
        ins += "R15 = R14;\n";
        ins += "R14 = __obp;\n";
        ins += "return 0";
        if ( instruction.condition.tok == TOKEN_NONE ) {
            ins += ";\n}\n";
            no_semi_colon = true;
        }
    }
    else if ( instruction.type == INSTRUCTION_CALL ) {
        bool xtern = isExtern(instruction.label);
        
        std::string s = "";
        
        if (!xtern) {
            writeInstruction("MEM[ R15++ ] = R14;");
            writeInstruction("R14 = R15 - " + std::to_string( instruction.src1.value ) + " - 1;" );
        }
        if (xtern) {
            if (instruction.src1.value >= 1) {
                s += "R15 = R15 - " + std::to_string( instruction.src1.value ) + ";\n";
            }
        }
        
        s += instruction.label + "(";
        if (xtern) {
            
            for (int i = 0; i < instruction.src1.value; i++) {
                s += "MEM[ R15 + " + std::to_string( i ) + " ]";
                if (i < instruction.src1.value-1) {
                    s += ", ";
                }
            }
        }
        s += ")";
        
        ins = s;
    }
    else if ( instruction.type == INSTRUCTION_BRANCH ) {
        ins = "goto " + instruction.label;
    }
    else if ( instruction.type == INSTRUCTION_PUSH ) {
        no_semi_colon = true;
        ins = instructionPushToString(instruction);
    }
    else if ( instruction.type == INSTRUCTION_POP ) {
        no_semi_colon = true;
        ins = instructionPopToString(instruction);
    }
    
    if (instruction.condition.tok == TOKEN_NONE) {
        if (no_semi_colon) {
            writeInstruction(ins);
        }
        else {
            writeInstruction(ins + ";");
        }
    }
    else {
        std::string conditionString = "if (_C " + instruction.condition.str + " 0) { " + ins + "; }";
        writeInstruction(conditionString);
    }
}

void Parser::parse()
{
    parseStatements();
}

void Parser::parseStatements()
{
    parseStatement();
    parseStatementList();
}

void Parser::parseStatementList()
{
    if (getCurrentToken(this->currentToken).tok == TOKEN_NEWLINE) {
        this->currentToken++;
        parseStatements();
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_EOF ) {
        return;
    }
    else {
        std::cout << "Expected eof, found " << getCurrentToken(this->currentToken).str << std::endl;
        exit(1);
    }
}

void Parser::parseStatement()
{
    if ( getCurrentToken(this->currentToken).tok == TOKEN_COMMENT ) {
        currentToken++;
        return;
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_REG || getCurrentToken(this->currentToken).tok == TOKEN_ACCESS) {
        parsePrimary();
        parseOperation();
        this->instructions.push_back(this->currentInstruction);
        this->currentInstruction = Instruction();
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_CALL ) {
        currentToken++;
        
        parseCall();
        
        currentToken++;
        
        this->instructions.push_back(this->currentInstruction);
        this->currentInstruction = Instruction();
    }

    else if ( getCurrentToken(this->currentToken).tok == TOKEN_BRANCH ) {
        currentToken++;
       
        parseBranch();
        
        currentToken++;
        
        this->instructions.push_back(this->currentInstruction);
        this->currentInstruction = Instruction();
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_LABEL ) {
        this->currentInstruction.type = INSTRUCTION_LABEL;
        
        parseLabel();
        parseLabelValue();
        this->instructions.push_back(this->currentInstruction);
        this->currentInstruction = Instruction();
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_PROC ) {
        this->currentInstruction.type = INSTRUCTION_PROC;
        
        this->currentToken++;
        
        parseLabel();
        
        this->instructions.push_back(this->currentInstruction);
        this->currentInstruction = Instruction();
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_RET ) {
        this->currentInstruction.type = INSTRUCTION_RET;
        
        this->currentToken++;
        parseCondition();
        
        this->instructions.push_back(this->currentInstruction);
        this->currentInstruction = Instruction();

    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_PUSH ) {
        this->currentInstruction.type = INSTRUCTION_PUSH;
        
        this->currentToken++;
        parsePush();
        
        this->instructions.push_back(this->currentInstruction);
        this->currentInstruction = Instruction();
        
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_POP ) {
        this->currentInstruction.type = INSTRUCTION_POP;
        
        this->currentToken++;
        parsePop();
        
        this->instructions.push_back(this->currentInstruction);
        this->currentInstruction = Instruction();
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_EXTERN ) {
        this->currentInstruction.type = INSTRUCTION_EXTERN;
        
        this->currentToken++;
        parseExtern();
        
        this->includes.push_back( this->currentInstruction.label );
        this->externs.push_back( this->currentInstruction.src1.str );
        
        this->currentInstruction = Instruction();
    }

    else if ( getCurrentToken(this->currentToken).tok == TOKEN_NEWLINE ) {
        this->currentInstruction = Instruction();
        parseStatementList();
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_EOF ) {
        return;
    }
    else {
        std::cout << "Expected statement, found " << getCurrentToken(this->currentToken).str << std::endl;
        exit(1);
    }
}

void Parser::parsePrimary()
{
    if ( getCurrentToken(this->currentToken).tok == TOKEN_REG ) {
        this->currentInstruction.dest.value = getCurrentToken(this->currentToken).value;
        this->currentInstruction.dest.type = OPERAND_REG;
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_ACCESS ) {
        this->currentInstruction.dest.offset = getCurrentToken(this->currentToken).offset;
        this->currentInstruction.dest.value = getCurrentToken(this->currentToken).value;
        this->currentInstruction.dest.type = OPERAND_ACCESS;
    }
    else {
        std::cout << "Expected register or access, found " << getCurrentToken(this->currentToken).str << std::endl;
        exit(1);
    }
    this->currentToken++;
}

void Parser::parseListPrimary()
{
    if ( getCurrentToken(this->currentToken).tok == TOKEN_REG ) {
        Operand t;
        t.value = getCurrentToken(this->currentToken).value;
        t.type = OPERAND_REG;
        this->currentInstruction.list.push_back(t);
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_ACCESS ) {
        Operand t;
        t.offset = getCurrentToken(this->currentToken).offset;
        t.value = getCurrentToken(this->currentToken).value;
        t.type = OPERAND_ACCESS;
        this->currentInstruction.list.push_back(t);
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_NUM ) {
        Operand t;
        t.value = getCurrentToken(this->currentToken).value;
        t.type = OPERAND_NUM;
        this->currentInstruction.list.push_back(t);
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_STRING ) {
        Operand t;
        t.str = getCurrentToken(this->currentToken).str;
        t.type = OPERAND_STRING;
        this->currentInstruction.list.push_back(t);
    }
    else {
        std::cout << "Expected register, access, number or string, found " << getCurrentToken(this->currentToken).str << std::endl;
        exit(1);
    }
    this->currentToken++;
}

void Parser::parseOperation()
{
    if ( getCurrentToken(this->currentToken).tok == TOKEN_CMP ) {
        this->currentToken++;
        this->currentInstruction.type = INSTRUCTION_COMPARE;
        parseLeftPrimary();
        parseCondition();
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_MOVE ) {
        this->currentToken++;
        this->currentInstruction.type = INSTRUCTION_MOVE;
        parseLeftPrimary();
        parseOptional();
        parseCondition();
    }
    else {
        std::cout << "Expected '<-' or '==', found " << getCurrentToken(this->currentToken).str << std::endl;
        exit(1);
    }
}

void Parser::parseLeftPrimary()
{
    if ( getCurrentToken(this->currentToken).tok == TOKEN_REG ) {
        if ( this->currentInstruction.src1.type == OPERAND_NONE ) {
            this->currentInstruction.src1.value = getCurrentToken(this->currentToken).value;
            this->currentInstruction.src1.type = OPERAND_REG;
        }
        else {
            this->currentInstruction.src2.value = getCurrentToken(this->currentToken).value;
            this->currentInstruction.src2.type = OPERAND_REG;
        }
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_ACCESS ) {
        if ( this->currentInstruction.src1.type == OPERAND_NONE ) {
            this->currentInstruction.src1.offset = getCurrentToken(this->currentToken).offset;
            this->currentInstruction.src1.value = getCurrentToken(this->currentToken).value;
            this->currentInstruction.src1.type = OPERAND_ACCESS;
        }
        else {
            this->currentInstruction.src2.offset = getCurrentToken(this->currentToken).offset;
            this->currentInstruction.src2.value = getCurrentToken(this->currentToken).value;
            this->currentInstruction.src2.type = OPERAND_ACCESS;
        }
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_NUM ) {
        if ( this->currentInstruction.src1.type == OPERAND_NONE ) {
            this->currentInstruction.src1.value = getCurrentToken(this->currentToken).value;
            this->currentInstruction.src1.type = OPERAND_NUM;
        }
        else {
            this->currentInstruction.src2.value = getCurrentToken(this->currentToken).value;
            this->currentInstruction.src2.type = OPERAND_NUM;
        }
    }
    else if ( getCurrentToken(this->currentToken).tok == TOKEN_LABEL ) {
        if ( this->currentInstruction.src1.type == OPERAND_NONE ) {
            this->currentInstruction.src1.str = getCurrentToken(this->currentToken).str + "Val";
            this->currentInstruction.src1.type = OPERAND_LABEL;
        }
        else {
            this->currentInstruction.src2.str = getCurrentToken(this->currentToken).str + "Val";
            this->currentInstruction.src2.type = OPERAND_LABEL;
        }
    }
    else {
        std::cout << "Expected register, access, number or label, found " << getCurrentToken(this->currentToken).str << std::endl;
        exit(1);
    }
    this->currentToken++;
}

void Parser::parseCondition()
{
    if ( getCurrentToken(this->currentToken).tok == TOKEN_COLON ) {
        currentToken++;
        this->currentInstruction.condition = getCurrentToken(this->currentToken);
        currentToken++;
    }
}

void Parser::parseOptional()
{
    if ( getCurrentToken(this->currentToken).tok == TOKEN_ADD || getCurrentToken(this->currentToken).tok == TOKEN_SUB || getCurrentToken(this->currentToken).tok == TOKEN_MUL || getCurrentToken(this->currentToken).tok == TOKEN_DIV || getCurrentToken(this->currentToken).tok == TOKEN_AND || getCurrentToken(this->currentToken).tok == TOKEN_OR || getCurrentToken(this->currentToken).tok == TOKEN_NOT || getCurrentToken(this->currentToken).tok == TOKEN_XOR || getCurrentToken(this->currentToken).tok == TOKEN_LSHIFT || getCurrentToken(this->currentToken).tok == TOKEN_RSHIFT) {
        
        this->currentInstruction.op = getCurrentToken(this->currentToken);
        
        this->currentToken++;
        parseLeftPrimary();
    }
}

void Parser::parseLabelValue()
{
    if ( getCurrentToken(this->currentToken).tok == TOKEN_COLON ) {
        this->currentToken++;
        if ( getCurrentToken(this->currentToken).tok == TOKEN_NUM ) {
            this->currentInstruction.dest.value = getCurrentToken(this->currentToken).value;
            this->currentInstruction.dest.type = OPERAND_NUM;
            this->currentToken++;
        }
        else if ( getCurrentToken(this->currentToken).tok == TOKEN_STRING ) {
            this->currentInstruction.dest.str = getCurrentToken(this->currentToken).str;
            this->currentInstruction.dest.type = OPERAND_STRING;
            this->currentToken++;
        }
    }
    else {
        std::cout << "Expected ':', found " << getCurrentToken(this->currentToken).str << std::endl;
        exit(1);
    }
}

void Parser::parseLabel()
{
    if (this->getCurrentToken(this->currentToken).tok == TOKEN_LABEL) {
        if (this->currentInstruction.label.compare("") == 0) {
            this->currentInstruction.label = getCurrentToken(this->currentToken).str;
        }
        else {
            this->currentInstruction.src1.str = getCurrentToken(this->currentToken).str;
        }
        
        for (int i = 0; i < this->keywords.size(); i++) {
            if ( this->currentInstruction.label.compare(this->keywords[i]) == 0) {
                this->currentInstruction.label += "_";
            }
        }
    }
    else {
        std::cout << "Expected label, found " << getCurrentToken(this->currentToken).str << std::endl;
        exit(1);
    }
    this->currentToken++;
}

void Parser::parseCall()
{
    parseLabel();
    
    this->currentInstruction.dest.type = OPERAND_NONE;
    this->currentInstruction.src1.type = OPERAND_NONE;
    this->currentInstruction.src2.type = OPERAND_NONE;
    
    this->currentInstruction.type = INSTRUCTION_CALL;
    
    if (getCurrentToken(this->currentToken).tok == TOKEN_COMMA) {
        this->currentToken++;
    }
    else {
        std::cout << "Expected comma, found " << getCurrentToken(this->currentToken).str << std::endl;
        exit(1);
    }
    
    
    if ( getCurrentToken(this->currentToken).tok == TOKEN_NUM ) {
        this->currentInstruction.src1.value = getCurrentToken(this->currentToken).value;
        this->currentInstruction.src1.type = OPERAND_NUM;
    }
    else {
        std::cout << "Expected number, found " << getCurrentToken(this->currentToken).str << std::endl;
        exit(1);
    }
    
    parseCondition();
}

void Parser::parseBranch()
{
    parseLabel();
    
    this->currentInstruction.dest.type = OPERAND_NONE;
    this->currentInstruction.src1.type = OPERAND_NONE;
    this->currentInstruction.src2.type = OPERAND_NONE;
    
    this->currentInstruction.type = INSTRUCTION_BRANCH;
    
    parseCondition();
}



void Parser::parsePush()
{
    parseListPrimary();
    parseList();
}

void Parser::parsePop()
{
    parseListPrimary();
    parseList();
}

void Parser::parseList()
{
    if (this->getCurrentToken(this->currentToken).tok == TOKEN_COMMA) {
        this->currentToken++;
        parseListPrimary();
        parseList();
    }
}

void Parser::parseExtern()
{
    parseLabel();
    if ( this->getCurrentToken(this->currentToken).tok == TOKEN_DIV ) {
        this->currentToken++;
        parseLabel();
    }
    else {
        std::cout << "Expected '/', found " << getCurrentToken(this->currentToken).str << std::endl;
        exit(1);
    }
}






