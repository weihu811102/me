#ifndef _NODE_H_
#define _NODE_H_

#include <vector>

class Noncopyable
{
protected:
	Noncopyable() {}
	~Noncopyable() {}
private:
	Noncopyable(const Noncopyable&);
	const Noncopyable& operator=(const Noncopyable&);
};


class Node : private Noncopyable
{
public:
	virtual double Calc() const = 0;
	virtual ~Node() {}
};

class NumberNode : public Node
{
public:
	NumberNode(double number) : number_(number) {}
	double Calc() const;
private:
	const double number_;
};

template <typename T>
class BinaryNode : public Node
{
public:
	BinaryNode(Node* left, Node* right)
		: left_(left), right_(right) {}
	~BinaryNode()
	{
		delete left_;
		delete right_;
	}
	double Calc()const
	{
		T op;
		return op(left_->Calc(), right_->Calc());
	}
protected:
	Node* const left_;
	Node* const right_;
};

class UnaryNode : public Node
{
public:
	UnaryNode(Node* child)
		: child_(child) {}
	~UnaryNode();
protected:
	Node* const child_;
};

class UMinusNode : public UnaryNode
{
public:
	UMinusNode(Node* child)
		: UnaryNode(child) {}
	double Calc() const;
};
class MultipleNode : public Node
{
public:
	MultipleNode(Node* node)
	{
	  AppendChild(node, true);
	}
	void AppendChild(Node* node, bool positive)
	{
	  childs_.push_back(node);
	  positives_.push_back(positive);
	}
	~MultipleNode();
protected:
  std::vector<Node*> childs_;
  std::vector<bool>  positives_;
};
class SumNode : public MultipleNode
{
public:
  SumNode(Node* node) : MultipleNode(node){}
  double Calc() const;
};
class ProductNode : public MultipleNode
{
public:
  ProductNode(Node* node) : MultipleNode(node){}
  double Calc() const;
};
#endif // _NODE_H_
