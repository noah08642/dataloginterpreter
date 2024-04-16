#pragma once
#include <vector>
#include "parser.h"
#include "token.h"
#include "Parameter.h"
#include <string>
using namespace std;

class Predicate {
private:
  string name;
  vector<Parameter> parameters;

public:

    Predicate() {
        name = "";
        parameters = {};
    }

    Predicate(string newName) {
        name = newName;
        parameters = {};
    }

    string getName() {
        return name;
    }

    vector<Parameter> getParameters() {
        return parameters;
    }

    void setName(string newName) {
        name = newName;
    }

    void setParameters(vector<Parameter> newVector) {
        parameters = newVector;
    }


    void toString() {


        if (!parameters.empty()){
            cout << name << "(" << parameters.at(0).toString();
            for(unsigned int i = 1; i < parameters.size(); i++) {
                cout << "," << parameters.at(i).toString();
            }
        }
        else {
            cout << name << "(";
        }

        cout << ")";

    }




};



#ifndef RICHALUK_PREDICATE_H
#define RICHALUK_PREDICATE_H

#endif //RICHALUK_PREDICATE_H
