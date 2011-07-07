
#include "IndexReader.h"
#include "CassandraConnection.h"
#include "Constants.h"
#include "Serializer.h"

using namespace tiberius::index;

IndexReader::IndexReader(){
    // make sure the connection is open
    CassandraConnection::instance().open();
}

 
TermFrequencyVector IndexReader::getTermFrequencyVector(string &docId){
    ColumnParent column_parent;
    column_parent.column_family.assign(DOCUMENT_COLUMN_FAMILY);
    column_parent.__isset.super_column = false;


    vector<KeySlice> key_slices;
    CassandraConnection::instance().getAll(docId, docId, column_parent, key_slices);

    unsigned int token_count;
    vector<unsigned int> frequencies;
    vector<vector<unsigned int> > positions;
    vector<string> terms;

    if(key_slices.size() > 0){
        for(size_t i=0; i < key_slices[0].columns.size(); i++){
            string col_name = key_slices[0].columns[i].column.name;
            string value    = key_slices[0].columns[i].column.value;

            if(col_name == COLUMN_KEY_TOKEN_COUNT){
                deserialize(value, token_count);
            }else if(col_name == COLUMN_KEY_POSITIONS){
//                deserialize(value, positions);
            }else if(col_name == COLUMN_KEY_TERMS){
                deserialize(value, terms);
            }else if(col_name == COLUMN_KEY_FREQ){
                deserialize(value, frequencies);
            }
        }
    }
    TermFrequencyVector tv(token_count, terms, frequencies, positions);

//    cout << tv.toString() << endl;

    return tv;
}


void IndexReader::getDocsForTermList(vector<string> &terms, map<string, int> &docs){
    ColumnParent column_parent;
    column_parent.column_family.assign(TERMINFO_COLUMN_FAMILY);
    column_parent.__isset.super_column = false;

    map<string, vector<ColumnOrSuperColumn> > results;
    CassandraConnection::instance().getAll(terms, column_parent, results);

    for(map<string, vector<ColumnOrSuperColumn> >::iterator it = results.begin(); it!= results.end(); it++){
        for(size_t c=0; c < it->second.size(); c++){
            ColumnOrSuperColumn * col = &it->second[c];

            string docid = col->super_column.name;
            unsigned int frequency;

            for(size_t i=0; i < col->super_column.columns.size(); i++){
                string col_name = col->super_column.columns[i].name;
                string value = col->super_column.columns[i].value;
                if(col_name == COLUMN_KEY_FREQ){
                    deserialize(value, frequency);
                }
            }

            docs[docid]++; // = frequency;
        }
    }

}

void IndexReader::getDocsForTerm(string &term, vector<DocResults> &docs){
    ColumnParent column_parent;
    column_parent.column_family.assign(TERMINFO_COLUMN_FAMILY);
    column_parent.__isset.super_column = false;

    vector<KeySlice> key_slices;
    CassandraConnection::instance().getAll(term, term, column_parent, key_slices);

    for(size_t s=0; s < key_slices.size(); s++){
        for(size_t c=0; c < key_slices[s].columns.size(); c++){
            string docid = key_slices[s].columns[c].super_column.name;

            vector<unsigned int> positions;
            unsigned int frequency;
            for(size_t i=0; i < key_slices[s].columns[c].super_column.columns.size(); i++){
                string col_name = key_slices[s].columns[c].super_column.columns[i].name;
                string value = key_slices[s].columns[c].super_column.columns[i].value;

                if(col_name == COLUMN_KEY_POSITIONS){
                    deserialize(value, positions);
                }else if(col_name == COLUMN_KEY_FREQ){
                    deserialize(value, frequency);
                }

            }
            docs.push_back(DocResults(docid, frequency, positions));
/*
            cout << "{docid: " << docid << ", " << " frequency: " << frequency << ", positions: [";
            for(unsigned int i=0; i < positions.size(); i++){
                if(i > 0) cout << ", ";
                cout << positions[i];
            }
            cout << "]}" << endl;
*/
        }
    }
}

int IndexReader::getDocCount(){
    ColumnOrSuperColumn csc;
    CassandraConnection::instance().get(csc, KEY_GLOBAL_DOC_COUNT, COLUMN_FAMILY_DOC_COUNT, COLUMN_KEY_DOC_COUNT);

    return csc.counter_column.value;
    
}

void IndexReader::getDocFreqsForTerms(const vector<string> &terms, map<string, int32_t> &counts){
    string empty;
    CassandraConnection::instance().getCounts(counts, terms, TERMINFO_COLUMN_FAMILY, empty);
    
}

int IndexReader::getDocFreqForTerm(const string &term){
    string empty;
    return CassandraConnection::instance().getCount(term, TERMINFO_COLUMN_FAMILY, empty);
}
