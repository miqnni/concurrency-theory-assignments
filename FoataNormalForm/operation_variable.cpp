#include "operation_variable.h"

std::ostream& operator<<(std::ostream &out, OperationVariable operationVariable) {
	out << operationVariable.representation;
	return out;
}
