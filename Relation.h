#ifndef RICHALUK_RELATION_H
#define RICHALUK_RELATION_H
#include "Tuple.h"
#include <set>

class Relation {

private:

    string name;
    Scheme scheme;
    set<Tuple> tuples;

public:

    Relation(const string& name, const Scheme& scheme)
            : name(name), scheme(scheme) { }
    Relation() {}

    bool addTuple(const Tuple& tuple) {
        return tuples.insert(tuple).second;
    }



    Relation join(const Relation& right) {
        const Relation& left = *this;
        Relation result;

        //join scheme
        Scheme leftScheme = left.getScheme();
        Scheme rightScheme = right.getScheme();

        vector<int> rightIndiciesDelete;
        Scheme resultScheme = leftScheme;

        for (int i = 0; i < leftScheme.size(); i++) {
            for(int j = 0; j < rightScheme.size(); j++) {
                if (leftScheme.at(i) == rightScheme.at(j)) {
                    rightIndiciesDelete.push_back(j);
                }
                else {
                    if (resultScheme.isAdded(rightScheme.at(j))) {
                        // do nothing
                    }
                    else {
                        resultScheme.pushback(rightScheme.at(j));
                    }
                }
            }
        }
        result.setScheme(resultScheme);


        // join the tuples

        for (const auto &leftTuple: tuples) {
            //cout << "left tuple: " << leftTuple.toString(left.scheme) << endl;

            for (const auto &rightTuple: right.tuples) {
                if (joinable(leftScheme, rightScheme, leftTuple, rightTuple)) {
                    Tuple resultTuple = joinTuples(leftTuple, rightTuple, rightIndiciesDelete);
                    result.addTuple(resultTuple);
                }
                //cout << "right tuple: " << rightTuple.toString(right.scheme) << endl;
            }
        }


        return result;
    }

    Tuple joinTuples(Tuple leftTuple, Tuple rightTuple, vector<int> rightIndiciesDelete) {
        Tuple resultTuple = leftTuple;
        for(int j = 0; j < rightTuple.getValues().size(); j++) {
            bool isEqual = false;
            for (int i = 0; i < rightIndiciesDelete.size()  ; i++) {
                if (j == rightIndiciesDelete.at(i)) {
                    isEqual = true;
                }
            }
            if (!isEqual) {
                resultTuple.add(rightTuple.getValues().at(j));
            }
        }
        return resultTuple;
    };



    static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme,
                         const Tuple& leftTuple, const Tuple& rightTuple) {
        bool sameName = false;

        for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
            const string& leftName = leftScheme.at(leftIndex);
            const string& leftValue = leftTuple.at(leftIndex);
            //cout << "left name: " << leftName << " value: " << leftValue << endl;

            for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
                const string& rightName = rightScheme.at(rightIndex);
                const string& rightValue = rightTuple.at(rightIndex);
                //cout << "right name: " << rightName << " value: " << rightValue << endl;

                if((leftName == rightName) && (leftTuple.at(leftIndex) != rightTuple.at(rightIndex))) {
                    return false;
                }
                if(leftName == rightName) {
                    sameName = true;
                }
            }
        }

//        if (sameName == false) {
//            return false;
//        }

        return true;
    }




    Relation unite(Relation databaseRelation) {
        Relation result = databaseRelation;
        for (auto const tuple: tuples) {
            if (result.addTuple(tuple)) {
                cout << tuple.toString(result.getScheme()) << endl;
            }
            else {
                // nothing
            }
        }
        return result;
    }




    string returnName() {
        return name;
    }

    void setScheme(Scheme newScheme) {
        scheme = newScheme;
    }

    Scheme getScheme() const {
        return scheme;
    }

    string toString() const {
        stringstream out;
        for (const auto &tuple: tuples) {
            out << tuple.toString(scheme);
            out << "\n";
        }
        // add code to print the Tuples, one per line
        return out.str();
    }

    Relation select1(int index, const string& value) const {
        Relation result(name, scheme);

        for (const auto &tuple : tuples) {
            if (tuple.at(index) == value) {
                result.addTuple(tuple);
            }
        }

        // add tuples to the result if they meet the condition
        return result;
    }

    Relation select2(int index1, int index2) {
        Relation result(name, scheme);
        for (const auto &tuple: tuples) {
            if (tuple.at(index1) == tuple.at(index2)) {
                result.addTuple(tuple);
            }
        }
        return result;
    }

    Relation project(vector<int> included) {
        Relation result(name,scheme);
        for (const auto &tuple: tuples) {
            vector<string> strings;
            for(int i : included) {
                strings.push_back(tuple.at(i));
            }
            Tuple newTuple(strings);
            result.addTuple(newTuple);
        }
        vector<string> schemeStrings;
        for(int i: included) {
            schemeStrings.push_back(scheme.at(i));
        }
        Scheme newScheme = Scheme(schemeStrings);
        result.setScheme(newScheme);
        return result;
    }

    Relation rename(vector<string> newColumnNames) {
        Scheme newScheme = Scheme(newColumnNames);
        Relation result(name, newScheme);
        for (const auto &tuple: tuples) {
            result.addTuple(tuple);
        }
        return result;
    }

    set<Tuple> getTuples() {
        return tuples;
    }

    int size() {
        return tuples.size();
    }

    int numberTuples() const {
        int i = 0;
        for (const auto& tuple: tuples) {
            i++;
        }
        return i;
    }

};

#endif //RICHALUK_RELATION_H
