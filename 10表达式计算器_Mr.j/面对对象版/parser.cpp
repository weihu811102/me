#include <cassert>
#include <iostream>
#include <cstring>
#include "parser.h"
#include "scanner.h"
#include "node.h"
#include "Calc.h"
#include "Exception.h"
#include "DebugNew.h"

Parser::Parser(Scanner& scanner, Calc& calc) : scanner_(scanner), calc_(calc), tree_(0), status_(STATUS_OK)
{
}
Parser::~Parser()
{

}

STATUS Parser::Parse()
{
	tree_ = Expr();
	if(!scanner_.IsDone())//没有完全解析
	{
		status_ = STATUS_ERROR;
	}
	return status_;
}
//Expr:=Term('+' | '-')Term
std::auto_ptr<Node> Parser::Expr()
{
	std::auto_ptr<Node> node = Term();//1.

	EToken token = scanner_.Token();
	if(TOKEN_PLUS == token || TOKEN_MINUS == token)//2.
	{
	    std::auto_ptr<MultipleNode> multipleNode( new SumNode(node));
	    do
	    {
		//Expr:=Term('+' | '-')Term
		scanner_.Accept();
		std::auto_ptr<Node> nextNode = Term();//3.
		multipleNode->AppendChild(nextNode, (TOKEN_PLUS == token));
		token = scanner_.Token();
	    }while(TOKEN_PLUS == token || TOKEN_MINUS == token);
	    node = multipleNode;
	 }
	else if(TOKEN_ASSIGN == token)//4. it is assign(=) 遇到等号
	{
		//Expr := Term
		scanner_.Accept();
		std::auto_ptr<Node> nodeRight = Expr();
		if(node->IsLvalue())
		{
			node = std::auto_ptr<Node>(new AssignNode(node, nodeRight));
		}
		else
		{
			status_ = STATUS_ERROR;
			//TODO:throw ex
			throw SyntaxError("The left-hand side of an assignment must be a variable.");
		}
	}

	return node;
}

std::auto_ptr<Node> Parser::Term()
{
	std::auto_ptr<Node> node = Factor();
	EToken token = scanner_.Token();

	if(TOKEN_MULTIPLY == token || TOKEN_DIVIDE == token)
	{
	    std::auto_ptr<MultipleNode> multipleNode(new ProductNode(node));
	    do
	    {
		//Term:= factor('/' | '*')Term
		scanner_.Accept();
		std::auto_ptr<Node> nextNode = Factor();
		multipleNode->AppendChild(nextNode, (TOKEN_MULTIPLY == token));
		token = scanner_.Token();
	    }while(TOKEN_MULTIPLY == token || TOKEN_DIVIDE == token);
	    node = multipleNode;
	  }

	return node;
}

std::auto_ptr<Node> Parser::Factor()
{
	std::auto_ptr<Node> node;
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
			throw SyntaxError("Missing parenthesis.");
		}
	}
	else if (token == TOKEN_NUMBER)
	{
		node = std::auto_ptr<Node>(new NumberNode(scanner_.Number()));
		scanner_.Accept();
	}
	else if(TOKEN_IDENTIFIER == token)//identifier 标示符
	{
		std::string symbol = scanner_.GetSymbol();
		unsigned int id = calc_.FindSymbol(symbol);
		scanner_.Accept();

		if(TOKEN_LPARENTHESIS == scanner_.Token())
		{
			scanner_.Accept();//Accept '('
			node = Expr();
			if(TOKEN_RPARENTHESIS == scanner_.Token())
			{
				scanner_.Accept();//Accept ')'
				if(SymbolTable::IDNOTFOUND != id && calc_.IsFunction(id))
				{
					node = std::auto_ptr<Node>(new FunctionNode(node, calc_.GetFunction(id)));					
				}
				else
				{
					status_ = STATUS_ERROR;
					char buf[256] = {0};
					sprintf(buf, "Unknown funciton \"%s\".", symbol.c_str());//有警告 sprintf函数是不可移植的
					throw SyntaxError(buf);
				}
			}
			else
			{
				status_ = STATUS_ERROR;
				throw SyntaxError("Missing parenthesis in a function call.");
			}
		}
		else//
		{
			if(SymbolTable::IDNOTFOUND == id)
			{
				id = calc_.AddSymbol(symbol);
			}
			node = std::auto_ptr<Node>(new VariableNode(id, calc_.GetStorage()));
		}
	}
	else if (token == TOKEN_MINUS)//减号
	{
		scanner_.Accept();		// accept minus
		node = std::auto_ptr<Node>(new UMinusNode(Factor()));
	}
	else
	{
		status_ = STATUS_ERROR;
		throw SyntaxError("not a valid expression");
	}
	return node;
}

double Parser::Calculate() const
{
	assert(tree_.get() != 0);
	return tree_->Calc();
}
