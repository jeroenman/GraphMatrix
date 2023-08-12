#include "Graph.h"
#include "Helper.h"

Graph::Graph(string txtFilePath)
{
    // CREATE NODES BASED ON TXT FILE
    vector<NodeRelationship*> nodeRelationships = createNodeRelationships(txtFilePath);
    setupMatrix(nodeRelationships);

    // CLEANUP nodeRelationships
    for (NodeRelationship* nodeRelationship : nodeRelationships)
    {
        delete nodeRelationship;
    }
}

vector<NodeRelationship*> Graph::createNodeRelationships(string txtPath)
{
    vector<NodeRelationship*> nodesRelationships;
    nodesRelationships.reserve(10);

    string s;
    ifstream in;

    in.open(txtPath);

    if (!in.is_open())
    {
        std::cerr << "Could not open file located at: "+ txtPath << std::endl;
        return nodesRelationships;
    }

    // CREATE A NodeRelationship FOR EACH LINE / RELATIONSHIP
    while (!in.eof())
    {
        getline(in, s);

        NodeRelationship* nodesRelationship = new NodeRelationship(s.substr(0, 1), s.substr(1, 2), s.substr(3, 1));
        nodesRelationships.push_back(nodesRelationship);
    }

    return nodesRelationships;
}

void Graph::setupMatrix(vector<NodeRelationship*> nodeRelationships)
{
    vector<string> uniqueLabels = getUniqueNodesLabels(nodeRelationships);
    matrixSize = static_cast<int>(uniqueLabels.size());

    for (string label : uniqueLabels)
    {
        // FAST WAY TO ADD A NODE WITH PREDEFINED SIZE
        vector<int> vecIn = vector<int>();
        vecIn.resize(matrixSize);
        matrixIn[label] = vecIn;
    }

    for (NodeRelationship* nodeRelationship : nodeRelationships)
    {
        string labelFrom = nodeRelationship->labelFrom;
        string labelTo = nodeRelationship->labelTo;
        string dir = nodeRelationship->direction;

        if (dir == "->")
        {
            addEdge(labelFrom, labelTo);
        }
        else
        if (dir == "<-")
        {
            addEdge(labelTo, labelFrom);
        }
        else
        if (dir == "<>")
        {
            addEdge(labelFrom, labelTo);
            addEdge(labelTo, labelFrom);
        }
    }
}

void Graph::addNode(string label)
{
    bool nodeExists = matrixIn.count(label);
    if (!nodeExists)
    {
        // ADD NODE TO MATRIX ROW
        vector<int> vecIn = vector<int>();
        matrixIn[label] = vecIn;
        matrixSize++;

        // ADD NODE TO MATRIX COLUMN
        int i = getIndexOfLabel(label);
        for (auto& pair : matrixIn)
        {
            if (pair.first == label)
            {
				pair.second.resize(matrixSize);
				continue;
			}

            pair.second.insert(pair.second.begin() + i, 0);
        }
    }
}

void Graph::removeNode(string label)
{
    bool nodeExists = matrixIn.count(label);
    if (nodeExists)
    {
        int i = getIndexOfLabel(label);

        matrixIn.erase(label);

        for (auto& pair : matrixIn)
        {
            spliceVectorAtIndex(pair.second, i);
        }

        matrixSize--;
    }
}

void Graph::addEdge(string labelFrom, string labelTo)
{
    int iFrom = getIndexOfLabel(labelFrom);
    matrixIn[labelTo][iFrom] = 1;
}

void Graph::removeEdge(string labelFrom, string labelTo)
{
    int iFrom = getIndexOfLabel(labelFrom);
    matrixIn[labelTo][iFrom] = 0;
}

void Graph::removeEdgeBidirectional(string labelFrom, string labelTo)
{
    removeEdge(labelFrom, labelTo);
    removeEdge(labelTo, labelFrom);
}

vector<string> Graph::getNodesWitIncomingNumberOfEdges(int edgeCount)
{
    vector <string> matchingNodes;

    for (const auto& pair : matrixIn)
    {
        const string& key = pair.first;
        const vector<int>& vec = pair.second;

        // COUNT INCOMING EDGES
        int edgeCountNode = 0;
        for (int i = 0; i < matrixSize; i++)
        {
            if (vec[i] == 1)
            {
                edgeCountNode++;
			}
        }

        // ADD IF MATCHING
        if (edgeCountNode == edgeCount)
        {
            matchingNodes.push_back(key);
        }
    }

    return matchingNodes;
}

