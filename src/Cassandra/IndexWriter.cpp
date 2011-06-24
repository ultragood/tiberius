
#include "IndexWriter.h"
#include "Streams/TokenStream.h"
#include "TermFrequencyVector.h"
#include <boost/unordered_map.hpp>
#include "Serializer.h"
#include "CassandraConnection.h"

#include "Constants.h"

using namespace tiberius::index;
using namespace tiberius::analysis;

const string IndexWriter::FREQ_KEY = "freq";
const string IndexWriter::POS_KEY = "pos";

IndexWriter::IndexWriter(){
    // make sure the connection is open
    CassandraConnection::instance().open();
}

void IndexWriter::writeDocument(string &doc_id, string &text, Analyzer &analyzer){

    streams::TokenStream * ts = analyzer.getTokenStream(text);

    // Assemble all the data we're collecting from the stream in temporary map
    int pos(0);
    Token token;
    boost::unordered_map<Token, boost::unordered_map<string, vector<unsigned int> > > tokenInfo;
    while(ts->next(token)){
        tokenInfo[token][POS_KEY].push_back(++pos);
        if(tokenInfo[token][FREQ_KEY].size() == 0){
            tokenInfo[token][FREQ_KEY].push_back(0);
        }
        tokenInfo[token][FREQ_KEY][0]++;
    }

    // Write it out to our real data structure
    TermFrequencyVector doc;
    for(boost::unordered_map<Token, boost::unordered_map<string, vector<unsigned int> > >::iterator it = tokenInfo.begin(); it!= tokenInfo.end(); it++){
        Token term(it->first);
        unsigned int freq = 0;
        if(it->second[FREQ_KEY].size() > 0)   
            freq = it->second[FREQ_KEY].at(0);

        vector<unsigned int> positions = it->second[POS_KEY];
        doc.add(term, freq, positions);

        // write to super column with key = term, super_column = doc_id
        // lucene uses index/field/term
        string key(term);
        string super_column(doc_id);
        CassandraConnection::instance().insert(key, TERMINFO_COLUMN_FAMILY, COLUMN_KEY_POSITIONS, serialize(positions), super_column);
        CassandraConnection::instance().insert(key, TERMINFO_COLUMN_FAMILY, COLUMN_KEY_FREQ, serialize(freq), super_column);

    }


    // write to 1 row for document:
    
    string key(doc_id);
    // Serialize the data strucutre into the db
    CassandraConnection::instance().insert(doc_id, DOCUMENT_COLUMN_FAMILY, COLUMN_KEY_TERMS, serialize(doc.getTerms()));
    CassandraConnection::instance().insert(doc_id, DOCUMENT_COLUMN_FAMILY, COLUMN_KEY_POSITIONS, serialize(doc.getPositions()));
    CassandraConnection::instance().insert(doc_id, DOCUMENT_COLUMN_FAMILY, COLUMN_KEY_FREQ, serialize(doc.getFrequencies()));
    CassandraConnection::instance().insert(doc_id, DOCUMENT_COLUMN_FAMILY, COLUMN_KEY_TOKEN_COUNT, serialize(doc.getTokenCount()));

    cout << "written!" << endl;
}
