#include "date.h"
#include "node.h"
using namespace std;

template <class T>
bool Compare(const T& lhs, const T& rhs, Comparison cmp) {
    switch(cmp) {
    case Comparison::Less :
        return lhs < rhs;
        break;
    case Comparison::LessOrEqual :
        return lhs <= rhs;
        break;
    case Comparison::Greater :
        return lhs > rhs;
        break;
    case Comparison::GreaterOrEqual :
        return lhs >= rhs;
        break;
    case Comparison::Equal :
        return lhs == rhs;
        break;
    case Comparison::NotEqual :
        return lhs != rhs; 
        break;
    }
}

bool DateComparisonNode::Evaluate(const Date& d, const string& s) const {
    return Compare(d, d_, cmp_);
}

bool EventComparisonNode::Evaluate(const Date& d, const string& s) const {
    return Compare(s, e_, cmp_);
}

bool LogicalOperationNode::Evaluate(const Date& d, const string& s) const {
    bool l = left_->Evaluate(d,s);
    bool r = right_->Evaluate(d,s);
    switch (logical_operation_) {
    case LogicalOperation::And :
        return (l && r);
        break;
    case LogicalOperation::Or :
        return (l || r);
        break;    
    }
}