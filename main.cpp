#include <iostream>
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "predicate.h"
#include "Parameter.h"
#include "Datalogprogram.h"
#include <fstream>
#include "Scheme.h"
#include "Tuple.h"
#include "Relation.h"
#include "Interpreter.h"
#include "Node.h"
#include "Graph.h"
using namespace std;

int main(int argc, char* argv[]) {

    ifstream in;
    in.open(argv[1]);
    stringstream ss;
    ss << in.rdbuf();
    string input = ss.str();
    in.close();

    Scanner s = Scanner(input);
    vector<Token> tokens = s.scanAllTokens();
    Parser p = Parser(tokens);
    Datalogprogram program = p.parse();
    Interpreter i = Interpreter(program);

    i.InterpretSchemes();
    i.InterpretFacts();
    i.InterpretRules();
    i.InterpretQueries();
}

//int main() {
//
//    // predicate names for fake rules
//    // first is name for head predicate
//    // second is names for body predicates
//    pair<string,vector<string>> ruleNames[] = {
//            { "A", { "B", "C" } },
//            { "B", { "A", "D" } },
//            { "B", { "B" } },
//            { "E", { "F", "G" } },
//            { "E", { "E", "F" } },
//    };
//
//    vector<Rule> rules;
//
//    for (auto& rulePair : ruleNames) {
//        string headName = rulePair.first;
//        Rule rule = Rule(Predicate(headName));
//        vector<string> bodyNames = rulePair.second;
//        for (auto& bodyName : bodyNames)
//            rule.addBodyPredicate(Predicate(bodyName));
//        rules.push_back(rule);
//    }
//
//    Graph graph = Interpreter::makeGraph(rules);
//    graph.toString();
//
//}
