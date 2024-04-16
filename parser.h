#pragma once
#include <vector>
#include "token.h"
#include "predicate.h"
#include "Parameter.h"
#include "Datalogprogram.h"
#include "Rule.h"

class Parser {
private:
    vector<Token> tokens;
    vector<string> domain;

public:
    Parser(const vector<Token>& tokens) : tokens(tokens) {}

    TokenType tokenType() const {
        return tokens.at(0).getType();
    }

    void advanceToken() {
        tokens.erase(tokens.begin());
    }

    void throwError() {
        throw tokens.at(0);
    }

    void match(TokenType t) {
        if (tokens.at(0).getType() == t) {
            advanceToken();
        }
        else {
            throwError();
        }
    }

    Datalogprogram parse() {
        try {
            Datalogprogram d = datalogProgram();
            //cout << "Success!" << endl;
            //d.printDatalog();
            return d;
        }
        catch (Token error) {
            cout << "Failure!" << endl;
            cout << error.toString() << endl;
        }
    }

# parses the structure of the datalog input.  Creates the vectors of schemes, facts, rules, and queries.  Stores the vectors as a datalogprogram object.
    Datalogprogram datalogProgram() {
        match(SCHEMES);
        match(COLON);
        Predicate newScheme = scheme();
        vector<Predicate> newSchemeList = schemeList();
        newSchemeList.insert(newSchemeList.begin(), newScheme);
        if(newSchemeList.at(0).getName() == "") {
            throwError();
        }


        match(FACTS);
        match(COLON);
        vector<Predicate> newFactList = factList();

        match(RULES);
        match(COLON);
        vector<Rule> newRuleList = ruleList();

        match(QUERIES);
        match(COLON);
        Predicate newQuery = query();
        vector<Predicate> newQueryList = queryList();
        newQueryList.insert(newQueryList.begin(), newQuery);
        if(newQueryList.at(0).getName() == "") {
            throwError();
        }

        match(EOFILE);

        Datalogprogram d = Datalogprogram(newSchemeList, newFactList, newRuleList, newQueryList, domain);
        return d;
    }

    vector<Predicate> schemeList() {
        vector<Predicate> newSchemeList;
        if(tokenType() == ID) {
            Predicate newScheme = scheme();
            newSchemeList.insert(newSchemeList.begin(), newScheme);
            vector<Predicate> nextSchemeList = schemeList();
            newSchemeList.insert(newSchemeList.end(), nextSchemeList.begin(), nextSchemeList.end());
        }
        else {
            //epsilon
        }
        return newSchemeList;
    }

    vector<Predicate> factList() {
        vector<Predicate> newFactList;
        if(tokenType() == ID) {
            Predicate newFact = fact();
            newFactList.push_back(newFact);
            vector<Predicate> nextFactList = factList();
            newFactList.insert(newFactList.end(), nextFactList.begin(), nextFactList.end());
        }
        else {
            //epsilon
        }
        return newFactList;
    }

    vector<Rule> ruleList() {
        vector<Rule> newRuleList;
        if(tokenType() == ID) {
            Rule newRule = rule();
            newRuleList.push_back(newRule);
            vector<Rule> nextRuleList = ruleList();
            newRuleList.insert(newRuleList.end(), nextRuleList.begin(), nextRuleList.end());
        }
        else {
            //epsilon
        }
        return newRuleList;
    }

    vector<Predicate> queryList() {
        vector<Predicate> newQueryList;
        if(tokenType() == ID) {
            Predicate newQuery = query();
            newQueryList.insert(newQueryList.begin(), newQuery);
            vector<Predicate> nextQueryList = queryList();
            newQueryList.insert(newQueryList.end(), nextQueryList.begin(), nextQueryList.end());
        }
        else {
            //epsilon
        }
        return newQueryList;
    }

    Predicate scheme() {
        Predicate newPredicate;
        if(tokenType() == ID) {
            newPredicate.setName(tokens.at(0).getValue());
            match(ID);
            match(LEFT_PAREN);

            Parameter newParameter;
            newParameter.setParameter(tokens.at(0).getValue());
            newParameter.setType(tokens.at(0).getTypeString());
            match(ID);

            vector<Parameter> newIdList;
            newIdList.push_back(newParameter);

            vector<Parameter> nextIdList = idList();
            newIdList.insert(newIdList.end(), nextIdList.begin(), nextIdList.end());
            newPredicate.setParameters(newIdList);
            match(RIGHT_PAREN);
        }
        else {
            //epsilon
        }
        return newPredicate;
    }

