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
