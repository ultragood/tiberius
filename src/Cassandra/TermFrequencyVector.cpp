#include <iostream>
#include "TermFrequencyVector.h"
#include <string.h>
#include <sstream>

using namespace std;

tiberius::TermFrequencyVector::TermFrequencyVector() : count(0){
}

string tiberius::TermFrequencyVector::toBytes() {

    ostringstream ss;
    boost::archive::text_oarchive oa(ss);
    oa << *this;
    return ss.str();
}

void tiberius::TermFrequencyVector::read(string &bytes){
    cout << bytes << endl;
    istringstream is(bytes);
    boost::archive::text_iarchive ia(is);
    ia >> *this;
}

void tiberius::TermFrequencyVector::add(string term, int freq, vector<int> &pos){
    count++;
    terms.push_back(term);
    frequencies.push_back(freq);
    positions.push_back(pos);
}
