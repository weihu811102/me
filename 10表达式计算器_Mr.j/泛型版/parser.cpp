#include "parser.h"
#include "scanner.h"
#include "node.h"

#include <cassert>
#include <iostream>
#include <functional>

Parser::Parser(Scanner& scanner) : scanner_(scanner), tree_(0)
{
}

void Parser::Parse()
{
	tree_ = Expr();
}

Node* Parser::Expr()
{
	Node* node = Term();
	EToken token = scanner_.Token();

	if (token == TOKEN_PLUS)
	{
		scanner_.Accept();
		Node* nodeRight = Expr();
		node = new BinaryNode<std::plus<double> >(node, nodeRight);//���������STL�ĺ���������ʵ��
	}
	else if (token == TOKEN_MINUS)
	{
		scanner_.Accept();
		Node* nodeRight = Expr();
		node = new BinaryNode<std::minus<double> >(node, nodeRight);
	}

	return node;
}

Node* Parser::Term()
{
	Node* node = Factor();
	EToken token = scanner_.Token();

	if (token == TOKEN_MULTIPLY)
	{
		scanner_.Accept();
		Node* nodeRight = Term();
		node = new BinaryNode<std::multiplies<double> >(node, nodeRight);
	}
	else if (token == TOKEN_DIVIDE)
	{
		scanner_.Accept();
		Node* nodeRight = Term();
		node = new BinaryNode<std::divides<double> >(node, nodeRight);
	}

	return node;
}

Node* Parser::Factor()
{
	Node* node = 0;
	EToken token = scanner_.Token();
	if (token == TOKEN_LPARENTHESIS)
	{
		scanner_.Accept();		// accept '('
		node = Expr();
		if (scanner_.Token() == TOKEN_RPARENTHESIS)
		{
			scanner_.Accept();	// accept ')'
		}
		else
		{
			status_ = STATUS_ERROR;
			// Todo:�׳��쳣
			std::cout<<"missing parenthesis"<<std::endl;
			node = 0;
		}
	}
	else if (token == TOKEN_NUMBER)
	{
		node = new NumberNode(scanner_.Number());
		scanner_.Accept();
	}
	else if (token == TOKEN_MINUS)
	{
		scanner_.Accept();		// accept minus
		node = new UMinusNode(Factor());
	}
	else
	{
		status_ = STATUS_ERROR;
		// Todo:�׳��쳣
		std::cout<<"not a valid expression"<<std::endl;
		node = 0;
	}
	return node;
}

double Parser::Calculate() const
{
	//assert(tree_ != 0);
	return tree_->Calc();
}
