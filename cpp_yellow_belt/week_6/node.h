#pragma once
#include "date.h"
#include <memory>

enum class LogicalOperation {
    And, 
    Or
};

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual,
};

class Node {
public:
    Node() {}
    virtual bool Evaluate(const Date& d, const string& s) const = 0;
private:

};

class EmptyNode : public Node {
public:
    inline bool Evaluate(const Date& d, const string& s) const override { 
        return true;
    }
private:
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison& cmp, const Date& d) : cmp_(cmp),d_(d) {}
    bool Evaluate(const Date& d, const string& s) const override;
private:
    Comparison cmp_;
    Date d_;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(const Comparison& cmp, const string& e) : cmp_(cmp),e_(e) {}
    bool Evaluate(const Date& d, const string& s) const override;
private:
    Comparison cmp_;
    string e_;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(const LogicalOperation& logical_operation, 
                                shared_ptr<Node> left, 
                                shared_ptr<Node> right) : logical_operation_(logical_operation), 
                                                            left_(left), right_(right) {}
    bool Evaluate(const Date& d, const string& s) const override;
private:
    LogicalOperation logical_operation_;
    shared_ptr<Node> left_, right_;
}; 