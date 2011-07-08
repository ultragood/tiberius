#include "Cassandra/IndexWriter.h"
#include "Cassandra/IndexReader.h"
#include "Cassandra/SimpleAnalyzer.h"
#include "Cassandra/TermFrequencyVector.h"
#include "Cassandra/CassandraConnection.h"
#include "Constants.h"
#include "Cassandra/SimpleAnalyzer.h"
#include <math.h>

using namespace tiberius;
using namespace tiberius::index;
using namespace tiberius::analysis;

int docCount = 0;
double idf(int cdc, int tdc){
    return log(cdc/tdc);
}

double idf(IndexReader &reader, string &term){
    docCount = docCount == 0 ? reader.getDocCount() : docCount;
    return idf(docCount, reader.getDocFreqForTerm(term));

}
double idf(IndexReader &reader, int termDocs){
    docCount = docCount == 0 ? reader.getDocCount() : docCount;
    return idf(docCount, termDocs);

}

double cosine(map<string, double> &idfs, TermFrequencyVector &tv){
    double prod_sum        =0;
    double sum_of_squares1 =0;
    double sum_of_squares2 =0;

    IndexReader reader;

    map<string, int> counts;
    reader.getDocFreqsForTerms(tv.getTerms(), counts);

    // just being lazy right here
    for(unsigned int i=0; i < tv.getTerms().size(); i++){
        int freq = counts[tv.getTerms()[i]];
        if(freq == 0) continue;

        double tf = tv.getFrequencies()[i] / static_cast<double>(tv.getTokenCount());
        sum_of_squares1 += tv.getFrequencies()[i] * idf(reader, counts[tv.getTerms()[i]]) * tf; 

        map<string, double>::const_iterator it = idfs.find(tv.getTerms()[i]);
        if(it != idfs.end()){
            prod_sum += pow(2, idfs[tv.getTerms()[i]]); 
        }
    }
    if(sum_of_squares1 == 0) return 0;

    for(map<string, double>::const_iterator it = idfs.begin(); it != idfs.end(); ++it){
        sum_of_squares2 += pow(2, it->second);
    }
    if(sum_of_squares2 == 0) return 0;

    return prod_sum / (sqrt(sum_of_squares1) * sqrt(sum_of_squares2));

}

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

    TermFrequencyVector qv(terms.size(), terms);

    IndexReader reader;


    cout << endl;
    int corpus_size = reader.getDocCount();
    cout << " Total documents: " << corpus_size << endl;

    cout << endl;
    cout << "Doc Counts for terms " << endl;

    map<string, int32_t> counts;
    reader.getDocFreqsForTerms(terms, counts);
    map<string, double> idfs;
    double tf = 1 / terms.size();
    for(map<string, int>::iterator it= counts.begin(); it!= counts.end(); it++){
        cout << it->first << " : " << it->second << " " << idf(corpus_size, it->second) << endl;
        if(it->second == 0) continue;
        double idfval =  idf(corpus_size, it->second);
        idfs[it->first] = idfval * tf;
    }

    cout << "-- Candidates --" << endl;

    map<string, int> docs;
    reader.getDocsForTermList(terms, docs);
    for(map<string, int>::iterator it = docs.begin(); it!= docs.end(); it++){
        cout << it->first << " matches " << it->second << " terms " << endl;

        string docid(it->first);
        TermFrequencyVector tv = reader.getTermFrequencyVector(docid);
//        cout << tv.toString() << endl;
        cout << "cos: " << cosine(idfs, tv) << endl;
     
    }

    /*
    cout << endl << "-- Terms --" << endl;


    for(vector<string>::iterator it = terms.begin(); it!= terms.end(); it++){
        vector<DocResults> docs;
        reader.getDocsForTerm(*it, docs);
        cout << "-- Docs matching '" << *it << "' --" << endl;
        for(vector<DocResults>::iterator it = docs.begin(); it!= docs.end(); it++){
            cout << it->doc_id << " freq: " << it->frequency << endl;
        }
    }
    cout << endl;
    */



    return 0;
}

