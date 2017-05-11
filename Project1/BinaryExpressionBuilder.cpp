#include "BinaryExpressionBuilder.h"
#include "Nodes.h"
#include <sstream>

using namespace std;

int BinaryExpressionBuilder::precedence(char op)
{
	enum { lowest, mid, highest };

	switch (op) {

	case '+':
	case '-':
		return mid;

	case '/':
	case '*':
		return highest;

	default:
		return lowest;
	}
}

// Input: +, -, /, or *
// creates BinaryOperationNode's from all preceding
BinaryOperationNode *BinaryExpressionBuilder::parse(std::string& str) throw(NotWellFormed)
{
	istringstream istr(str);
	char token;

	while (istr >> token) {

		switch (token) {

		case '+':
		case '-':
		case '*':
		case '/':
			processOperator(token);
			break;

		case ')':
			processRightParenthesis();
			break;

		case '(':
			operatorStack.push(token);
			break;

		default:
			// If it is not an operator, it must be a number.
			// Since token is only a char in width, we put it back,
			// and get the complete number as a double.
			istr.putback(token);
			double number;

			istr >> number;

			NumericElementNode *newNode = new NumericElementNode(number);
			operandStack.push(newNode);

			continue;
		} // end switch
	}    // end while

	while (!operatorStack.empty()) {

		doBinary(operatorStack.top());
		operatorStack.pop();
	}

	// Invariant: At this point the operandStack should have only one element
	//     operandStack.size() == 1
	// otherwise, the expression is not well formed.
	if (operandStack.size() != 1) {

		throw NotWellFormed();
	}

	ExpressionElementNode *p = operandStack.top();

	return static_cast<BinaryOperationNode *>(p);
}

void BinaryExpressionBuilder::processOperator(char op)
{
	// pop operators with higher precedence and create their BinaryOperationNode
	int opPrecedence = precedence(op);

	while ((!operatorStack.empty()) && (opPrecedence <= precedence(operatorStack.top()))) {

		doBinary(operatorStack.top());
		operatorStack.pop();
	}

	// lastly push the operator passed onto the operatorStack
	operatorStack.push(op);
}

void BinaryExpressionBuilder::processRightParenthesis()
{
	while (!operatorStack.empty() && operatorStack.top() != '(') {

		doBinary(operatorStack.top());
		operatorStack.pop();
	}

	operatorStack.pop(); // remove '('
}

// Creates a BinaryOperationNode from the top two operands on operandStack
// These top two operands are removed (poped), and the new BinaryOperation
// takes their place on the top of the stack.
void BinaryExpressionBuilder::doBinary(char op)
{
	ExpressionElementNode *right = operandStack.top();

	operandStack.pop();

	ExpressionElementNode *left = operandStack.top();

	operandStack.pop();

	BinaryOperationNode *p = new BinaryOperationNode(operatorStack.top(), left, right);

	operandStack.push(p);
}