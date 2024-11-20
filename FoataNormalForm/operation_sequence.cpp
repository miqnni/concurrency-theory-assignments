#include "operation_sequence.h"

#include <iostream>
#include <algorithm> // std::function
#include <functional> // std::sort()

void OperationSequence::calculateOperationRelations() {

    // Identity => dependent.
    for (const Operation& op : operations) {
        dependenceRelation.push_back({ op, op });
    }

    for (int i{}; i < operations.size(); i++) {
        for (int j{ i + 1 }; j < operations.size(); j++) {
            Operation op1 = operations[i];
            Operation op2 = operations[j];
            if (op1.isDependentOn(op2)) {
                dependenceRelation.push_back({ op1, op2 });
                dependenceRelation.push_back({ op2, op1 }); // Symmetry.
                continue;
            }
            independenceRelation.push_back({ op1, op2 });
            independenceRelation.push_back({ op2, op1 }); // Symmetry.
        }
    }

    std::function<bool(std::pair<Operation, Operation>, std::pair<Operation, Operation>)> comparePairs{
        [](std::pair<Operation, Operation> p1, std::pair<Operation, Operation> p2) {
            if (p1.first.getLabel() != p2.first.getLabel())
                return p1.first.getLabel() < p2.first.getLabel();

            return p1.second.getLabel() < p2.second.getLabel();
        }
    };
    
    std::sort(dependenceRelation.begin(), dependenceRelation.end(), comparePairs);
    std::sort(independenceRelation.begin(), independenceRelation.end(), comparePairs);


    //for (const std::vector<std::pair<Operation, Operation>>& relationVector : { dependenceRelation, independenceRelation })
    //    std::sort(relationVector.begin(), relationVector.end(), comparePairs);

}

void OperationSequence::printRelation(std::ostream& out, std::vector<std::pair<Operation, Operation>> relation, std::string relationLabel) {
    out << relationLabel << " = {";

    if (relation.empty()) {
        out << "}" << std::endl;
        return;
    }

    out << "(" << relation[0].first.getLabel() << ", " << relation[0].second.getLabel() << ")";

    for (int i{1}; i < relation.size(); i++)
        out << ", " << "(" << relation[i].first.getLabel() << ", " << relation[i].second.getLabel() << ")";

    out << std::endl;
}

std::map<std::pair<Operation, Operation>, bool> OperationSequence::getDependenceMap()
{
    std::map<std::pair<Operation, Operation>, bool> dependenceMap;
    for (std::pair<Operation, Operation> opPair : dependenceRelation) {
        dependenceMap[opPair] = true;
    }
    for (std::pair<Operation, Operation> opPair : independenceRelation) {
        dependenceMap[opPair] = false;
    }

    return dependenceMap;
}
