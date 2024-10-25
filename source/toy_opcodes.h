#pragma once

typedef enum Toy_OpcodeType {
	//variable instructions
	TOY_OPCODE_READ,
	TOY_OPCODE_DECLARE,
	TOY_OPCODE_ASSIGN,
	TOY_OPCODE_ACCESS,

	TOY_OPCODE_DUPLICATE, //duplicate the top of the stack

	//arithmetic instructions
	TOY_OPCODE_ADD,
	TOY_OPCODE_SUBTRACT,
	TOY_OPCODE_MULTIPLY,
	TOY_OPCODE_DIVIDE,
	TOY_OPCODE_MODULO,

	//comparison instructions
	TOY_OPCODE_COMPARE_EQUAL,
	// TOY_OPCODE_COMPARE_NOT, //NOTE: optimized into a composite of TOY_OPCODE_COMPARE_EQUAL + TOY_OPCODE_NEGATE
	TOY_OPCODE_COMPARE_LESS,
	TOY_OPCODE_COMPARE_LESS_EQUAL,
	TOY_OPCODE_COMPARE_GREATER,
	TOY_OPCODE_COMPARE_GREATER_EQUAL,

	//logical instructions
	TOY_OPCODE_AND,
	TOY_OPCODE_OR,
	TOY_OPCODE_TRUTHY,
	TOY_OPCODE_NEGATE,

	//control instructions
	TOY_OPCODE_RETURN,

	//various action instructions
	TOY_OPCODE_PRINT,
	TOY_OPCODE_CONCAT,
	//TODO: clear the program stack?

	//meta instructions
	TOY_OPCODE_PASS,
	TOY_OPCODE_ERROR,
	TOY_OPCODE_EOF = 255,
} Toy_OpcodeType;

