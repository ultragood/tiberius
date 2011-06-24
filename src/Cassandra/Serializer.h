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

namespace tiberius{
/*
    template<class Archive>
    static void serialize(Archive &ar, vector<int> &o, const unsigned int version){
        ar & o;
    };
*/

    template<class T>
    static string serialize(T value){
        ostringstream ss;
        boost::archive::text_oarchive oa(ss);
        oa << value;
        return ss.str();
    }

    template<class T>
    static void deserialize(string s, T &value){
        istringstream is(s);
        boost::archive::text_iarchive ia(is);
        ia >> value;
    }
}
