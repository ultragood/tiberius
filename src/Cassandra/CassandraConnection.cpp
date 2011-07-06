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
#include "IndexWriter.h"
#include "IndexReader.h"
#include <boost/unordered_map.hpp>

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

/*
int main(){
//    CassandraConnection::instance;
    string text(" characters in pattern rules can be quoted with preceding backslashes (). Backslashes that would otherwise quote  characters can be quoted with more backslashes. Backslashes that quote  characters or other backslashes are removed from the pattern before it is compared to file names or has a stem substituted into it. Backslashes that are not in danger of quoting  characters go unmolested. For example, the pattern theweirdpattern has theweird preceding the operative  character, and pattern following it. The final two backslashes are left alone because they cannot affect any  character.");
    SimpleAnalyzer a;
    string id("doc1");

    IndexWriter writer;
    writer.writeDocument(id, text, a);
    

    TermFrequencyVector v;
    IndexReader reader;
    v = reader.getTermFrequencyVector(id);

    vector<string> docs;
    string term("pattern");
    reader.getDocsForTerm(term, docs);
    return 0;
}

*/
