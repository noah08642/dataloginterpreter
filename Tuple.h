#ifndef RICHALUK_TUPLE_H
#define RICHALUK_TUPLE_H
#include <string>
#include <vector>
#include <sstream>
#include "Scheme.h"
using namespace std;

class Tuple {

private:

    vector<string> values;

public:
    Tuple(){}
    Tuple(vector<string> values) : values(values) { }

    unsigned size() const {
        return values.size();
    }

    const string& at(int index) const {
        return values.at(index);
    }

    bool operator<(const Tuple t) const {
        return values < t.values;
    }

    string toString(const Scheme& scheme) const {
        const Tuple& tuple = *this;
        stringstream out;
        for (unsigned int i = 0; i < scheme.size(); ++i) {
            out << scheme.at(i);
            out << '=';
            out << tuple.at(i);
            if (i < scheme.size() - 1) {
                out << ", ";
            }
        }
        return out.str();
    }

    void remove(int index) {
        values.erase(values.begin() + index);
    }

    void add(string newValue) {
        values.push_back(newValue);
    }

    vector<string> getValues() {
        return values;
    }

    // TODO: add more delegation functions as needed

};

#endif //RICHALUK_TUPLE_H
