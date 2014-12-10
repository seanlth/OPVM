//
//  Lexer.cpp
//  OPVM_Compiler
//
//  Created by Seán Hargadon on 31/10/2014.
//  Copyright (c) 2014 Seán Hargadon. All rights reserved.
//

#include "Lexer.h"
#include <regex>

Lexer::Lexer() :
match_newline("\\n"),
match_whitespace("\\s+"),
match_reg("R[1-9][0-9]|R[0-9]"),
match_access ("\\[R[1-9][0-9]\\]|\\[R[0-9]\\]|\\[[a-zA-Z]+\\]"),
match_comment ("\\/\\/.*"),
match_pipe ("\\<\\-"),
match_cmp ("=="),
match_add ("\\+"),
match_sub ("\\-"),
match_mul ("\\*"),
match_div ("\\/"),
match_call ("call"),
match_branch ("B"),
match_string ("\"[a-zA-Z]+\""),
match_colon (":"),
match_comma (","),
match_proc ("@"),
match_push ("push"),
match_pop ("pop"),
match_arg ("&[1-9][0-9]+"),
match_ret ("ret"),
match_AL("AL"),
match_EQ("EQ"),
match_NE("NE"),
match_GT("GT"),
match_LT("LT"),
match_GE("GE"),
match_LE("LE"),
match_label ("[a-zA-Z]+"),
match_binary ("0[b][01]+"),
match_hex ("0[x][0-9a-fA-F]+"),
match_dec ("[0-9]+"),
match_error (".+")
{
    
}

Lexer::Lexer(std::string file) :
match_newline("\\n"),
match_whitespace("\\s+"),
match_reg("R[1-9][0-9]|R[0-9]"),
match_access ("\\[R[1-9][0-9]\\]|\\[R[0-9]\\]|\\[[a-zA-Z]+\\]"),
match_comment ("\\/\\/.*"),
match_pipe ("\\<\\-"),
match_cmp ("=="),
match_add ("\\+"),
match_sub ("\\-"),
match_mul ("\\*"),
match_div ("\\/"),
match_call ("call"),
match_branch ("B"),
match_string ("\"[a-zA-Z]+\""),
match_colon (":"),
match_comma (","),
match_proc ("@"),
match_push ("push"),
match_pop ("pop"),
match_arg ("&[0-9]+"),
match_ret ("ret"),
match_AL("AL"),
match_EQ("EQ"),
match_NE("NE"),
match_GT("GT"),
match_LT("LT"),
match_GE("GE"),
match_LE("LE"),
match_label ("[a-zA-Z]+"),
match_binary ("0[b][01]+"),
match_hex ("0[x][0-9a-fA-F]+"),
match_dec ("[0-9]+"),
match_error (".+")
{
    tokenise( this->readFile(file) );
}

std::vector<std::string> Lexer::readFile(std::string file)
{
    std::vector<std::string> result;
    
    std::ifstream f(file);
    std::string output;
    if (f.is_open()) {
        while (std::getline(f, output))
        {
            result.push_back(output);
        }
    }
    else {
        std::cout << "Error opening file" << std::endl;
        exit(-1);
    }
    f.close();
    
    return result;
}

std::vector<Token> Lexer::getTokens()
{
    return this->tokens;
}

