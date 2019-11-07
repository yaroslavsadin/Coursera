#include <map>
#include <iostream>
#include <string>
#include <vector>

// Serialization

template <typename T>
void Serialize(T pod, std::ostream& out) {
    out.write(reinterpret_cast<char*>(&pod),sizeof(pod));
}

void Serialize(const std::string& str, std::ostream& out) {
    size_t size = str.size();
    out.write(reinterpret_cast<char*>(&size),sizeof(size));
    out.write(str.c_str(),str.size());
}

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out) {
    size_t size = data.size();
    out.write(reinterpret_cast<char*>(&size),sizeof(size));
    for(const auto& item : data) {
        Serialize(item,out);
    }
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out) {
    size_t size = data.size();
    out.write(reinterpret_cast<char*>(&size),sizeof(size));
    for(const auto& [key,item] : data) {
        Serialize(key,out);
        Serialize(item,out);
    }
}


// Deserialization

template <typename T>
void Deserialize(std::istream& in, T& pod) {
    in.read(reinterpret_cast<char*>(&pod),sizeof(pod));
}

void Deserialize(std::istream& in, std::string& str) {
    size_t size {0};
    in.read(reinterpret_cast<char*>(&size),sizeof(size));
    str.reserve(size);
    while(size--) {
        char c;
        in.read(&c,1);
        str.push_back(c);
    }
}

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data) {
    size_t size {0};
    in.read(reinterpret_cast<char*>(&size),sizeof(size));
    data.reserve(size);
    while(size--) {
        T item;
        Deserialize(in,item);
        data.push_back(item);
    }
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data) {
    size_t size {0};
    in.read(reinterpret_cast<char*>(&size),sizeof(size));
    while(size--) {
        T1 key;
        Deserialize(in,key);
        T2 value;
        Deserialize(in,value);
        data[key] = value;
    }
}
