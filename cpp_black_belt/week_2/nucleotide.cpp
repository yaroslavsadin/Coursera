#include <cstddef>
#include <array>
#include <unordered_map>
#if 0
#include "nucleotide.h"
#endif

#if 1
struct Nucleotide {
  char Symbol;
  size_t Position;
  int ChromosomeNum;
  int GeneNum;
  bool IsMarked;
  char ServiceInfo;
};

struct CompactNucleotide {
  uint64_t Symbol:2;
  uint64_t Position:32;
  uint64_t ChromosomeNum:6;
  uint64_t GeneNum:15;
  uint64_t IsMarked:1;
  uint64_t ServiceInfo:8;
};
#endif

std::array<char,4> to_symbol =
{
  'A', 'T', 'G', 'C'
};

std::unordered_map<char,uint64_t> to_index =
{
  {'A',0}, 
  {'T',1}, 
  {'G',2}, 
  {'C',3}
};


bool operator == (const Nucleotide& lhs, const Nucleotide& rhs) {
  return (lhs.Symbol == rhs.Symbol)
      && (lhs.Position == rhs.Position)
      && (lhs.ChromosomeNum == rhs.ChromosomeNum)
      && (lhs.GeneNum == rhs.GeneNum)
      && (lhs.IsMarked == rhs.IsMarked)
      && (lhs.ServiceInfo == rhs.ServiceInfo);
}


CompactNucleotide Compress(const Nucleotide& n) {
  return {
    to_index.at(n.Symbol),
    static_cast<uint64_t>(n.Position),
    static_cast<uint64_t>(n.ChromosomeNum),
    static_cast<uint64_t>(n.GeneNum),
    static_cast<uint64_t>(n.IsMarked),
    static_cast<uint64_t>(n.ServiceInfo)
  };
};


Nucleotide Decompress(const CompactNucleotide& cn) {
  return {
    to_symbol[static_cast<char>(cn.Symbol)],
    static_cast<size_t>(cn.Position),
    static_cast<int>(cn.ChromosomeNum),
    static_cast<int>(cn.GeneNum),
    static_cast<bool>(cn.IsMarked),
    static_cast<char>(cn.ServiceInfo)
  };
}
