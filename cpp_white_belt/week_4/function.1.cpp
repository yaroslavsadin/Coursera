#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class FunctionPart {
public:
  FunctionPart(char op,double value) : op(op), value(value) {}
  double Apply(double a) const {
    switch(op) {
      case '+': return value + a; 
      case '-': return a - value;
      case '*': return a * value;
      case '/': return a / value;
      default:
        return 42.0;
    }
  }
  void Invert() {
    switch(op) {
      case '+': op = '-'; break; 
      case '-': op = '+'; break;
      case '*': op = '/'; break;
      case '/': op = '*'; break;
    }
  }
private:
  char op;
  double value;
};

class Function {
public:
  void AddPart(char op,double value) {
    parts.push_back({op,value});
  }
  double Apply(double value) const {
    for(const auto& part : parts) {
      value = part.Apply(value);
    }
    return value;
  }
  void Invert() {
    for(auto& part : parts) {
      part.Invert();
    }
    reverse(begin(parts),end(parts));
  }
private:
  vector<FunctionPart> parts;
};
#if 0
struct Image {
  double quality;
  double freshness;
  double rating;
};

struct Params {
  double a;
  double b;
  double c;
};

Function MakeWeightFunction(const Params& params,
                            const Image& image) {
  Function function;
  function.AddPart('*', params.a);
  function.AddPart('-', image.freshness * params.b);
  function.AddPart('+', image.rating * params.c);
  return function;
}

double ComputeImageWeight(const Params& params, const Image& image) {
  Function function = MakeWeightFunction(params, image);
  return function.Apply(image.quality);
}

double ComputeQualityByWeight(const Params& params,
                              const Image& image,
                              double weight) {
  Function function = MakeWeightFunction(params, image);
  function.Invert();
  return function.Apply(weight);
}

int main() {
  Image image = {10, 2, 6};
  Params params = {4, 2, 6};
  cout << ComputeImageWeight(params, image) << endl;
  cout << ComputeQualityByWeight(params, image, 52) << endl;
  return 0;
}
#endif