#ifndef RICHALUK_INTERPRETER_H
#define RICHALUK_INTERPRETER_H
#include "Database.h"
#include "Datalogprogram.h"
#include "Graph.h"
#include<vector>

class Interpreter {

private:
    Database database;
    Datalogprogram datalogprogram;
    Graph graph;
    Graph reverseGraph;


public:
    Interpreter(Datalogprogram datalogprogram) : datalogprogram(datalogprogram) {}


    void makeGraph(vector<Rule> rules) {
        graph = Graph(rules.size());
        reverseGraph = Graph(rules.size());
        for (int i = 0; i < rules.size(); i++) {
//            cout << "from rule R" << i << " ";
//            rules.at(i).toString();
//            cout << endl;

            for(int j = 0; j < rules.at(i).getBody().size(); j++) {
//                cout << "from body predicate: ";
//                rules.at(i).getBody().at(j).toString();
//                cout << endl;

                for (int k = 0; k < rules.size(); k++) {
//                    cout << "to rule R" << k << " ";
//                    rules.at(k).toString();
//                    cout << endl;

                    if (rules.at(i).getBody().at(j).getName() == rules.at(k).getHead().getName()) {
                        graph.addEdge(i,k);
                        reverseGraph.addEdge(k,i);
                    }
                }
            }
        }
    }






    void InterpretSchemes() {
        for(Predicate predicate: datalogprogram.getSchemes()) {
            Relation emptyRelation(predicate.getName(), ConvertToScheme(predicate));
            database.AddRelation(emptyRelation);
        }
    }

    void InterpretFacts() {
        for(Predicate predicate: datalogprogram.getFacts()){
            Tuple newTuple = ConvertToTuple(predicate);
            database.AddToRelation(newTuple, predicate.getName());
        }
    }

    void InterpretRules() {
        vector<int> order;
        makeGraph(datalogprogram.getRules());
        cout << "Dependency Graph" << endl;
        graph.toString();
        cout << endl;
        cout << "Rule Evaluation" << endl;
        order = reverseGraph.dfsForest();
        vector<vector<int>> SCCs = graph.findSCCs(order);

        for(int i = 0; i < SCCs.size(); i++) {
            FixedPoint(SCCs.at(i));
        }
    }


    void FixedPoint(vector<int> SCC) {

        cout << "SCC: ";
        SCCString(SCC);
        cout << endl;

        int numberTuples = database.NumberTuples();
        RulesWithoutFixedPoint(SCC);
        int numberTuplesAfter = database.NumberTuples();
        int timesLooped = 1;

        bool selfLooping = false;
        if(SCC.size() == 1) {
            for(auto index: graph.getGraph()[SCC.at(0)].returnChildren()) {
                if (SCC.at(0) == index){
                    selfLooping = true;
                }
            }
        }

        if(SCC.size() != 1 || selfLooping) {
            while (numberTuples != numberTuplesAfter) {
                numberTuples = database.NumberTuples();
                RulesWithoutFixedPoint(SCC);
                numberTuplesAfter = database.NumberTuples();
                timesLooped++;
            }
        }

        cout << timesLooped << " passes: ";
        SCCString(SCC);
        cout << endl;
    }

    void RulesWithoutFixedPoint(vector<int> SCC) {
        vector<Rule> connectedRules;
        connectedRules.reserve(SCC.size());
        for(auto integer: SCC) {
            connectedRules.push_back(datalogprogram.getRules().at(integer));
        }

        for (Rule rule: connectedRules) {
            rule.toString();
            cout << "." << endl;
            vector<Relation> bodyRelations;
            for(Predicate bodyPredicate: rule.getBody()) {
                Relation result = evaluatePredicate(bodyPredicate);
                bodyRelations.push_back(result);
            }

            if(bodyRelations.size() == 1) {
                // do nothing
            }
            else {
                for(int i = 1; i < bodyRelations.size(); i++) {
                    bodyRelations.at(0) = bodyRelations.at(0).join(bodyRelations.at(i));
                }
            }

            Relation joinedBody = bodyRelations.at(0);

            // 1. get the strings 
            // 2. for each of those strings, loop through the scheme and save the location of the matching as an int in a vector

            vector<int> toProject;
            for (int i=0 ; i < rule.getHead().getParameters().size(); i++) {
                for(int j=0 ; j < joinedBody.getScheme().size(); j++) {
                    if (rule.getHead().getParameters().at(i).getParameter() == joinedBody.getScheme().at(j)) {
                        toProject.push_back(j);
                    }
                }
            }

            Relation projectedBody = joinedBody.project(toProject);

            string relationName = rule.getHead().getName();
            vector<string> newNames = database.GetRelationCopy(relationName).getScheme().getNames();
            Relation renamedRelation = projectedBody.rename(newNames);

            Relation copiedFromDatabase = database.GetRelationCopy(relationName);


            Relation updatedRelation = renamedRelation.unite(copiedFromDatabase);

            database.AddRelation(updatedRelation);
        }
    }

