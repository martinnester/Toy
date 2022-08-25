#pragma once

#include "literal.h"
#include "opcodes.h"
#include "token_types.h"

//nodes are the intermediaries between parsers and compilers
typedef union _node Node;

typedef enum NodeType {
	NODE_ERROR,
	NODE_LITERAL, //a simple value
	NODE_UNARY, //one child + opcode
	NODE_BINARY, //two children, left and right + opcode
	NODE_GROUPING, //one child
	NODE_BLOCK, //contains a sub-node array
	NODE_COMPOUND, //contains a sub-node array
	NODE_PAIR, //contains a left and right
	NODE_VAR_TYPES, //contains a type and a sub-node array for compound types
	NODE_VAR_DECL, //contains identifier literal, typenode, expression definition
	NODE_FN_DECL, //containd identifier literal, arguments node, returns node, block node
	NODE_FN_COLLECTION, //parts of a function
	NODE_PATH_IF, //for control flow
	NODE_PATH_WHILE, //for control flow
	NODE_PATH_FOR, //for control flow
	NODE_PATH_BREAK,
	NODE_PATH_CONTINUE,
	NODE_PATH_RETURN,
	NODE_INCREMENT_PREFIX,
	NODE_INCREMENT_POSTFIX,
} NodeType;

typedef struct NodeLiteral {
	NodeType type;
	Literal literal;
} NodeLiteral;

typedef struct NodeUnary {
	NodeType type;
	Opcode opcode;
	Node* child;
} NodeUnary;

typedef struct NodeBinary {
	NodeType type;
	Opcode opcode;
	Node* left;
	Node* right;
} NodeBinary;

typedef struct NodeGrouping {
	NodeType type;
	Node* child;
} NodeGrouping;

typedef struct NodeBlock {
	NodeType type;
	Node* nodes;
	int capacity;
	int count;
} NodeBlock;

typedef struct NodeCompound {
	NodeType type;
	LiteralType literalType;
	Node* nodes;
	int capacity;
	int count;
} NodeCompound;

typedef struct NodePair {
	NodeType type;
	Node* left;
	Node* right;
} NodePair;

typedef struct NodeVarTypes {
	NodeType type;
	Literal typeLiteral;
} NodeVarTypes;

typedef struct NodeVarDecl {
	NodeType type;
	Literal identifier;
	Literal typeLiteral;
	Node* expression;
} NodeVarDecl;

typedef struct NodeFnDecl {
	NodeType type;
	Literal identifier;
	Node* arguments;
	Node* returns;
	Node* block;
} NodeFnDecl;

typedef struct NodeFnCollection {
	NodeType type;
	Node* nodes;
	int capacity;
	int count;
} NodeFnCollection;

typedef struct NodePath {
	NodeType type;
	Node* preClause;
	Node* postClause;
	Node* condition;
	Node* thenPath;
	Node* elsePath;
} NodePath;

typedef struct NodeIncrement {
	NodeType type;
	Literal identifier;
	int increment;
} NodeIncrement;

union _node {
	NodeType type;
	NodeLiteral atomic;
	NodeUnary unary;
	NodeBinary binary;
	NodeGrouping grouping;
	NodeBlock block;
	NodeCompound compound;
	NodePair pair;
	NodeVarTypes varTypes;
	NodeVarDecl varDecl;
	NodeFnDecl fnDecl;
	NodeFnCollection fnCollection;
	NodePath path;
	NodeIncrement increment;
};

void freeNode(Node* node);
void emitNodeLiteral(Node** nodeHandle, Literal literal);
void emitNodeUnary(Node** nodeHandle, Opcode opcode);
void emitNodeBinary(Node** nodeHandle, Node* rhs, Opcode opcode);
void emitNodeGrouping(Node** nodeHandle);
void emitNodeBlock(Node** nodeHandle);
void emitNodeCompound(Node** nodeHandle, LiteralType literalType);
void emitNodePair(Node** nodeHandle, Node* left, Node* right);
void emitNodeVarTypes(Node** nodeHandle, Literal literal);
void emitNodeVarDecl(Node** nodeHandle, Literal identifier, Literal type, Node* expression);
void emitNodeFnDecl(Node** nodeHandle, Literal identifier, Node* arguments, Node* returns, Node* block);
void emitNodeFnCollection(Node** nodeHandle);
void emitNodePath(Node** nodeHandle, NodeType type, Node* preClause, Node* postClause, Node* condition, Node* thenPath, Node* elsePath);
void emiteNodePrefixIncrement(Node** nodeHandle, Literal identifier, int increment);
void emiteNodePostfixIncrement(Node** nodeHandle, Literal identifier, int increment);

void printNode(Node* node);