void Graph::removeNodesWithIncomingNumberOfEdges(int edgeCount)
{
    vector<string> matchingNodes = getNodesWitIncomingNumberOfEdges(edgeCount);
    for (string label : matchingNodes)
    {
        removeNode(label);
    }
}

string Graph::getLabelAtIndex(int i)
{
    int counter = 0;
    for (auto& pair : matrixIn)
    {
        if (counter == i)
        {
            const string& key = pair.first;
            return key;
        }

        counter++;
    }

    return "";
}

int Graph::getIndexOfLabel(string label)
{
    auto it = matrixIn.find(label);
    int index = 0;

    for (auto it_start = matrixIn.begin(); it_start != it; it_start++)
    {
        index++;
    }

    return index;
}

vector <string> Graph::getUniqueNodesLabels(vector<NodeRelationship*> nodeRelationships)
{
    vector<string> uniqueLabels = vector<string>();

    for (NodeRelationship* nodeRelationship : nodeRelationships)
    {
        string labelFrom = nodeRelationship->labelFrom;
        string labelTo = nodeRelationship->labelTo;

        int iFrom = getIndexOfElementInVector(uniqueLabels, labelFrom);
        if (iFrom == -1)
        {
            uniqueLabels.push_back(labelFrom);
        }

        int iTo = getIndexOfElementInVector(uniqueLabels, labelTo);
        if (iTo == -1)
        {
            uniqueLabels.push_back(labelTo);
        }
    }

    return uniqueLabels;
}

string Graph::getStringOfMatrix()
{
    string result = "";

    for (const auto& pair : matrixIn)
    {
        const string& key = pair.first;
        const vector<int>& vec = pair.second;

        for (int i = 0; i < matrixSize; i++)
        {
            result += to_string(matrixIn[key][i]) + " ";
        }

        result += "\n";
    }

    return result;
}

string Graph::getStringOfNodeRelationships()
{
    std::map<string, bool> nodeRelationshipsToIgnoreMap;

    string result = "";

    for (const auto& pair : matrixIn)
    {
        const string& edgeInLabel = pair.first;
        const vector<int>& vec = pair.second;

        for (int i = 0; i < matrixSize; i++)
        {
            int edgeIn = matrixIn[edgeInLabel][i];
            if (edgeIn == 1)
            {
                string edgeOutLabel = getLabelAtIndex(i);
                int labelIndex = getIndexOfLabel(edgeInLabel);
                int edgeOut = matrixIn[edgeOutLabel][labelIndex];
                string nodeRelationshipStr = "";
                string nodeRelationshipStrInv = "";

                if (edgeOut == 0)
                {
                    // DIRECTIONAL CASE
                    nodeRelationshipStr = edgeOutLabel + "->" + edgeInLabel;

                    if (nodeRelationshipsToIgnoreMap[nodeRelationshipStr])
                    {
                        // ALREADY ADDED 
                        continue;
                    }

                    nodeRelationshipStrInv = edgeInLabel + "->" + edgeOutLabel;
                }
                else
                if (edgeOut == 1)
                {
                    // BIDIRECTIONAL CASE
                    nodeRelationshipStr = edgeOutLabel + "<>" + edgeInLabel;

                    if (nodeRelationshipsToIgnoreMap[nodeRelationshipStr])
                    {
                        // ALREADY ADDED 
                        continue;
                    }

                    nodeRelationshipStrInv = edgeInLabel + "<>" + edgeOutLabel;
                }

                nodeRelationshipsToIgnoreMap[nodeRelationshipStrInv] = true;

                result += nodeRelationshipStr + "\n";
			}
        }
    }

    return result;
}

void Graph::printMatrix()
{
    string matrixStr = getStringOfMatrix();
    cout << matrixStr;
}

void Graph::printNodeRelationships()
{
    string nodeRelationshipStr = getStringOfNodeRelationships();
    cout << nodeRelationshipStr;
}

void Graph::clean()
{
    map<string, vector<int>>().swap(matrixIn);
    matrixSize = 0;
}