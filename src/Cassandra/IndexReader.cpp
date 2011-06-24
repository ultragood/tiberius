
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
    CassandraConnection::instance().getAll(docId, "", column_parent, key_slices);

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
                deserialize(value, positions);
            }else if(col_name == COLUMN_KEY_TERMS){
                deserialize(value, terms);
            }else if(col_name == COLUMN_KEY_FREQ){
                deserialize(value, frequencies);
            }
        }
    }
    TermFrequencyVector tv(token_count, terms, frequencies, positions);

    cout << tv.toString() << endl;

    return tv;
}


void IndexReader::getDocsForTerm(string &term, vector<string> &docs){
    ColumnParent column_parent;
    column_parent.column_family.assign(TERMINFO_COLUMN_FAMILY);
    column_parent.__isset.super_column = false;

    vector<KeySlice> key_slices;
    CassandraConnection::instance().getAll(term, term, column_parent, key_slices);

    cout << " Docs for Term: " << term << endl;

    for(size_t s=0; s < key_slices.size(); s++){
        for(size_t c=0; c < key_slices[s].columns.size(); c++){
            string docid = key_slices[s].columns[c].super_column.name;
            docs.push_back(docid);

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
            cout << "{docid: docid, " << " frequency: " << frequency << ", positions: [";
            for(unsigned int i=0; i < positions.size(); i++){
                if(i > 0) cout << ", ";
                cout << positions[i];
            }
            cout << "]}" << endl;
        }
    }

    
}
