#include <iostream>
#include "TermFrequencyVector.h"
#include <string.h>
#include <sstream>

using namespace std;

tiberius::TermFrequencyVector::TermFrequencyVector() : token_count(0){
}

void tiberius::TermFrequencyVector::add(string term, unsigned int freq, vector<unsigned int> pos){
    token_count++;
    terms.push_back(term);
    frequencies.push_back(freq);
    positions.push_back(pos);
}

