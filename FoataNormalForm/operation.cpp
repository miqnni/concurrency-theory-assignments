#include <sstream>
#include <cctype> // isalpha()

#include "operation.h"
#include "operation_variable.h"

Operation::Operation(const std::string representation, const bool withLabel) : representation{ representation } {
	// Representation format example
	// With label:     (a) y := 2y + w - 3v
	// Without label:  y := 2y + w - 3v

	std::istringstream is(representation);
	char currentChar{ '\0' };

	// (1) Get the label (if present).
	label = "?";
	if (withLabel) {
		while (!isalpha(currentChar = is.get()))
			; // skip char

		std::string readLabel{ currentChar };
		while (isalpha(currentChar = is.get())) {
			readLabel += currentChar;
		}
		label = readLabel;
		//std::cout << "LABEL: " + label << std::endl;
	}
	

	// (2) Get the modified variable.
	while (!isalpha(currentChar = is.get()))
		; // skip char

	std::string readModifiedVariable{currentChar};
	while (isalpha(currentChar = is.get())) {
		readModifiedVariable += currentChar;
	}
	modifiedVariable = OperationVariable(readModifiedVariable);
	//std::cout << "\tmv: " << modifiedVariable << std::endl;


	// (3) Get other variables, until EOF is reached.
	
	//otherVariablesOperatedOn = std::vector<OperationVariable>{};
	//std::string readVariable{""};
	//bool eofReached{};
	//std::cout << "\tov: ";
	//while ((currentChar = is.get()) != EOF) {
	//	if (!isalpha(currentChar))
	//		continue;

	//	// Variable name started.
	//	eofReached = true;
	//	readVariable = "" + currentChar;
	//	while ((currentChar = is.get()) != EOF) {
	//		if (!isalpha(currentChar))
	//		{
	//			// Variable name ended.
	//			otherVariablesOperatedOn.push_back(OperationVariable(readVariable));
	//			std::cout << readVariable << ", ";
	//			readVariable = "";
	//			eofReached = false; // Now we know the loop ended because of the end of variable name and not due to EOF.
	//			break;
	//		}

	//		readVariable += currentChar;
	//	}

	//	if (eofReached)
	//		break; // Do not check the outer while-loop condition anymore.
	//}

	//if (!readVariable.empty()) { // This means EOF was reached while reading the variable name.
	//	otherVariablesOperatedOn.push_back(OperationVariable(readVariable));
	//	std::cout << readVariable;
	//}
	//std::cout << std::endl;

	otherVariablesOperatedOn.clear();
	std::string readVariable;
	//std::cout << "\tov: ";
	while (is.good()) {
		if (!isalpha(currentChar = is.get()))
			continue; // Skip chars that don't make up the variable name.

		// New variable started.
		readVariable.clear();
		do {
			readVariable += currentChar;
		} while (isalpha(currentChar = is.get()));

		otherVariablesOperatedOn.push_back(OperationVariable(readVariable));
		//std::cout << readVariable << ", ";
	}
	//std::cout << std::endl;

}


bool Operation::isDependentOn(const Operation &other) const {
	if (this == &other)
		return true;

	// Edge case: Both operation modify the same variable that does not appear on the rhs of the operation.
	if (this->modifiedVariable == other.modifiedVariable)
		return true;


	for (const OperationVariable& var_oth : other.otherVariablesOperatedOn) {
		if (this->modifiedVariable == var_oth)
			return true;
	}
	
	for (const OperationVariable& var_this : this->otherVariablesOperatedOn) {
		if (other.modifiedVariable == var_this)
			return true;
	}

	return false;

}

std::ostream& operator<<(std::ostream& out, Operation operation) {
	if (!operation.representation.empty()) {
		out << operation.representation;
		return out;
	}

	// Default representation: the modifiedVariable is assigned with the sum of other variables or (if no other variables exist) 0.
	out << "(" << operation.label << ") "
	<< operation.modifiedVariable
	<< " != ";
	if (operation.otherVariablesOperatedOn.empty()) {
		out << "0";
		return out;
	}

	out << operation.otherVariablesOperatedOn[0];
	for (int i{ 1 }; i < operation.otherVariablesOperatedOn.size(); i++)
		out << " + " << operation.otherVariablesOperatedOn[i];
	
	return out;
}
