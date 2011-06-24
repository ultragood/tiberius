#include <iostream>
#include <protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <sstream>

#include "Cassandra.h"
#include "CassandraConnection.h"

#include "TermFrequencyVector.h"
#include "Serializer.h"

#include "SimpleAnalyzer.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace org::apache::cassandra;
using namespace boost;
using namespace tiberius;
using namespace tiberius::index;
using namespace tiberius::analysis;

struct data_t{
    int value;
};


int main(){
//    CassandraConnection::instance;

    const string& key="term2";

    ColumnOrSuperColumn csc;

    data_t foo;
    foo.value = 129;
    vector<char> value(sizeof(foo));
    memcpy(&value[0], &foo, sizeof(foo));
    string strval(value.begin(), value.end());

    TermFrequencyVector v;
    vector<int> positions;
    v.add("foo", 2, positions);
    v.add("bar", 5, positions);
    string strterms = serialize(v);

    cout << serialize(v) << endl;

    string text(" characters in pattern rules can be quoted with preceding backslashes (). Backslashes that would otherwise quote  characters can be quoted with more backslashes. Backslashes that quote  characters or other backslashes are removed from the pattern before it is compared to file names or has a stem substituted into it. Backslashes that are not in danger of quoting  characters go unmolested. For example, the pattern theweirdpattern has theweird preceding the operative  character, and pattern following it. The final two backslashes are left alone because they cannot affect any  character.");
    SimpleAnalyzer a;

    Token tk;
    streams::TokenStream * ts = a.getTokenStream(text);
    while(ts->next(tk)){
        cout << tk << endl;
    }


    CassandraConnection::instance().open();
    CassandraConnection::instance().insert(key, "TermInfo", "term", "term1");
    CassandraConnection::instance().insert(key, "TermInfo", "position", "term1");
    CassandraConnection::instance().insert(key, "Document", "docid", strterms);
    CassandraConnection::instance().get(csc, key, "TermInfo", "term");
    cout << "Value read is '" << csc.column.value.data() << "'..." << endl;
    CassandraConnection::instance().get(csc, key, "Document", "docid");
//    cout << "Value read is '" << ((data_t *) csc.column.value.data())->value << "'..." << endl;

    ColumnParent cp;
    cp.column_family.assign("TermInfo");
    cp.__isset.super_column = false;

    cout << "getAll" << endl;
    CassandraConnection::instance().getAll(key, "", cp);
    cout << "endGetAll" << endl;

    TermFrequencyVector v2;
/*
    const char * data = csc.column.value.data();
    vector<char> bytes2(data[0], data[csc.column.value.size()]);
    v2.read(bytes2);
*/
    deserialize(csc.column.value, v2);
    cout << v2.getCount() << endl;

    CassandraConnection::instance().close();

    return 0;
}


