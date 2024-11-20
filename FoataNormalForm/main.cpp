#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

#include "operation_variable.h"
#include "operation.h"
#include "operation_sequence.h"

std::vector<std::string> parseAlphabet(const std::string alphabetSet) {
    // Format: e.g. A = { a, b, c, d }
    std::istringstream is{ alphabetSet };
    char currentChar{ '\0' };
    std::vector<std::string> alphabetResult;

    // Ignore `A = `.
    is.ignore(4);

    std::string currentOperationLabel;
    while (is.good()) {
        if (!isalpha(currentChar = is.get()))
            continue; // Skip chars that don't make up the operation label name.

        // New label started.
        currentOperationLabel.clear();
        do {
            currentOperationLabel += currentChar;
        } while (isalpha(currentChar = is.get()));

        alphabetResult.push_back(currentOperationLabel);
    }

    return alphabetResult;
}

std::vector<std::string> parseTrailIntoSingleCharLabels(const std::string trail) {
    std::vector<std::string> trailResult{};

    int startIdx{ 0 };

    // Ignore `w = `.
    for (int i{ 0 }; i < trail.size(); i++) {
        if (trail[i] == '=') {
            startIdx = i + 1;
            break;
        }
    }
    for (int i{ startIdx }; i < trail.size(); i++) {
        if (trail[i] != ' ') {
            startIdx = i;
            break;
        }
    }

    for (int i{ startIdx }; i < trail.size(); i++) {
        trailResult.push_back(std::string{ trail[i]});
    }

    return trailResult;
}

