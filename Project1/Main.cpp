/*
* File: main.cpp
*/

#include <cstdlib>
#include <iostream>
#include "Nodes.h"
#include "BinaryExpressionBuilder.h"
using namespace std;

int main(int argc, char** argv)
{

	NumericElementNode num1(10);
	NumericElementNode num2(20);
	BinaryOperationNode n('+', &num1, &num2);

	BinaryExpressionBuilder b;

	cout << "Enter expression" << endl;

	string expression;
	getline(cin, expression);

	BinaryOperationNode *root = b.parse(expression);

	cout << " result = " << root->value();
	cin.get();
	cin.get();
	return 0;
}