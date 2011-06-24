
#pragma once

#include "Serializable.h"
#include <vector>
#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>


#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include <sstream>

using namespace std;

namespace tiberius{
    class TermFrequencyVector { // : Serializable {

        public:

        TermFrequencyVector();
        TermFrequencyVector(unsigned int token_count, vector<string> terms, vector<unsigned int> freqs, vector<vector<unsigned int> > positions) :
            token_count(token_count), 
            terms(terms), 
            frequencies(freqs), 
            positions(positions){
        }

        // temp for testing
        void add(string term, unsigned int freq, vector<unsigned int> positions);

        int getTokenCount(){return token_count;}
        vector<vector<unsigned int> > getPositions() { return positions; }
        vector<unsigned int> getFrequencies() { return frequencies; }
        vector<string> getTerms(){ return terms;}

        string toString(){
            ostringstream os;
            os << "{token_count: " << token_count << ", terms: {";
            for(unsigned int i=0; i < terms.size(); i++){
                os << "{term: \"" << terms[i] << "\", freqency: " << frequencies[i] << ", positions: [";
                for(unsigned int j = 0; j < positions[i].size(); j++) {
//vector<unsigned int>::iterator it = positions[i].begin(); it!= positions[i].end(); it++){
                    if(j > 0) os << ", ";
                    os << positions[i][j];
                }
                os << "]}";
            }
            os << "}";
            return os.str();
        }



        private:

        unsigned int token_count;
        vector<string> terms;
        vector<unsigned int> frequencies;
        vector<vector<unsigned int> > positions;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive &ar, const unsigned int version){
            ar & terms;
            ar & frequencies;
            ar & positions;
            ar & token_count;
        }

    };
}
