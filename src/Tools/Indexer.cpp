#include "Cassandra/IndexWriter.h"
#include "Cassandra/IndexReader.h"
#include "Cassandra/SimpleAnalyzer.h"
#include "Cassandra/TermFrequencyVector.h"

using namespace tiberius;
using namespace tiberius::index;
using namespace tiberius::analysis;

int main(int argc, char **argv){

    if(argc < 2){
        cout << "Usage: Indexer [id] [text]" << endl;
        return 1;
    }
    string id(argv[1]);
    string text(argv[2]);
/*
    string text(" characters in pattern rules can be quoted with preceding backslashes (). Backslashes that would otherwise quote  characters can be quoted with more backslashes. Backslashes that quote  characters or other backslashes are removed from the pattern before it is compared to file names or has a stem substituted into it. Backslashes that are not in danger of quoting  characters go unmolested. For example, the pattern theweirdpattern has theweird preceding the operative  character, and pattern following it. The final two backslashes are left alone because they cannot affect any  character.");
    string id("doc1");
*/

    SimpleAnalyzer a;
    IndexWriter writer;
    writer.writeDocument(id, text, a);


/*
    TermFrequencyVector v;
    IndexReader reader;
    v = reader.getTermFrequencyVector(id);

    vector<string> docs;
    string term("pattern");
    reader.getDocsForTerm(term, docs);
    return 0;
    */
}

