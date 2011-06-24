#include <iostream>
#include "TermFrequencyVector.h"
#include <string.h>
#include <sstream>

using namespace std;

tiberius::TermFrequencyVector::TermFrequencyVector() : count(0){
}

void tiberius::TermFrequencyVector::add(string term, int freq, vector<int> &pos){
    count++;
    terms.push_back(term);
    frequencies.push_back(freq);
    positions.push_back(pos);
}
