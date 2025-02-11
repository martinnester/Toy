#pragma once

#include "toy_common.h"

#include "toy_bucket.h"
#include "toy_stack.h"
#include "toy_scope.h"

typedef struct Toy_VM {
	//hold the raw bytecode
	unsigned char* bc;

	//raw instructions to be executed
	unsigned char* routine;
	unsigned int routineSize;

	unsigned int paramSize;
	unsigned int jumpsSize;
	unsigned int dataSize;
	unsigned int subsSize;

	unsigned int paramAddr;
	unsigned int codeAddr;
	unsigned int jumpsAddr;
	unsigned int dataAddr;
	unsigned int subsAddr;

	unsigned int routineCounter;

	//stack - immediate-level values only
	Toy_Stack* stack;

	//scope - block-level key/value pairs
	Toy_Scope* scope;

	//easy access to memory
	Toy_Bucket* stringBucket; //stores the string literals
	Toy_Bucket* scopeBucket; //stores the scopes

	//TODO: panic flag
} Toy_VM;

TOY_API void Toy_initVM(Toy_VM* vm);
TOY_API void Toy_bindVM(Toy_VM* vm, unsigned char* bytecode); //process the version data
TOY_API void Toy_bindVMToRoutine(Toy_VM* vm, unsigned char* routine); //process the routine only

TOY_API void Toy_runVM(Toy_VM* vm);
TOY_API void Toy_freeVM(Toy_VM* vm);

TOY_API void Toy_resetVM(Toy_VM* vm); //prepares for another run without deleting stack, scope and memory

//TODO: inject extra data (hook system for external libraries)
