#pragma once
#include <cstdint>
#include <cstddef>

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
  uint64_t GeneNum:16;
  uint64_t IsMarked:1;
  uint64_t ServiceInfo:8;
};

CompactNucleotide Compress(const Nucleotide& n);
Nucleotide Decompress(const CompactNucleotide& cn);
bool operator == (const Nucleotide& lhs, const Nucleotide& rhs);