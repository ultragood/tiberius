#pragma once

#include "TermFrequencyVector.h"

using namespace std;
using namespace tiberius;

namespace tiberius{ namespace index {

    class IndexReader{
        public:
        IndexReader();
        TermFrequencyVector getTermFrequencyVector(string &docId);
        void getDocsForTerm(string &term, vector<string> &docs);
        
    };

} }
