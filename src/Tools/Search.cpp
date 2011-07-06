#include "Cassandra/IndexWriter.h"
#include "Cassandra/IndexReader.h"
#include "Cassandra/SimpleAnalyzer.h"
#include "Cassandra/TermFrequencyVector.h"
#include "Cassandra/CassandraConnection.h"
#include "Constants.h"
#include "Cassandra/SimpleAnalyzer.h"

using namespace tiberius;
using namespace tiberius::index;
using namespace tiberius::analysis;

int main(int argc, char **argv){

    if(argc < 1){
        cout << "Usage: Searcher [terms] " << endl;
        return 1;
    }
    string text(argv[1]);

    SimpleAnalyzer a;
    streams::TokenStream *ts = a.getTokenStream(text);
    Token token;
    vector<string> terms;
    while(ts->next(token)){
        terms.push_back(token);
    }

    IndexReader reader;

    for(vector<string>::iterator it = terms.begin(); it!= terms.end(); it++){
        vector<DocResults> docs;
        reader.getDocsForTerm(*it, docs);
        cout << "-- Docs matching '" << *it << "' --" << endl;
        for(vector<DocResults>::iterator it = docs.begin(); it!= docs.end(); it++){
            cout << it->doc_id << " : " << it->frequency << endl;
        }
    }
    

    cout << endl;

    cout << "Doc Counts for terms" << endl;

    map<string, int32_t> counts;
    reader.getDocFreqsForTerms(terms, counts);
    for(map<string, int>::iterator it= counts.begin(); it!= counts.end(); it++){
        cout << it->first << " : " << it->second << endl;
    }

    cout << " Total documents: " << reader.getDocCount() << endl;

    return 0;
}