    Predicate fact() {
        Predicate newFact;
        newFact.setName(tokens.at(0).getValue());
        match(ID);
        match(LEFT_PAREN);
        Parameter firstParameter;
        firstParameter.setParameter(tokens.at(0).getValue());
        firstParameter.setType(tokens.at(0).getTypeString());
        domain.push_back(tokens.at(0).getValue());
        match(STRING);
        vector<Parameter> newStringList = stringList();
        newStringList.insert(newStringList.begin(), firstParameter);
        newFact.setParameters(newStringList);
        match(RIGHT_PAREN);
        match(PERIOD);

        return newFact;
    }

    Rule rule() {
        Rule newRule;
        Predicate newHead = headPredicate();
        match(COLON_DASH);

        Predicate firstBody = predicate();
        vector<Predicate> otherBodies = predicateList();
        otherBodies.insert(otherBodies.begin(), firstBody);  // might be .end() !

        newRule.setBody(otherBodies);
        newRule.setHead(newHead);
        match(PERIOD);

        return newRule;
    };

    Predicate query() {
        Predicate newPredicate = predicate();
        match(Q_MARK);
        return newPredicate;
    }

    Predicate headPredicate() {
        Predicate newHead;
        newHead.setName(tokens.at(0).getValue());
        match(ID);
        match(LEFT_PAREN);
        Parameter firstID;
        firstID.setType("ID");
        firstID.setParameter(tokens.at(0).getValue());
        match(ID);

        vector<Parameter> newIds = idList();
        newIds.insert(newIds.begin(), firstID);
        newHead.setParameters(newIds);
        match(RIGHT_PAREN);

        return newHead;
    }

    Predicate predicate() {
        string name = tokens.at(0).getValue();
        match(ID);
        match(LEFT_PAREN);
        Parameter newParameter = parameter();
        vector<Parameter> newParameterList= parameterList();
        newParameterList.insert(newParameterList.begin(), newParameter); // maybe change this to .end() ?
        match(RIGHT_PAREN);

        Predicate newPredicate;
        newPredicate.setName(name);
        newPredicate.setParameters(newParameterList);
        return newPredicate;
    }

    vector<Predicate> predicateList() {
        vector<Predicate> newPredicateList;
        if(tokenType() == COMMA) {
            match(COMMA);
            Predicate newPredicate = predicate();
            newPredicateList.push_back(newPredicate);

            vector<Predicate> nextPredicateList = predicateList();
            newPredicateList.insert(newPredicateList.end(), nextPredicateList.begin(), nextPredicateList.end()); // maybe change to .begin() ?

        }
        else {
            //epsilon
        }
        return newPredicateList;
    }

    vector<Parameter> parameterList(){
        vector<Parameter> newParameterList;
        if(tokenType() == COMMA) {
            match(COMMA);
            Parameter newParameter = parameter();
//            newParameter.setParameter(tokens.at(0).getValue());
//            newParameter.setType(tokens.at(0).getTypeString());
            newParameterList.push_back(newParameter);

            vector<Parameter> nextParameterList = parameterList();
            newParameterList.insert(newParameterList.end(), nextParameterList.begin(), nextParameterList.end());
        }
        else {
            // epsilon
        }
        return newParameterList;
    }

    vector<Parameter> stringList() {
        vector<Parameter> newStringList;
        if(tokenType() == COMMA) {
            match(COMMA);
            Parameter newParameter;
            newParameter.setParameter(tokens.at(0).getValue());
            newParameter.setType(tokens.at(0).getTypeString());
            newStringList.push_back(newParameter);
            domain.push_back(newParameter.getParameter());
            match(STRING);

            vector<Parameter> nextStringList = stringList();
            newStringList.insert(newStringList.end(), nextStringList.begin(), nextStringList.end());
        }
        else {
            //epsilon
        }
        return newStringList;
    }

    vector<Parameter> idList() {
        vector<Parameter> newList;
        if(tokenType() == COMMA) {
            match(COMMA);

            Parameter newParameter;
            newParameter.setParameter(tokens.at(0).getValue());
            newParameter.setType(tokens.at(0).getTypeString());
            newList.push_back(newParameter);
            match(ID);
            vector<Parameter> nextList = idList();
            newList.insert(newList.end(), nextList.begin(), nextList.end());
        }
        else {
            // epsilon
        }
        return newList;
    }

    Parameter parameter() {
        Parameter newParameter;
        string newString = tokens.at(0).getValue();
        newParameter.setParameter(newString);
        if(tokenType() == STRING) {
            match(STRING);
        }
        else if(tokenType() == ID)  {
            newParameter.setType("ID");
            match(ID);
        }



        return newParameter;
    }


};















//
// Created by richaluk on 2/6/24.
//

#ifndef RICHALUK_PARSER_H
#define RICHALUK_PARSER_H

#endif //RICHALUK_PARSER_H
