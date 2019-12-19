#include "object.h"
#include "statement.h"

#include <sstream>
#include <string_view>

using namespace std;

namespace Runtime {

void ClassInstance::Print(std::ostream& os) {
    if(this->HasMethod("__str__",0)) {    
        this->Call("__str__",{})->Print(os);
    } else {
        os << std::hex << this;
    }
}

bool ClassInstance::HasMethod(const std::string& method, size_t argument_count) const {
    auto vptr = cls.GetVptr();
    auto it = vptr->find(method);
    if(it != vptr->end()) {
        if(it->second->formal_params.size() == argument_count) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

const Closure& ClassInstance::Fields() const {
    return fields;
}

Closure& ClassInstance::Fields() {
    return fields;
}

ClassInstance::ClassInstance(const Class& cls)
// TODO: Check why if I share this object here directly to the "fields" instead of line 54
// VariableValue::Execute fails with "self"
: cls(cls), fields()
{
}

ObjectHolder ClassInstance::Call(const std::string& method, const std::vector<ObjectHolder>& actual_args) {
    if(this->HasMethod(method,actual_args.size())) {
        const Method* method_p = cls.GetMethod(method);
        const vector<string>& formal = method_p->formal_params;
        Runtime::Closure method_closure = this->Fields();
        for(size_t i = 0; i < formal.size(); i++) {
            method_closure[formal[i]] = actual_args[i];
        }
        method_closure["self"] = ObjectHolder::Share(*this);
        
        // To work around the various tests, tak either body statement's return value
        // Or something Return would give back...
        auto ret = method_p->body->Execute(method_closure);
        if(method_closure.count("*ret")) {
            return method_closure.at("*ret");
        } else {
            return move(ret);
        }
    } else {
        throw runtime_error("No such method");
    }
}

Class::Class(std::string name, std::vector<Method> methods, const Class* parent) 
: name(move(name)), methods(move(methods)), vtable((parent != nullptr) ? parent->vtable : Vtable())
{
    for(auto& method : this->methods) {
        vtable[method.name] = &method;
    }
}

const Method* Class::GetMethod(const std::string& name) const {
    if(auto it = vtable.find(name); it != vtable.end()) 
        return it->second;
    else
        return nullptr;
}

void Class::Print(ostream& os) {
    /// ???
}

const std::string& Class::GetName() const {
    return this->name;
}

void Bool::Print(std::ostream& os) {
    if(GetValue()) {
        os << "True";
    } else {
        os << "False";
    }
}

} /* namespace Runtime */
