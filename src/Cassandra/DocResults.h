
#pragma once

namespace tiberius{

    struct DocResults{
        DocResults(string doc_id, unsigned int frequency, vector<unsigned int> &positions) : doc_id(doc_id), frequency(frequency), positions(positions){
            
        }
        string doc_id;
        unsigned int frequency;
        vector<unsigned int> positions;
    };
}
