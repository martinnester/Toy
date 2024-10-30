#pragma once

#include "toy_common.h"

//forward declarations
struct Toy_String;

typedef enum Toy_ValueType {
	TOY_VALUE_NULL,
	TOY_VALUE_BOOLEAN,
	TOY_VALUE_INTEGER,
	TOY_VALUE_FLOAT,
	TOY_VALUE_STRING,
	TOY_VALUE_ARRAY,
	TOY_VALUE_TABLE,
	TOY_VALUE_FUNCTION,
	TOY_VALUE_OPAQUE,
	TOY_VALUE_TYPE,
	TOY_VALUE_ANY,
	TOY_VALUE_UNKNOWN, //The correct value is unknown, but will be determined later
} Toy_ValueType;

//8 bytes in size
typedef struct Toy_Value {          //32 | 64 BITNESS
	union {
		bool boolean;               //1  | 1
		int integer;                //4  | 4
		float number;               //4  | 4
		struct Toy_String* string;  //4  | 8
		//TODO: more types go here
		//TODO: consider 'stack' as a possible addition
	} as;                           //4  | 8

	Toy_ValueType type;             //4  | 4
} Toy_Value;                        //8  | 16

#define TOY_VALUE_IS_NULL(value)				((value).type == TOY_VALUE_NULL)
#define TOY_VALUE_IS_BOOLEAN(value)				((value).type == TOY_VALUE_BOOLEAN)
#define TOY_VALUE_IS_INTEGER(value)				((value).type == TOY_VALUE_INTEGER)
#define TOY_VALUE_IS_FLOAT(value)				((value).type == TOY_VALUE_FLOAT)
#define TOY_VALUE_IS_STRING(value)				((value).type == TOY_VALUE_STRING)
#define TOY_VALUE_IS_ARRAY(value)				((value).type == TOY_VALUE_ARRAY)
#define TOY_VALUE_IS_TABLE(value)				((value).type == TOY_VALUE_TABLE)
#define TOY_VALUE_IS_FUNCTION(value)			((value).type == TOY_VALUE_FUNCTION)
#define TOY_VALUE_IS_OPAQUE(value)				((value).type == TOY_VALUE_OPAQUE)

#define TOY_VALUE_AS_BOOLEAN(value)				((value).as.boolean)
#define TOY_VALUE_AS_INTEGER(value)				((value).as.integer)
#define TOY_VALUE_AS_FLOAT(value)				((value).as.number)
#define TOY_VALUE_AS_STRING(value)				((value).as.string)
//TODO: more

#define TOY_VALUE_FROM_NULL()					((Toy_Value){{ .integer = 0 }, TOY_VALUE_NULL})
#define TOY_VALUE_FROM_BOOLEAN(value)			((Toy_Value){{ .boolean = value }, TOY_VALUE_BOOLEAN})
#define TOY_VALUE_FROM_INTEGER(value)			((Toy_Value){{ .integer = value }, TOY_VALUE_INTEGER})
#define TOY_VALUE_FROM_FLOAT(value)				((Toy_Value){{ .number = value }, TOY_VALUE_FLOAT})
#define TOY_VALUE_FROM_STRING(value)			((Toy_Value){{ .string = value }, TOY_VALUE_STRING})
//TODO: more

//utilities
TOY_API unsigned int Toy_hashValue(Toy_Value value);

TOY_API Toy_Value Toy_copyValue(Toy_Value value);
TOY_API void Toy_freeValue(Toy_Value value);

TOY_API bool Toy_checkValueIsTruthy(Toy_Value value);
TOY_API bool Toy_checkValuesAreEqual(Toy_Value left, Toy_Value right);
TOY_API bool Toy_checkValuesAreComparable(Toy_Value left, Toy_Value right);
TOY_API int Toy_compareValues(Toy_Value left, Toy_Value right);