std::vector<std::string> Lexer::getStringTokens(std::string str)
{
    std::vector<std::string> toks;
    
    std::string reg = match_newline + "|" + match_whitespace+ "|" +  match_reg + "|" + match_access + "|" + match_comment + "|" + match_cmp + "|" + match_pipe + "|" + match_add + "|" + match_sub + "|" + match_mul + "|" + match_div + "|" + match_call + match_branch + "|" + match_string + "|" + match_comma + "|" + match_colon + "|" + match_proc + "|" + match_arg + "|" + match_ret + "|" + match_push + "|" + match_pop + "|" + match_AL + "|" + match_EQ + "|" + match_NE + "|" + match_GT + "|" + match_LT + "|" + match_GE + "|" + match_LE + "|" + match_label + "|" + match_binary + "|" + match_hex + "|" + match_dec + "|" + match_error;
    
    //std::regex r("\\n|\\s+|\\/\\/.*|\\[R[1-9][0-9]\\]|R[1-9][0-9]|\\[R[0-9]\\]|R[0-9]|\\<\\-|\\+|\\*|\\/|AL|EQ|NE|GT|LT|GE|LE|B|\"[a-zA-Z]+\"|:|[a-zA-Z]+|0[b][01]+|0[x][0-9a-fA-F]+|[0-9]+|.+");
    
    std::regex r(reg);
    
    std::smatch m;
    
    regex_search(str, m, r);
    
    while (std::regex_search (str, m, r)) {
        for (auto x:m) if (x.str()[0] != ' ') toks.push_back(x);
        str = m.suffix().str();
    }
    toks.push_back("\n");
    
    return toks;
}


void Lexer::tokenise(std::vector<std::string> assembly)
{
    std::vector<Token> toks;

    for (int i = 0; i < assembly.size(); i++) {
        std::string instruction = assembly[i];
        
        auto s = getStringTokens(instruction);
        
        for (auto x:s) {
            Token tok;
            if (std::regex_match(x, std::regex(match_reg))) {
                std::string reg(x.begin()+1, x.end());
                tok = Token(TOKEN_REG, x, atoi(reg.c_str()));
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_comment)) ) {
                tok = Token(TOKEN_COMMENT, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_newline))) {
                tok = Token(TOKEN_NEWLINE, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_access))) {
                std::string reg(x.begin()+2, x.end()-1);
                tok = Token(TOKEN_ACCESS, x, atoi(reg.c_str()));
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_cmp))) {
                tok = Token(TOKEN_CMP, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_pipe))) {
                tok = Token(TOKEN_MOVE, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_add))) {
                tok = Token(TOKEN_ADD, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_sub))) {
                tok = Token(TOKEN_SUB, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_mul))) {
                tok = Token(TOKEN_MUL, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_div))) {
                tok = Token(TOKEN_DIV, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_call))) {
                tok = Token(TOKEN_CALL, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_branch))) {
                tok = Token(TOKEN_BRANCH, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_comma))) {
                tok = Token(TOKEN_COMMA, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_colon))) {
                tok = Token(TOKEN_COLON, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_proc))) {
                tok = Token(TOKEN_PROC, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_arg))) {
                std::string offset(x.begin()+1, x.end());
                tok = Token(TOKEN_ACCESS, "[R15]", 15, atoi(offset.c_str()));
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_ret))) {
                tok = Token(TOKEN_RET, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_push))) {
                tok = Token(TOKEN_PUSH, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_pop))) {
                tok = Token(TOKEN_POP, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_AL))) {
                tok = Token(TOKEN_AL, "true");
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_EQ))) {
                tok = Token(TOKEN_EQ, "==");
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_NE))) {
                tok = Token(TOKEN_NE, "!=");
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_GT))) {
                tok = Token(TOKEN_GT, ">");
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_LT))) {
                tok = Token(TOKEN_LT, "<");
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_GE))) {
                tok = Token(TOKEN_GE, ">=");
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_LE))) {
                tok = Token(TOKEN_LE, "<=");
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_string))) {
                tok = Token(TOKEN_STRING, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_label))) {
                tok = Token(TOKEN_LABEL, x);
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_binary))) {
                tok = Token(TOKEN_NUM, (int)strtoull(x.c_str(), NULL, 2) );
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_hex))) {
                tok = Token(TOKEN_NUM, (int)strtoull(x.c_str(), NULL, 16));
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_dec))) {
                tok = Token(TOKEN_NUM, atoi(x.c_str()));
                toks.push_back(tok);
            }
            else if (std::regex_match(x, std::regex(match_error))) {
                tok = Token(TOKEN_ERROR, x);
                toks.push_back(tok);
            }
            
        }
    }
    this->tokens.push_back(Token(TOKEN_EOF, "EOF"));
    this->tokens = toks;
}

