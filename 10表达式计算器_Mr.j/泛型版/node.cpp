#include "node.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <cassert>


double NumberNode::Calc() const
{
	return number_;
}

UnaryNode::~UnaryNode()
{
	delete child_;
}

double UMinusNode::Calc() const
{
	return - child_->Calc();
}
MultipleNode::~MultipleNode()
{
  std::vector<Node*>::const_iterator it;
  for(it=childs_.begin(); it != childs_.end(); ++it)
  {
     delete *it;
  }
}

double SumNode::Calc() const
{
  double result = 0.0;
  std::vector<Node*>::const_iterator childIt = childs_.begin();
  std::vector<bool>::const_iterator positiveIt = positives_.begin();

  for (; childIt != childs_.end(); ++childIt, ++positiveIt)
    {
      assert(positiveIt != positives_.end());
      double val = (*childIt)->Calc();
      if (*positiveIt)
        result += val;
      else
        result -= val;
    }
      assert(positiveIt == positives_.end());

          return result;
}
double ProductNode::Calc() const
{
  double result = 1.0;
  std::vector<Node*>::const_iterator childIt = childs_.begin();
  std::vector<bool>::const_iterator positiveIt = positives_.begin();

  for (; childIt != childs_.end(); ++childIt, ++positiveIt)
    {
      assert(positiveIt != positives_.end());
      double val = (*childIt)->Calc();
      if (*positiveIt)
        result *= val;
      else if(0.0 != val)
        result /= val;
      else
        {
          std::cout<<"Division by zero\n"<<std::endl;
          return HUGE_VAL;
        }
    }
      assert(positiveIt == positives_.end());

      return result;
}