void taskParsingAndSolution(std::istream &in = std::cin, std::ostream &out = std::cout) {

    std::string line;
    std::string alphabetLine;
    std::string trailLine;
    std::vector<Operation> inputOperations;
    while (std::getline(in, line))
    {
        if (line.substr(0, 3) == "A =")
            break;

        inputOperations.push_back(Operation(line));
    }
    alphabetLine = line;
    std::getline(in, trailLine);

    std::vector<std::string> inputAlphabet = parseAlphabet(alphabetLine);
    std::vector<std::string> inputTrail = parseTrailIntoSingleCharLabels(trailLine);

    OperationSequence operationSequence{
        inputOperations,
        inputAlphabet,
        inputTrail
    };

    operationSequence.calculateOperationRelations();
    operationSequence.printDependenceRelation();
    out << std::endl;
    operationSequence.printIndependenceRelation();

    std::map<std::string, int> labelToInt;
    std::map<int, std::string> intToLabel;

    std::map<std::pair<Operation, Operation>, bool> dependenceMap{ operationSequence.getDependenceMap() };

    std::vector<Operation> operationsCopy{ operationSequence.getOperations() };
    size_t labelCount{ operationsCopy.size() };

    for (int labelID{}; labelID < labelCount; labelID++) {
        Operation& op = operationsCopy[labelID];
        labelToInt[op.getLabel()] = labelID;
        intToLabel[labelID] = op.getLabel();
    }

    std::vector<std::vector<bool>> labelNumDependenceMatrix{
        labelCount,
        std::vector<bool>(labelCount)
    };
    for (int i{}; i < labelCount; i++) {
        for (int j{}; j < labelCount; j++) {
            labelNumDependenceMatrix[i][j] = dependenceMap[{operationsCopy[i], operationsCopy[j]}];
        }
    }

    /*out << std::endl << std::endl;
    for (int i{}; i < labelCount; i++) {
        for (int j{}; j < labelCount; j++) {
            out << (labelNumDependenceMatrix[i][j]) << " ";
        }
        out << std::endl;
    }*/

    // Now, everything goes according to the provided trail.
    size_t nodeCount{ operationSequence.getTrail().size() };
    std::vector<std::vector<bool>> graphAdjacencyMatrix{
        nodeCount,
        std::vector<bool>(nodeCount)
    };

    std::vector<int> trailInIDs;
    for (std::string opLabel : operationSequence.getTrail())
        trailInIDs.push_back(labelToInt[opLabel]);


    // CREATE GRAPH. Mark dependence everywhere.
    for (int nodeI{}; nodeI < nodeCount; nodeI++) {
        for (int nodeJ{nodeI + 1}; nodeJ < nodeCount; nodeJ++) {
            int labelNumI = trailInIDs[nodeI];
            int labelNumJ = trailInIDs[nodeJ];
            if (labelNumDependenceMatrix[labelNumI][labelNumJ])
                graphAdjacencyMatrix[nodeI][nodeJ] = true;
        }
    }

    // Minimize graph.
    for (int i{}; i < nodeCount; i++) {
        for (int j{ i + 1 }; j < nodeCount; j++) {
            for (int k{ j + 1 }; k < nodeCount; k++) {
                if (graphAdjacencyMatrix[i][k] && graphAdjacencyMatrix[i][j] && graphAdjacencyMatrix[j][k])
                    graphAdjacencyMatrix[i][k] = false;
            }
        }
    }

    // PRINT GRAPH
    out << std::endl;
    out << "  ";
    for (int j{}; j < nodeCount; j++) {
        out << (inputTrail[j]) << " ";
    }
    out << std::endl;
    for (int i{}; i < nodeCount; i++) {
        out << inputTrail[i] << " ";
        for (int j{}; j < nodeCount; j++) {
            out << (graphAdjacencyMatrix[i][j]) << " ";
        }
        out << std::endl;
    }

    // FNF
    // - graph copy
    std::vector<std::vector<bool>> graphCpy{
        nodeCount,
        std::vector<bool>(nodeCount)
    };
    for (int nodeI{}; nodeI < nodeCount; nodeI++) {
        for (int nodeJ{}; nodeJ < nodeCount; nodeJ++) {
            graphCpy[nodeI][nodeJ] = graphAdjacencyMatrix[nodeI][nodeJ];
        }
    }

    // - Iterate through graph. Find nodes without incoming edges and remove them (also skip all edges from them later on).
    std::vector<std::vector<std::string>> fnf;
    std::vector<int> activeNodes;
    for (int i{}; i < nodeCount; i++)
        activeNodes.push_back(i);

    std::vector<bool> isRemoved(nodeCount);
    for (int removeNodesIteration{}; removeNodesIteration < nodeCount; removeNodesIteration++) {
        std::vector<bool> hasIncomingEdges(nodeCount);
        for (int i{}; i < nodeCount; i++) {
            if (isRemoved[i]) // Do not consider edges from removed nodes.
                continue;

            // If a node is not removed, mark its neighbors (as nodes with incoming edges).
            for (int j{i + 1}; j < nodeCount; j++) {
                if (graphCpy[i][j])
                    hasIncomingEdges[j] = true;
            }
        }

        // Remove nodes with no incoming edges
        std::vector<std::string> newFoataClass{};
        for (int i{}; i < nodeCount; i++) {
            if (isRemoved[i] || hasIncomingEdges[i])
                continue;

            // If a node has no incoming edges, mark it as removed 
            isRemoved[i] = true;
            newFoataClass.push_back(inputTrail[i]);
        }
        // If we found any nodes to remove, add their labels to FNF within one class.
        if (!newFoataClass.empty())
            fnf.push_back(newFoataClass);
    }

    // Print FNF
    out << std::endl;
    out << "FNF([w]) = ";
    for (const auto& foataClass : fnf) {
        if (foataClass.empty()) {
            out << "(ERR!)";
        }
        out << "(" << foataClass[0];
        for (int i{1}; i < foataClass.size(); i++) {
            out << " " << foataClass[i];
        }
        out << ")";
    }
    out << std::endl;


}

int main()
{
    std::ifstream infile{"sample_input03.txt"};
    if (!infile) {
        std::cout << "FILE OPEN ERROR" << std::endl;
        return EXIT_FAILURE;
    }
    taskParsingAndSolution(infile);

    infile.close();
    return 0;
}
