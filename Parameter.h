#ifndef RICHALUK_PARAMETER_H
#define RICHALUK_PARAMETER_H
#pragma once
#include "token.h"
#include <string>

class Parameter {
private:
    string parameter;
    string type;

public:

    Parameter() {
        parameter = "";
        type = "STRING";
    }

    string getParameter() {
        return parameter;
    }

    string getType() {
        return type;
    }

    void setType(string newType) {
        type = newType;
    }

    void setParameter(string newParameter) {
        parameter = newParameter;
    }

    string toString(){
        return parameter;
    }

};



#endif //RICHALUK_PARAMETER_H
