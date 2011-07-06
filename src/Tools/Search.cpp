#include "Cassandra/IndexWriter.h"
#include "Cassandra/IndexReader.h"
#include "Cassandra/SimpleAnalyzer.h"
#include "Cassandra/TermFrequencyVector.h"

using namespace tiberius;
using namespace tiberius::index;
using namespace tiberius::analysis;

int main(int argc, char **argv){

    if(argc < 1){
        cout << "Usage: Searcher [term] " << endl;
        return 1;
    }
    string term(argv[1]);

    IndexReader reader;

    vector<string> docs;
    reader.getDocsForTerm(term, docs);



    return 0;
}

