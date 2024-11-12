#include "toy_value.h"
#include "toy_console_colors.h"

#include "toy_string.h"

#include <stdio.h>
#include <stdlib.h>

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
			fprintf(stderr, TOY_CC_ERROR "ERROR: Can't hash an unknown value type, exiting\n" TOY_CC_RESET);
			exit(-1);
	}

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
			fprintf(stderr, TOY_CC_ERROR "ERROR: Can't copy an unknown value type, exiting\n" TOY_CC_RESET);
			exit(-1);
	}

	//dummy return
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
			fprintf(stderr, TOY_CC_ERROR "ERROR: Can't free an unknown value type, exiting\n" TOY_CC_RESET);
			exit(-1);
	}
}

bool Toy_checkValueIsTruthy(Toy_Value value) {
	//null is an error
	if (TOY_VALUE_IS_NULL(value)) {
		Toy_error("'null' is neither true nor false");
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
			fprintf(stderr, TOY_CC_ERROR "ERROR: Unknown types in value equality, exiting\n" TOY_CC_RESET);
			exit(-1);
	}

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
			fprintf(stderr, TOY_CC_ERROR "Unknown types in value comparison check, exiting\n" TOY_CC_RESET);
			exit(-1);
	}

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
			fprintf(stderr, TOY_CC_ERROR "Unknown types in value comparison, exiting\n" TOY_CC_RESET);
			exit(-1);
	}

	return -1;
}

void Toy_stringifyValue(Toy_Value value, Toy_callbackType callback) {
	//NOTE: don't append a newline
	switch(value.type) {
		case TOY_VALUE_NULL:
			callback("null");
			break;

		case TOY_VALUE_BOOLEAN:
			callback(TOY_VALUE_AS_BOOLEAN(value) ? "true" : "false");
			break;

		case TOY_VALUE_INTEGER: {
			char buffer[16];
			sprintf(buffer, "%d", TOY_VALUE_AS_INTEGER(value));
			callback(buffer);
			break;
		}

		case TOY_VALUE_FLOAT: {
			char buffer[16];
			sprintf(buffer, "%f", TOY_VALUE_AS_FLOAT(value));
			callback(buffer);
			break;
		}

		case TOY_VALUE_STRING: {
			Toy_String* str = TOY_VALUE_AS_STRING(value);

			//TODO: decide on how long strings, etc. live for in memory
			if (str->type == TOY_STRING_NODE) {
				char* buffer = Toy_getStringRawBuffer(str);
				callback(buffer);
				free(buffer);
			}
			else if (str->type == TOY_STRING_LEAF) {
				callback(str->as.leaf.data);
			}
			else if (str->type == TOY_STRING_NAME) {
				callback(str->as.name.data); //should this be a thing?
			}
			break;
		}

		case TOY_VALUE_ARRAY:
		case TOY_VALUE_TABLE:
		case TOY_VALUE_FUNCTION:
		case TOY_VALUE_OPAQUE:
		case TOY_VALUE_TYPE:
		case TOY_VALUE_ANY:
		case TOY_VALUE_UNKNOWN:
			fprintf(stderr, TOY_CC_ERROR "Unknown types in value stringify, exiting\n" TOY_CC_RESET);
			exit(-1);
	}
}

const char* Toy_private_getValueTypeAsCString(Toy_ValueType type) {
	switch (type) {
		case TOY_VALUE_NULL: return "null";
		case TOY_VALUE_BOOLEAN: return "bool";
		case TOY_VALUE_INTEGER: return "int";
		case TOY_VALUE_FLOAT: return "float";
		case TOY_VALUE_STRING: return "string";
		case TOY_VALUE_ARRAY: return "array";
		case TOY_VALUE_TABLE: return "table";
		case TOY_VALUE_FUNCTION: return "function";
		case TOY_VALUE_OPAQUE: return "opaque";
		case TOY_VALUE_TYPE: return "type";
		case TOY_VALUE_ANY: return "any";
		case TOY_VALUE_UNKNOWN: return "unknown";
	}

	return NULL;
}