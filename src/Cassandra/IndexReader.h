#pragma once

#include "TermFrequencyVector.h"
#include "DocResults.h"
#include <map>

using namespace std;
using namespace tiberius;

namespace tiberius{ namespace index {

    class IndexReader{
        public:
        IndexReader();
        TermFrequencyVector getTermFrequencyVector(string &docId);
        int getDocCount();
        void getDocsForTerm(string &term, vector<DocResults> &docs);
        void getDocFreqsForTerms(const vector<string> &terms, map<string, int32_t> &counts);
        
    };

} }
