#ifndef RICHALUK_DATABASE_H
#define RICHALUK_DATABASE_H

#include "Relation.h"
#include<map>
#include<string>
#include<iostream>
using namespace std;

class Database {

private:
    map <string, Relation> relations;

public:


    void AddRelation(Relation newRelation) {
        relations[newRelation.returnName()] = newRelation;
    }


    Relation GetRelationCopy(string name) {
        return relations.at(name);
    }

    void AddToRelation(Tuple newTuple, string name) {
        relations.at(name).addTuple(newTuple);
    }

    int NumberTuples() {
        int i = 0;
        for (const auto& pair : relations) {
            i = i + pair.second.numberTuples();
        }
        return i;
    }



};

#endif //RICHALUK_DATABASE_H
