#pragma once

#include "Graph.h"
#include "Helper.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct NodeRelationship
{
    NodeRelationship(string _nodeFrom, string _direction, string _nodeTo)
    {
        labelFrom = _nodeFrom;
        labelTo = _nodeTo;
        direction = _direction;
    }

    string labelFrom;
    string labelTo;
    string direction;
};

class Graph
{
    public:
        Graph(string txtFilePath);

        void addNode(string label);
        void removeNode(string label);
        void addEdge(string labelFrom, string labelTo);
        void removeEdge(string labelFrom, string labelTo);
        void removeEdgeBidirectional(string labelFrom, string labelTo);
        void removeNodesWithIncomingNumberOfEdges(int edgeCount);

        void printMatrix();
        void printNodeRelationships();

        void clean();

    private:
        vector<NodeRelationship*> createNodeRelationships(string txtPath);
        void setupMatrix(vector<NodeRelationship*> nodeRelationships);

        string getStringOfMatrix();
        string getStringOfNodeRelationships();

        vector<string> getNodesWitIncomingNumberOfEdges(int edgeCount);
        vector <string> getUniqueNodesLabels(vector<NodeRelationship*> nodeRelationships);
        int getIndexOfLabel(string label);
        string getLabelAtIndex(int i);

        int matrixSize = 0;
        map<string, vector<int>> matrixIn;
};
