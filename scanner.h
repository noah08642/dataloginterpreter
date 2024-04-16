#pragma once
#include "token.h"
#include <string>
#include <cctype>
#include <vector>
using namespace std;

class Scanner {
private:
    string input;
    int line = 1;
    int extraLines = 0;
    string value;
    int numberTokens = 0;
    bool ended = false;
    vector<Token> Tokens;

public:
    Scanner(const string& input) : input(input) { }


# scans the next token, identifying its type, value, and line number, then deletes the token from the input
    Token scanToken() {
        TokenType type;

        line = line + extraLines;
        extraLines = 0;


        if (!input.empty()) {
            while (isspace(input.at(0))) {
                if (input.at(0) == '\n') {
                    line++;
                }
                input = input.substr(1);
                if(input.empty()) {
                    break;
                }
            }

            if(input.empty()) {
                type = EOFILE;
                value = "";
                ended = true;
                return Token(type, value, line);
            }
            else {
                if (input.at(0) == ',') {
                    type = COMMA;
                    value = ",";
                    input = input.substr(1);
                } else if (input.at(0) == '.') {
                    type = PERIOD;
                    value = ".";
                    input = input.substr(1);
                } else if (input.at(0) == '?') {
                    type = Q_MARK;
                    value = "?";
                    input = input.substr(1);
                } else if (input.at(0) == '(') {
                    type = LEFT_PAREN;
                    value = "(";
                    input = input.substr(1);
                } else if (input.at(0) == ')') {
                    type = RIGHT_PAREN;
                    value = ")";
                    input = input.substr(1);
                } else if (input.at(0) == ':') {
                    if (input.at(1) == '-') {
                        type = COLON_DASH;
                        value = ":-";
                        input = input.substr(2);
                    } else {
                        type = COLON;
                        value = ":";
                        input = input.substr(1);
                    }
                } else if (input.at(0) == '*') {
                    type = MULTIPLY;
                    value = "*";
                    input = input.substr(1);
                } else if (input.at(0) == '+') {
                    type = ADD;
                    value = "+";
                    input = input.substr(1);
                } else if (input.substr(0, 8) == "Schemes\n" || input.substr(0,8) == "Schemes " || input.substr(0,8) == "Schemes:") {
                    type = SCHEMES;
                    value = "Schemes";
                    input = input.substr(7);
                } else if (input.substr(0,6) == "Facts " || input.substr(0,6) == "Facts:" || input.substr(0,6) == "Facts\n") {
                    type = FACTS;
                    value = "Facts";
                    input = input.substr(5);
                } else if (input.substr(0, 6) == "Rules\n" || input.substr(0,6) == "Rules " || input.substr(0,6) == "Rules:") {
                    type = RULES;
                    value = "Rules";
                    input = input.substr(5);
                } else if (input.substr(0, 8) == "Queries\n" || input.substr(0,8) == "Queries " || input.substr(0,8) == "Queries:") {
                    type = QUERIES;
                    value = "Queries";
                    input = input.substr(7);
                } else if (isalpha(input.at(0))) {
                    string word = "";
                    while ((!input.empty()) && (isalpha(input.at(0)) || isdigit(input.at(0)))) {
                        word = word + input.at(0);
                        input = input.substr(1);
                    }
                    type = ID;
                    value = word;
                } else if (input.at(0) == '\'') {
                    input = input.substr(1);
                    string word = "";
                    while (input.at(0) != '\'') {
                        if(input.at(0) == '\n') {
                            extraLines++;
                        }
                        if(input.length() == 1) {
                            if(input.at(0) == '\'') {
                                //keep going?
                            }
                            else {
                                type = UNDEFINED;
                                value = '\'' + word + input.at(0);
                                input = input.substr(1);
                                return Token(type, value, line);
                            }
                        }
                        else {
                            word = word + input.at(0);
                            input = input.substr(1);
                        }
                    }
                    input = input.substr(1);
                    type = STRING;
                    value = "\'" + word + "\'";
                } else if (input.at(0) == '#') {
                    input = input.substr(1);
                    string word = "#";
                    while (input.at(0) != '\n' ) {
                        word = word + input.at(0);
                        input = input.substr(1);
                    }
                    type = COMMENT;
                    value = word;
                } else if (!input.empty()) {
                    type = UNDEFINED;
                    value = input.at(0);
                    input = input.substr(1);
                }
            }
            return Token(type, value, line);
        }

        else {
            type = EOFILE;
            value = "";
            ended = true;
            return Token(type, value, line);
        }
    }



    vector<Token> scanAllTokens() {
        while(!input.empty()) {
            Token t = scanToken();
            if (t.getType() == COMMENT) {
                // don't do anything
            }
            else {
                Tokens.push_back(t);
                numberTokens = numberTokens + 1;
            }
        }
        if (!ended) {
            Token t = scanToken();
            //cout << t.toString() << endl;
            Tokens.push_back(t);
            ++numberTokens;
        }
//        cout << "Total Tokens = " << numberTokens << endl;
        return Tokens;
    }

};


#ifndef RICHALUK_SCANNER_H
#define RICHALUK_SCANNER_H

#endif //RICHALUK_SCANNER_H
