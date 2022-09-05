#pragma once

#include "literal.h"
#include "literal_array.h"
#include "literal_dictionary.h"
#include "scope.h"

typedef void (*PrintFn)(const char*);

//the interpreter acts depending on the bytecode instructions
typedef struct Interpreter {
	//input
	unsigned char* bytecode;
	int length;
	int count;
	int codeStart; //BUGFIX: for jumps, must be initialized to -1
	LiteralArray literalCache; //read-only - built from the bytecode, refreshed each time new bytecode is provided

	//operation
	Scope* scope;
	LiteralArray stack;

	//output
	LiteralDictionary* exports; //read-write - interface with Toy from C - this is a pointer, since it works at a script-level
	LiteralDictionary* exportTypes;
	PrintFn printOutput;
	PrintFn assertOutput;
	PrintFn errorOutput;

	bool panic;
} Interpreter;

//native function API
typedef int (*NativeFn)(Interpreter* interpreter, LiteralArray* arguments);
bool injectNativeFn(Interpreter* interpreter, char* name, NativeFn func);
//TODO: injectNativeHook

//utilities for the host program
bool parseIdentifierToValue(Interpreter* interpreter, Literal* literalPtr);
void setInterpreterPrint(Interpreter* interpreter, PrintFn printOutput);
void setInterpreterAssert(Interpreter* interpreter, PrintFn assertOutput);
void setInterpreterError(Interpreter* interpreter, PrintFn errorOutput);

//main access
void initInterpreter(Interpreter* interpreter); //start of program
void runInterpreter(Interpreter* interpreter, unsigned char* bytecode, int length); //run the code
void resetInterpreter(Interpreter* interpreter); //use this to reset the interpreter's environment between runs
void freeInterpreter(Interpreter* interpreter); //end of program
