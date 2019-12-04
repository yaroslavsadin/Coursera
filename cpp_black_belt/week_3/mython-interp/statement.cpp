#include "statement.h"
#include "object.h"
#include "misc.h"

#include <iostream>
#include <sstream>

using namespace std;

namespace Ast {

using Runtime::Closure;

ObjectHolder Assignment::Execute(Closure& closure) {
  closure[var_name] = right_value->Execute(closure);
  return closure[var_name];
}

Assignment::Assignment(std::string var, std::unique_ptr<Statement> rv)
: var_name(move(var)), right_value(move(rv))
{
}

VariableValue::VariableValue(std::string var_name) 
: dotted_ids{var_name}
{
}

VariableValue::VariableValue(std::vector<std::string> dotted_ids) 
: dotted_ids(move(dotted_ids))
{
}

ObjectHolder VariableValue::Execute(Closure& closure) {
  Closure final = closure;
  for(auto it = dotted_ids.begin(); it < dotted_ids.end(); it++) {
    auto id = *it;
    if(!final.count(id)) throw runtime_error("Variable not found");
    if(auto next = final.at(id).TryAs<Runtime::ClassInstance>(); next) {
      if(it == prev(dotted_ids.end())) {
        return final.at(id);
      } else { 
        final = next->Fields();
      }
    } else break;
  }
  return final.at(dotted_ids.back());
}

unique_ptr<Print> Print::Variable(std::string var) {
  return make_unique<Print>(make_unique<VariableValue>(var));
}

Print::Print(unique_ptr<Statement> argument)
// : args{move(argument)}  <<-- doesn't work?
{
  args.push_back(move(argument));
}

Print::Print(vector<unique_ptr<Statement>> args)
: args(move(args)) 
{
}

ObjectHolder Print::Execute(Closure& closure) {
  if(args.size() < 1) return {};
  ostream& stream = *output;
  for(const auto& arg : Range(args.begin(),prev(args.end()))) {
    if(auto ptr = arg->Execute(closure).Get(); ptr != nullptr) {
      ptr->Print(stream);
    } else {
      stream << "None";
    }
    stream << ' ';
  }
  if(auto ptr = args.back()->Execute(closure).Get(); ptr != nullptr) {
    ptr->Print(stream);
  } else {
    stream << "None";
  }
  stream << '\n';
  return {};
}

ostream* Print::output = &cout;

void Print::SetOutputStream(ostream& output_stream) {
  output = &output_stream;
}

MethodCall::MethodCall(
  std::unique_ptr<Statement> object
  , std::string method
  , std::vector<std::unique_ptr<Statement>> args
) : object(move(object)), method(move(method)), args(move(args))
{
}

ObjectHolder MethodCall::Execute(Closure& closure) {
  auto cls_holder = object->Execute(closure);
  auto cls = cls_holder.TryAs<Runtime::ClassInstance>();
  if(cls->HasMethod(method,args.size())) {
    vector<ObjectHolder> actual_args;
    actual_args.reserve(args.size());
    for(const auto& arg : args) {
      actual_args.push_back(arg->Execute(closure));
    }
    return cls->Call(method,actual_args);
  } else {
    throw runtime_error("No such method");
  }
}

ObjectHolder Stringify::Execute(Closure& closure) {
  stringstream ss;
  argument->Execute(closure)->Print(ss);
  return ObjectHolder::Own(Runtime::String(ss.str()));
}

ObjectHolder Add::Execute(Closure& closure) {
  using namespace Runtime;

  auto lhs_eval = lhs->Execute(closure);
  auto rhs_eval = rhs->Execute(closure);

  if(auto lhs_class = lhs_eval.TryAs<Runtime::ClassInstance>(); lhs_class) {
    return lhs_class->Call("__add__",{rhs_eval});
  }
  
  // Probably there's a better way...
  if(String* str_p1 = lhs_eval.TryAs<String>(); str_p1) {
    if(String* str_p2 = rhs_eval.TryAs<String>(); str_p2) {
      return ObjectHolder::Own(String(str_p1->GetValue() + str_p2->GetValue()));
    }
  } else if(auto bool_p1 = lhs_eval.TryAs<Bool>(); bool_p1) {
    if(auto bool_p2 = rhs_eval.TryAs<Bool>(); bool_p2) {
      return ObjectHolder::Own(Number(bool_p1->GetValue() + bool_p2->GetValue()));
    } else if(auto num_p2 = rhs_eval.TryAs<Number>(); num_p2) {
      return ObjectHolder::Own(Number(bool_p1->GetValue() + num_p2->GetValue()));
    }
  } else if(auto num_p1 = lhs_eval.TryAs<Number>(); num_p1) {
    if(auto bool_p2 = rhs_eval.TryAs<Bool>(); bool_p2) {
      return ObjectHolder::Own(Number(num_p1->GetValue() + bool_p2->GetValue()));
    } else if(auto num_p2 = rhs_eval.TryAs<Number>(); num_p2) {
      return ObjectHolder::Own(Number(num_p1->GetValue() + num_p2->GetValue()));
    }
  }
  throw runtime_error("Bad add");
}

ObjectHolder Sub::Execute(Closure& closure) {
  using namespace Runtime;

  auto lhs_eval = lhs->Execute(closure);
  auto rhs_eval = rhs->Execute(closure);
  
  if(auto bool_p1 = lhs_eval.TryAs<Bool>(); bool_p1) {
    if(auto bool_p2 = rhs_eval.TryAs<Bool>(); bool_p2) {
      return ObjectHolder::Own(Number(bool_p1->GetValue() - bool_p2->GetValue()));
    } else if(auto num_p2 = rhs_eval.TryAs<Number>(); num_p2) {
      return ObjectHolder::Own(Number(bool_p1->GetValue() - num_p2->GetValue()));
    }
  } else if(auto num_p1 = lhs_eval.TryAs<Number>(); num_p1) {
    if(auto bool_p2 = rhs_eval.TryAs<Bool>(); bool_p2) {
      return ObjectHolder::Own(Number(num_p1->GetValue() - bool_p2->GetValue()));
    } else if(auto num_p2 = rhs_eval.TryAs<Number>(); num_p2) {
      return ObjectHolder::Own(Number(num_p1->GetValue() - num_p2->GetValue()));
    }
  }
  throw runtime_error("Bad sub");
}

ObjectHolder Mult::Execute(Runtime::Closure& closure) {
  using namespace Runtime;

  auto lhs_eval = lhs->Execute(closure);
  auto rhs_eval = rhs->Execute(closure);
  
  if(auto bool_p1 = lhs_eval.TryAs<Bool>(); bool_p1) {
    if(auto bool_p2 = rhs_eval.TryAs<Bool>(); bool_p2) {
      return ObjectHolder::Own(Number(bool_p1->GetValue() * bool_p2->GetValue()));
    } else if(auto num_p2 = rhs_eval.TryAs<Number>(); num_p2) {
      return ObjectHolder::Own(Number(bool_p1->GetValue() * num_p2->GetValue()));
    }
  } else if(auto num_p1 = lhs_eval.TryAs<Number>(); num_p1) {
    if(auto bool_p2 = rhs_eval.TryAs<Bool>(); bool_p2) {
      return ObjectHolder::Own(Number(num_p1->GetValue() * bool_p2->GetValue()));
    } else if(auto num_p2 = rhs_eval.TryAs<Number>(); num_p2) {
      return ObjectHolder::Own(Number(num_p1->GetValue() * num_p2->GetValue()));
    }
  }
  throw runtime_error("Bad mult");
}

ObjectHolder Div::Execute(Runtime::Closure& closure) {
  using namespace Runtime;

  auto lhs_eval = lhs->Execute(closure);
  auto rhs_eval = rhs->Execute(closure);
  
  if(auto bool_p1 = lhs_eval.TryAs<Bool>(); bool_p1) {
    if(auto bool_p2 = rhs_eval.TryAs<Bool>(); bool_p2) {
      return ObjectHolder::Own(Number(bool_p1->GetValue() / bool_p2->GetValue()));
    } else if(auto num_p2 = rhs_eval.TryAs<Number>(); num_p2) {
      return ObjectHolder::Own(Number(bool_p1->GetValue() / num_p2->GetValue()));
    }
  } else if(auto num_p1 = lhs_eval.TryAs<Number>(); num_p1) {
    if(auto bool_p2 = rhs_eval.TryAs<Bool>(); bool_p2) {
      return ObjectHolder::Own(Number(num_p1->GetValue() / bool_p2->GetValue()));
    } else if(auto num_p2 = rhs_eval.TryAs<Number>(); num_p2) {
      return ObjectHolder::Own(Number(num_p1->GetValue() / num_p2->GetValue()));
    }
  }
  throw runtime_error("Bad div");
}

ObjectHolder Compound::Execute(Closure& closure) {
  for(const auto& stmnt : statements) {
    stmnt->Execute(closure);
  }
  return {};
}

ObjectHolder Return::Execute(Closure& closure) {
  return statement->Execute(closure);
}

ClassDefinition::ClassDefinition(ObjectHolder class_) 
: cls(class_), class_name(cls.TryAs<Runtime::Class>()->GetName())
{
}

ObjectHolder ClassDefinition::Execute(Runtime::Closure& closure) {
  return cls;
}

FieldAssignment::FieldAssignment(
  VariableValue object, std::string field_name, std::unique_ptr<Statement> rv
)
  : object(std::move(object))
  , field_name(std::move(field_name))
  , right_value(std::move(rv))
{
}

ObjectHolder FieldAssignment::Execute(Runtime::Closure& closure) {
  ObjectHolder instance = object.Execute(closure);
  Runtime::Closure& fields = instance.TryAs<Runtime::ClassInstance>()->Fields();
  return fields[field_name] = right_value->Execute(closure);
}

IfElse::IfElse(
  std::unique_ptr<Statement> condition,
  std::unique_ptr<Statement> if_body,
  std::unique_ptr<Statement> else_body
) : condition(move(condition)), if_body(move(if_body)), else_body(move(else_body))
{
}

ObjectHolder IfElse::Execute(Runtime::Closure& closure) {
  if(condition->Execute(closure)) {
    return if_body->Execute(closure);
  } else {
    return else_body->Execute(closure);
  }
}

ObjectHolder Or::Execute(Runtime::Closure& closure) {
}

ObjectHolder And::Execute(Runtime::Closure& closure) {
}

ObjectHolder Not::Execute(Runtime::Closure& closure) {
}

Comparison::Comparison(
  Comparator cmp, unique_ptr<Statement> lhs, unique_ptr<Statement> rhs
) {
}

ObjectHolder Comparison::Execute(Runtime::Closure& closure) {
}

NewInstance::NewInstance(
  const Runtime::Class& class_, std::vector<std::unique_ptr<Statement>> args
)
  : class_(class_)
  , args(std::move(args))
{
}

NewInstance::NewInstance(const Runtime::Class& class_) : NewInstance(class_, {}) {
}

ObjectHolder NewInstance::Execute(Runtime::Closure& closure) {
  // How to deal with args ???
  return ObjectHolder::Own(Runtime::ClassInstance(class_));
}


} /* namespace Ast */
