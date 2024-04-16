#ifndef RICHALUK_RULE_H
#define RICHALUK_RULE_H
#pragma once
#include "predicate.h"

class Rule {
private:
    Predicate headPredicate;
    vector<Predicate> bodyPredicates;

public:
    Rule(){}

    Rule(Predicate newHead) {
        headPredicate = newHead;
    }

    void addBodyPredicate(Predicate bodyPredicate) {
        bodyPredicates.push_back(bodyPredicate);
    }

    void setHead(Predicate newHead) {
        headPredicate = newHead;
    }

    void setBody(vector<Predicate> newBody) {
        bodyPredicates = newBody;
    }

    void toString() {
        headPredicate.toString();
        cout << " :- ";
        bodyPredicates.at(0).toString();
        for (unsigned int i = 1; i < bodyPredicates.size(); ++i) {
            cout << ",";
            bodyPredicates.at(i).toString();
        }
    }
    Predicate getHead() {
        return headPredicate;
    }

    vector<Predicate> getBody() {
        return bodyPredicates;
    }

};

#endif //RICHALUK_RULE_H
