#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
using namespace std;

struct Block {
    unsigned long long W;
    unsigned long long H;
    unsigned long long D;
};

istream& operator>>(istream& stream,Block& block) {
    stream >> block.W >> block.H >> block.D;
    return stream;
}

int main(void) {
    unsigned int num_blocks;
    unsigned int R;
    cin >> num_blocks >> R;
    unsigned long long sum = 0;
    for(int i = num_blocks; i > 0; i--) {
        Block block;
        cin >> block;
        sum += R*block.D*block.H*block.W;
    }
    cout << sum << endl;
    return 0;
}