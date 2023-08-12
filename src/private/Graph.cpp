#include "Graph.h"
#include "Helper.h"
#include <algorithm>

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

int Graph::findHighestAlphabetLetterIndex(vector<NodeRelationship*> nodeRelationships)
{
    int highestAlphabetLetterIndex = 0;

    for (NodeRelationship* nodeRelationship : nodeRelationships)
    {
        size_t node1Label = ALPHABET.find(nodeRelationship->node1Label);
        size_t node2Label = ALPHABET.find(nodeRelationship->node2Label);

		if (node1Label > highestAlphabetLetterIndex)
		{
            highestAlphabetLetterIndex = node1Label;
		}
		if (node2Label > highestAlphabetLetterIndex)
		{
            highestAlphabetLetterIndex = node2Label;
		}
    }

    return highestAlphabetLetterIndex;
}

void Graph::setupMatrix(vector<NodeRelationship*> nodeRelationships)
{
    matrixSize = findHighestAlphabetLetterIndex(nodeRelationships) + 1;

    for (int i = matrixSize - 1; i >= 0; i--)
    {
        string letter = ALPHABET.substr(i, 1);
        matrixX[letter].resize(matrixSize, 0);
        matrixY[letter].resize(matrixSize, 0);
    }

    matrix.resize(matrixSize, std::vector<int>(matrixSize, 0));

    for (NodeRelationship* nodeRelationship : nodeRelationships)
	{
		int nodeX = static_cast<int>(ALPHABET.find(nodeRelationship->node1Label));
		int nodeY = static_cast<int>(ALPHABET.find(nodeRelationship->node2Label));
        string dir = nodeRelationship->direction;

        if (dir == "->")
        {
            add(nodeX, nodeY);
        }
        else
        if (dir == "<-")
        {
            add(nodeY, nodeX);
        }
        else
        if (dir == "<>")
        {
            add(nodeX, nodeY);
            add(nodeY, nodeX);
        }
	}
}

void Graph::add(int x, int y)
{
    matrix[y][x] = 1;
}

void Graph::remove(int x, int y)
{
    matrix[y][x] = 0;
}

void Graph::addNode(int y)
{

}

void Graph::removeNode(int y)
{
    for (int x = 1; x < matrixSize; x++)
    {
        if (matrix[y][x] == 1)
        {
            remove(x, y);
            remove(y, x);
        }
    }
}


void Graph::removeNodesWithEdgeCount(int edgeCount)
{
for (int y = 0; y < matrixSize; ++y)
	{
		int edgeCountForNode = 0;

		for (int x = 0; x < matrixSize; ++x)
		{
			if (matrix[y][x] == 1)
			{
				edgeCountForNode++;
			}
		}

		if (edgeCountForNode == edgeCount)
		{
			removeNode(y);
		}
	}
    
}

string Graph::getStringOfNodeRelationships()
{
    string result = "";

    for (int i = 0; i < matrixSize; ++i)
    {
        for (int j = 0; j < matrixSize; ++j)
        {
            result += to_string(matrix[i][j]) + " ";
        }
        result += "\n";
    }

    return result;
}

void Graph::printNodeRelationships()
{
    string nodeRelationshipStr = getStringOfNodeRelationships();
    cout << nodeRelationshipStr;
}

void Graph::clean()
{

}