    void InterpretQueries() {
        cout << "\nQuery Evaluation" << endl;

        for (Predicate query: datalogprogram.getQueries()) {
            map<string, int> repeats;
            Relation result = database.GetRelationCopy(query.getName());

            for(unsigned int i = 0; i < query.getParameters().size(); i++) {
                if (query.getParameters().at(i).getType() == "STRING") {
                    result = result.select1(i, query.getParameters().at(i).getParameter());
                }
                if (query.getParameters().at(i).getType() == "ID") {

                    string keyToFind = query.getParameters().at(i).getParameter();
                    auto it = repeats.find(keyToFind);
                    if (it != repeats.end()) {
                        result = result.select2(it->second, i);
                    } else {
                        repeats[query.getParameters().at(i).getParameter()] = i;
                    }
                }
            }

            vector<int> columnsToKeep;
            vector<string> newHeaders;

            for (auto & repeat : repeats) {
                columnsToKeep.push_back(repeat.second);
            }

            sort(columnsToKeep.begin(), columnsToKeep.end());
            result = result.project(columnsToKeep);

            for(int i : columnsToKeep) {
                vector<Parameter> oldHeaders = query.getParameters();
                Parameter parameterToKeep = oldHeaders.at(i);
                string headerToKeep = parameterToKeep.toString();
                newHeaders.push_back(headerToKeep);
//                newHeaders.push_back(query.getParameters().at(columnsToKeep.at(i)).toString());
            }

            result = result.rename(newHeaders);

            query.toString();
            cout << "? ";
            if (result.size() == 0) {
                cout << "No";
            }
            else {
                cout << "Yes(" << result.size() << ")";
            }
            cout << endl;
            cout << result.toString();
        }
    }



    Relation evaluatePredicate(Predicate bodyPredicate) {
        map<string, int> repeats;
        Relation result = database.GetRelationCopy(bodyPredicate.getName());

        for(unsigned int i = 0; i < bodyPredicate.getParameters().size(); i++) {
            if (bodyPredicate.getParameters().at(i).getType() == "STRING") {
                result = result.select1(i, bodyPredicate.getParameters().at(i).getParameter());
            }
            if (bodyPredicate.getParameters().at(i).getType() == "ID") {

                string keyToFind = bodyPredicate.getParameters().at(i).getParameter();
                auto it = repeats.find(keyToFind);
                if (it != repeats.end()) {
                    result = result.select2(it->second, i);
                } else {
                    repeats[bodyPredicate.getParameters().at(i).getParameter()] = i;
                }
            }
        }

        vector<int> columnsToKeep;
        vector<string> newHeaders;

        for (auto & repeat : repeats) {
            columnsToKeep.push_back(repeat.second);
        }

        sort(columnsToKeep.begin(), columnsToKeep.end());
        result = result.project(columnsToKeep);

        for(int i : columnsToKeep) {
            vector<Parameter> oldHeaders = bodyPredicate.getParameters();
            Parameter parameterToKeep = oldHeaders.at(i);
            string headerToKeep = parameterToKeep.toString();
            newHeaders.push_back(headerToKeep);
        }

        result = result.rename(newHeaders);

        return result;
    }

    Scheme ConvertToScheme(Predicate predicate) {
        vector<string> strings;
        for (Parameter parameter: predicate.getParameters()) {
            strings.push_back(parameter.getParameter());
        }
        Scheme newScheme(strings);
        return newScheme;
    }

    Tuple ConvertToTuple(Predicate predicate) {
        vector<string> strings;
        for (Parameter parameter: predicate.getParameters()) {
            strings.push_back(parameter.getParameter());
        }
        Tuple newTuple(strings);
        return newTuple;
    }

    void SCCString(vector<int> component) {
        int count = 0;
        for (auto index: component) {
            cout  << "R" << index;
            if (++count < component.size()) {
                cout << ",";
            }
        }
    }


};

#endif //RICHALUK_INTERPRETER_H
