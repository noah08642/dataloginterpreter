#ifndef RICHALUK_SCHEME_H
#define RICHALUK_SCHEME_H
#include <vector>
#include <string>
using namespace std;

class Scheme {

private:

    vector<string> names;

public:

    Scheme(vector<string> names) : names(names) { }
    Scheme(){}

    unsigned size() const {
        return names.size();
    }

    const string& at(int index) const {
        return names.at(index);
    }

    void pushback(string newName) {
        names.push_back(newName);
    }

    vector<string> getNames() {
        return names;
    }

    bool isAdded(string name) {
        bool i = false;
        for (int j = 0; j < names.size(); j++) {
            if (names.at(j) == name) {
                i = true;
            }
        }
        return i;
    }


    // TODO: add more delegation functions as needed

};





#endif //RICHALUK_SCHEME_H
