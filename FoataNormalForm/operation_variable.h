#pragma once
#include <iostream>
#include <string>

class OperationVariable {
private:
	std::string representation;
	//static int unknownVariableID;

public:
	OperationVariable(std::string representation) : representation{ representation } {}
	OperationVariable() : representation{ std::string{"(?"} + /*std::to_string(unknownVariableID) + */ std::string{")"} } {}

	friend std::ostream& operator<<(std::ostream &out, OperationVariable operationVariable);
	bool operator==(const OperationVariable& other) const { return representation == other.representation; }
};
//int OperationVariable::unknownVariableID{ 0 };
