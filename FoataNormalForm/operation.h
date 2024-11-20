#pragma once
#include "operation_variable.h"
#include <iostream>
#include <vector>
#include <string>

class Operation {
private:
	std::string label; // (e.g.) a
	std::string representation; // (e.g.) x := y + z
	OperationVariable modifiedVariable; // (e.g.) x
	std::vector<OperationVariable> otherVariablesOperatedOn; // (e.g.) {y, z}
	
public:
	Operation() {}

	//Operation(const Operation& other, std::string newLabel)
	//	: label(newLabel),
	//	representation(other.representation),
	//	modifiedVariable(other.modifiedVariable),
	//	otherVariablesOperatedOn(other.otherVariablesOperatedOn) {}

	// Copy constructor
    Operation(const Operation& other)
        : label(other.label),
          representation(other.representation),
          modifiedVariable(other.modifiedVariable),
          otherVariablesOperatedOn(other.otherVariablesOperatedOn) {}

    // Copy assignment operator
    Operation& operator=(const Operation& other) {
        if (this != &other) {
            label = other.label;
            representation = other.representation;
            modifiedVariable = other.modifiedVariable;
            otherVariablesOperatedOn = other.otherVariablesOperatedOn;
        }
        return *this;
    }

	Operation(std::string label, OperationVariable modifiedVariable, std::vector<OperationVariable> otherVariablesOperatedOn)
		: label{ label }, representation{ "" }, modifiedVariable { modifiedVariable }, otherVariablesOperatedOn{ otherVariablesOperatedOn }
	{}

	Operation(const std::string representation, const bool withLabel = false);

	bool isDependentOn(const Operation& other) const;

	// Required if we want Operation to be std::map key.
	bool operator<(const Operation& other) const { return this->getLabel() < other.getLabel(); }

	friend std::ostream& operator<<(std::ostream& out, Operation operation);

	std::string getLabel() const { return label; }

	void setLabel(std::string newLabel) { label = newLabel; }

};
