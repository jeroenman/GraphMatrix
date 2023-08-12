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
    NodeRelationship(string _node1Label, string _direction, string _node2Label)
    {
        node1Label = _node1Label;
        direction = _direction;
        node2Label = _node2Label;
    }

    string node1Label;
    string node2Label;
    string direction;
};

class Graph
{
    public:
        Graph(string txtFilePath);

        void add(int x, int y);
        void remove(int x, int y);
        void addNode(int y);
        void removeNode(int y);
        void removeNodesWithEdgeCount(int edgeCount);
        void printNodeRelationships();
        void clean();

    private:
        vector<NodeRelationship*> createNodeRelationships(string txtPath);
        void setupMatrix(vector<NodeRelationship*> nodeRelationships);
        string getStringOfNodeRelationships();
        int findHighestAlphabetLetterIndex(vector<NodeRelationship*> nodeRelationships);

        int matrixSize = 0;
        vector<vector<int>> matrix;

        map<string, vector<int>> matrixX;
        map<string, vector<int>> matrixY;
};
