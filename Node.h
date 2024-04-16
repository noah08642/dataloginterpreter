#ifndef RICHALUK_NODE_H
#define RICHALUK_NODE_H
#include<set>
#include<iostream>
#include<string>
using namespace std;

class Node {

private:

    set<int> adjacentNodeIDs;
    bool visited;
    int postOrderNumber;

public:

    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }

    void toString() {
        int count = 0;
        for (auto edge: adjacentNodeIDs) {
            cout  << "R" << edge;
            if (++count < adjacentNodeIDs.size()) {
                cout << ",";
            }
        }
    }

    bool isVisited() {
        if(visited) {
            return true;
        }
        else {
            return false;
        }
    }

    void setVisited(bool answer){
        visited = answer;
    }

    set<int> returnChildren() {
        return adjacentNodeIDs;
    }

};




#endif //RICHALUK_NODE_H
