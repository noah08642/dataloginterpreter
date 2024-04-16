#ifndef RICHALUK_GRAPH_H
#define RICHALUK_GRAPH_H
#include "Node.h"
#include<map>
#include<stack>
class Graph {

private:

    map<int,Node> nodes;
    stack<int> postOrder;

public:
    Graph() {}

    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++)
            nodes[nodeID] = Node();
    }

    vector<vector<int>> findSCCs(vector<int> order){
        vector<vector<int>> SCCs;
        for (auto & node: nodes) {
            node.second.setVisited(false);
        }

        for(int i = 0; i < order.size(); i++) {
            if (!nodes.at(order.at(i)).isVisited()) {
                vector<int> SCC;
                SCC = dfs2(order.at(i), SCC);
                sort(SCC.begin(), SCC.end());
                SCCs.push_back(SCC);
            }
        }
        return SCCs;
    }





    vector<int> dfsForest() {
        vector<int> postOrderNumbers;
        for (pair pair: nodes) {
            int identifier = pair.first;
            Node node = pair.second;
            if(!node.isVisited()) {
                dfs(identifier);
            }

        }

        while (!postOrder.empty()) {
            int x = postOrder.top();
            postOrder.pop();
            postOrderNumbers.push_back(x);
        }
    return postOrderNumbers;
    }

    void dfs(int index) {
        nodes.at(index).setVisited(true);
        for (int identifier: nodes.at(index).returnChildren()) {
            if (!nodes[identifier].isVisited()) {
                dfs(identifier);
            }
        }
        postOrder.push(index);
    }

    vector<int> dfs2(int index, vector<int> SCC) {
        vector<int> StrongCC = SCC;
        nodes.at(index).setVisited(true);
        for (int identifier: nodes.at(index).returnChildren()) {
            if (!nodes[identifier].isVisited()) {
                StrongCC = dfs2(identifier, StrongCC);
            }
        }
        StrongCC.push_back(index);
        return StrongCC;
    }














    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
    }

    void toString() {
        for (auto& pair: nodes) {
            int nodeID = pair.first;
            Node node = pair.second;
            cout << "R" << nodeID << ":";
            node.toString();
            cout << endl;
        }
    }

    map<int,Node> getGraph() {
        return nodes;
    }

};

#endif //RICHALUK_GRAPH_H
