#include "toy_value.h"
#include "toy_console_colors.h"

#include "toy_string.h"
#include "toy_print.h"

//utils
static unsigned int hashUInt(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

//exposed functions
unsigned int Toy_hashValue(Toy_Value value) {
	switch(value.type) {
		case TOY_VALUE_NULL:
			return 0;

		case TOY_VALUE_BOOLEAN:
			return TOY_VALUE_AS_BOOLEAN(value) ? 1 : 0;

		case TOY_VALUE_INTEGER:
			return hashUInt(TOY_VALUE_AS_INTEGER(value));

		case TOY_VALUE_FLOAT:
			return hashUInt( *((int*)(&TOY_VALUE_AS_FLOAT(value))) );

		case TOY_VALUE_STRING:
			return Toy_hashString(TOY_VALUE_AS_STRING(value));

		case TOY_VALUE_ARRAY:
		case TOY_VALUE_TABLE:
		case TOY_VALUE_FUNCTION:
		case TOY_VALUE_OPAQUE:
		case TOY_VALUE_TYPE:
		case TOY_VALUE_ANY:
		case TOY_VALUE_UNKNOWN:
			break;
	}

	Toy_error(TOY_CC_ERROR "ERROR: Can't hash an unknown value type\n" TOY_CC_RESET);
	return 0;
}

Toy_Value Toy_copyValue(Toy_Value value) {
	switch(value.type) {
		case TOY_VALUE_NULL:
		case TOY_VALUE_BOOLEAN:
		case TOY_VALUE_INTEGER:
		case TOY_VALUE_FLOAT:
			return value;

		case TOY_VALUE_STRING: {
			Toy_String* string = TOY_VALUE_AS_STRING(value);
			return TOY_VALUE_FROM_STRING(Toy_copyString(string));
		}

		case TOY_VALUE_ARRAY:
		case TOY_VALUE_TABLE:
		case TOY_VALUE_FUNCTION:
		case TOY_VALUE_OPAQUE:
		case TOY_VALUE_TYPE:
		case TOY_VALUE_ANY:
		case TOY_VALUE_UNKNOWN:
			break;
	}

	//dummy return
	Toy_error(TOY_CC_ERROR "ERROR: Can't copy an unknown value type\n" TOY_CC_RESET);
	return TOY_VALUE_FROM_NULL();
}

void Toy_freeValue(Toy_Value value) {
	switch(value.type) {
		case TOY_VALUE_NULL:
		case TOY_VALUE_BOOLEAN:
		case TOY_VALUE_INTEGER:
		case TOY_VALUE_FLOAT:
			break;

		case TOY_VALUE_STRING: {
			Toy_String* string = TOY_VALUE_AS_STRING(value);
			Toy_freeString(string);
			break;
		}

		case TOY_VALUE_ARRAY:
		case TOY_VALUE_TABLE:
		case TOY_VALUE_FUNCTION:
		case TOY_VALUE_OPAQUE:
		case TOY_VALUE_TYPE:
		case TOY_VALUE_ANY:
		case TOY_VALUE_UNKNOWN:
			Toy_error(TOY_CC_ERROR "ERROR: Can't free an unknown type\n" TOY_CC_RESET);
	}
}

bool Toy_checkValueIsTruthy(Toy_Value value) {
	//null is an error
	if (TOY_VALUE_IS_NULL(value)) {
		Toy_error(TOY_CC_ERROR "ERROR: 'null' is neither true nor false\n" TOY_CC_RESET);
		return false;
	}

	//only 'false' is falsy
	if (TOY_VALUE_IS_BOOLEAN(value)) {
		return TOY_VALUE_AS_BOOLEAN(value);
	}

	//anything else is truthy
	return true;
}

bool Toy_checkValuesAreEqual(Toy_Value left, Toy_Value right) {
	switch(left.type) {
		case TOY_VALUE_NULL:
			return TOY_VALUE_IS_NULL(right);

		case TOY_VALUE_BOOLEAN:
			return TOY_VALUE_IS_BOOLEAN(right) && TOY_VALUE_AS_BOOLEAN(left) == TOY_VALUE_AS_BOOLEAN(right);

		case TOY_VALUE_INTEGER:
			if (TOY_VALUE_IS_INTEGER(right)) {
				return TOY_VALUE_AS_INTEGER(left) == TOY_VALUE_AS_INTEGER(right);
			}
			else if (TOY_VALUE_IS_FLOAT(right)) {
				return TOY_VALUE_AS_INTEGER(left) == TOY_VALUE_AS_FLOAT(right);
			}
			else {
				break;
			}

		case TOY_VALUE_FLOAT:
			if (TOY_VALUE_IS_INTEGER(right)) {
				return TOY_VALUE_AS_FLOAT(left) == TOY_VALUE_AS_INTEGER(right);
			}
			else if (TOY_VALUE_IS_FLOAT(right)) {
				return TOY_VALUE_AS_FLOAT(left) == TOY_VALUE_AS_FLOAT(right);
			}
			else {
				break;
			}

		case TOY_VALUE_STRING:
			if (TOY_VALUE_IS_STRING(right)) {
				return Toy_compareStrings(TOY_VALUE_AS_STRING(left), TOY_VALUE_AS_STRING(right)) == 0;
			}
			else {
				break;
			}

		case TOY_VALUE_ARRAY:
		case TOY_VALUE_TABLE:
		case TOY_VALUE_FUNCTION:
		case TOY_VALUE_OPAQUE:
		case TOY_VALUE_TYPE:
		case TOY_VALUE_ANY:
		case TOY_VALUE_UNKNOWN:
			break;
	}

	Toy_error(TOY_CC_ERROR "ERROR: Unknown types in value equality\n" TOY_CC_RESET);
	return false;
}

bool Toy_checkValuesAreComparable(Toy_Value left, Toy_Value right) {
	switch(left.type) {
		case TOY_VALUE_NULL:
			return false;

		case TOY_VALUE_BOOLEAN:
			return TOY_VALUE_IS_BOOLEAN(right);

		case TOY_VALUE_INTEGER:
		case TOY_VALUE_FLOAT:
			return TOY_VALUE_IS_INTEGER(right) || TOY_VALUE_IS_FLOAT(right);

		case TOY_VALUE_STRING:
			return TOY_VALUE_IS_STRING(right);

		case TOY_VALUE_ARRAY:
		case TOY_VALUE_TABLE:
		case TOY_VALUE_FUNCTION:
		case TOY_VALUE_OPAQUE:
		case TOY_VALUE_TYPE:
		case TOY_VALUE_ANY:
		case TOY_VALUE_UNKNOWN:
			break;
	}

	Toy_error(TOY_CC_ERROR "ERROR: Unknown types in value comparison check\n" TOY_CC_RESET);
	return false;
}

int Toy_compareValues(Toy_Value left, Toy_Value right) {
	//comparison means there's a difference in value, with some kind of quantity - so null, bool, etc. aren't comparable
	switch(left.type) {
		case TOY_VALUE_NULL:
		case TOY_VALUE_BOOLEAN:
			break;

		case TOY_VALUE_INTEGER:
			if (TOY_VALUE_IS_INTEGER(right)) {
				return TOY_VALUE_AS_INTEGER(left) - TOY_VALUE_AS_INTEGER(right);
			}
			else if (TOY_VALUE_IS_FLOAT(right)) {
				return TOY_VALUE_AS_INTEGER(left) - TOY_VALUE_AS_FLOAT(right);
			}
			else {
				break;
			}

		case TOY_VALUE_FLOAT:
			if (TOY_VALUE_IS_INTEGER(right)) {
				return TOY_VALUE_AS_FLOAT(left) - TOY_VALUE_AS_INTEGER(right);
			}
			else if (TOY_VALUE_IS_FLOAT(right)) {
				return TOY_VALUE_AS_FLOAT(left) - TOY_VALUE_AS_FLOAT(right);
			}
			else {
				break;
			}

		case TOY_VALUE_STRING:
			if (TOY_VALUE_IS_STRING(right)) {
				return Toy_compareStrings(TOY_VALUE_AS_STRING(left), TOY_VALUE_AS_STRING(right));
			}

		case TOY_VALUE_ARRAY:
		case TOY_VALUE_TABLE:
		case TOY_VALUE_FUNCTION:
		case TOY_VALUE_OPAQUE:
		case TOY_VALUE_TYPE:
		case TOY_VALUE_ANY:
		case TOY_VALUE_UNKNOWN:
			break;
	}

	Toy_error(TOY_CC_ERROR "ERROR: Unknown types in value comparison\n" TOY_CC_RESET);
	return -1;
}
