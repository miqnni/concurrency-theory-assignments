#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility> // std::pair
#include <map>

#include "operation_variable.h"
#include "operation.h"

class OperationSequence {
private:
	std::vector<Operation> operations;
	std::vector<std::string> alphabet;
	std::vector<std::string> trail;
	std::vector<std::pair<Operation, Operation>> dependenceRelation;
	std::vector<std::pair<Operation, Operation>> independenceRelation;

	void printRelation(std::ostream& out, std::vector<std::pair<Operation, Operation>> relation, std::string relationLabel);

public:
	OperationSequence(
		std::vector<Operation> operations,
		std::vector<std::string> alphabet,
		std::vector<std::string> trail
	)
		//: operations(std::move(operations)), alphabet(std::move(alphabet)), trail(std::move(trail)) // std::move nic tu nie zmienia
		: operations(operations), alphabet(alphabet), trail(trail)
	{
		for (int i = 0; i < this->operations.size(); i++) {
			this->operations[i].setLabel(this->alphabet[i]);
		}
	}

	void calculateOperationRelations();
	void printDependenceRelation(std::ostream& out = std::cout) { printRelation(out, dependenceRelation, "D"); }
	void printIndependenceRelation(std::ostream& out = std::cout) { printRelation(out, independenceRelation, "I"); }
	std::map<std::pair<Operation, Operation>, bool> getDependenceMap();

	std::vector<Operation> getOperations(){ return operations; }
	std::vector<std::string> getTrail(){ return trail; }
};
