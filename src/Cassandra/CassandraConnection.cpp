#include <iostream>
#include <protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "Cassandra.h"
#include "CassandraConnection.h"
#include "thrift/gen-cpp/tiberius_types.h"
#include "thrift/gen-cpp/tiberius_constants.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace org::apache::cassandra;
using namespace boost;
using namespace tiberius::index;

struct data_t{
    int value;
};


int main(){
//    CassandraConnection::instance;

    const string& key="term2";

    ColumnPath cpath;
    ColumnParent cp;
    
    ColumnOrSuperColumn csc;
    Column c;

    data_t foo;
    foo.value = 129;
    vector<char> value(sizeof(foo));
    memcpy(&value[0], &foo, sizeof(foo));
    string strval(value.begin(), value.end());

    CassandraConnection::instance().open();
    CassandraConnection::instance().insert(key, "TermInfo", "term", "term1");
    CassandraConnection::instance().insert(key, "Document", "docid", strval);
    CassandraConnection::instance().get(csc, key, "TermInfo", "term");
    cout << "Value read is '" << csc.column.value.data() << "'..." << endl;
    CassandraConnection::instance().get(csc, key, "Document", "docid");
    cout << "Value read is '" << ((data_t *) csc.column.value.data())->value << "'..." << endl;


    


    CassandraConnection::instance().close();

    return 0;
}


