#ifndef RICHALUK_DATALOGPROGRAM_H
#define RICHALUK_DATALOGPROGRAM_H
#pragma once
#include<vector>
#include<string>
#include<algorithm>
#include<iostream>
#include "predicate.h"
#include "Rule.h"
using namespace std;

class Datalogprogram {
private:
    vector<Predicate> schemes;
    vector<Predicate> facts;
    vector<Rule> rules;
    vector<Predicate> queries;
    vector<string> domain;


public:
    Datalogprogram(vector<Predicate> newSchemes, vector<Predicate> newFacts, vector<Rule> newRules, vector<Predicate> newQueries, vector<string> newDomain){
        schemes = newSchemes;
        facts = newFacts;
        rules = newRules;
        queries = newQueries;
        domain = newDomain;

        sort(domain.begin(), domain.end());
        auto it = std::unique(domain.begin(), domain.end());
        domain.erase(it, domain.end());


    }

    void addScheme(Predicate newScheme) {
        schemes.push_back(newScheme);
    }

    void addFact(Predicate newFact) {
        facts.push_back(newFact);
    }

    void addFact(Rule newRule) {
        rules.push_back(newRule);
    }

    void addQuery(Predicate newQuery) {
        queries.push_back(newQuery);
    }

    void printDatalog() {
        cout << "Schemes (" << schemes.size() << ") :\n";
        for (unsigned int i = 0; i < schemes.size(); ++i) {
            cout << "  ";
            schemes.at(i).toString();
            cout << endl;
        }

        cout << "Facts (" << facts.size() << ") :\n";
        for (unsigned int i = 0; i < facts.size(); ++i) {
            cout << "  ";
            facts.at(i).toString();
            cout << "." << endl;
        }

        cout << "Rules (" << rules.size() << ") :\n";
        for (unsigned int i = 0; i < rules.size(); ++i) {
            cout << "  ";
            rules.at(i).toString();
            cout << "." << endl;
        }

        cout << "Queries (" << queries.size() << ") :\n";
        for (unsigned int i = 0; i < queries.size(); ++i) {
            cout << "  ";
            queries.at(i).toString();
            cout  << "?" << endl;
        }

        cout << "Domain (" << domain.size() << ") :\n";
        for (unsigned int i = 0; i < domain.size(); ++i) {
            cout << domain.at(i) << endl;
        }
    }

    vector<Predicate> getSchemes() {
        return schemes;
    }

    vector<Predicate> getFacts() {
        return facts;
    }

    vector<Rule> getRules() {
        return rules;
    }

    vector<Predicate> getQueries() {
        return queries;
    }
};






#endif //RICHALUK_DATALOGPROGRAM_H
