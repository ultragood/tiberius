
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

using namespace std;

namespace tiberius{
    class TermFrequencyVector { // : Serializable {

        public:

        TermFrequencyVector();
        virtual string toBytes();
        virtual void read(string &bytes);

        // temp for testing
        void add(string term, int freq, vector<int> &positions);

// private:
        int count;
        vector<string> terms;
        vector<int> frequencies;
        vector<vector<int> > positions;

        private:
            friend class boost::serialization::access;
            template<class Archive>
            void serialize(Archive &ar, const unsigned int version){
                ar & terms;
                ar & frequencies;
                ar & positions;
                ar & count;
            }

    };
}
