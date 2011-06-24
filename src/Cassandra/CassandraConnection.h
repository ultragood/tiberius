#pragma once
#include "Cassandra.h"
#include <iostream>
#include <protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <exception>
#include <boost/thread.hpp>
using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace org::apache::cassandra;
using namespace boost;

namespace tiberius{ namespace index { 

    class CassandraConnection{


    public:

        static CassandraConnection &instance(){
            static boost::thread_specific_ptr<CassandraConnection> instance;
            if(instance.get() == NULL){
                instance.reset(new CassandraConnection("127.0.0.1", 9160));
            }
            return *instance;
        };

        bool open(){
            if(!isopen){
                try{
                    transport->open();
                    client.set_keyspace("InvertedIndex");
                    isopen = true;
                }catch(std::exception &e){
                    cerr << "Failed to open connection: " << e.what() << endl;
                }
            }
            return isopen;
        };

        bool close(){
            if(isopen){
                try{
                    transport->close(); 
                    isopen = false;
                }catch(std::exception &e){
                    cerr << "Failed to close connection: " << e.what() << endl;
                }
            }
            return !isopen;
        }

        int64_t getTs(){
            time_t ltime;
            ltime=time(NULL);
            return (int64_t)ltime;
        };

        bool insert(const string &key, const string &col_family,  const string &col_name, const string &value, string super_col = ""){
            Column c;
            c.name.assign(col_name);
            c.value.assign(value);
            c.timestamp = getTs();
            c.ttl = 300;
            c.__isset.value = true;
            c.__isset.timestamp = true;

            ColumnParent cp;
            cp.column_family.assign(col_family);
            cp.super_column.assign(super_col);
            cp.__isset.super_column = super_col.size() > 0;

            return insert(key, cp, c);

        }

        bool insert(const string &key, const ColumnParent &cp, const Column &c){
            try {
                client.insert(key, cp, c, org::apache::cassandra::ConsistencyLevel::ONE);
            }
            catch (NotFoundException &nf)       { cerr << "NotFoundException ERROR: "<< nf.what() << endl; }
            catch (InvalidRequestException &re) { cerr << "InvalidRequest ERROR: " << re.why << endl;     }
            catch (UnavailableException &ue)    { cerr << "UnavailableException ERROR: " << ue.what() << endl; }
            catch (TException &tx)              { cerr << "TException ERROR: " << tx.what() << endl; }
        
            return 0;

        }

        bool get(ColumnOrSuperColumn &csc, const string &key, const string &col_family, const string &col_name){
            ColumnPath cpath;
            cpath.column_family.assign(col_family);
            cpath.__isset.column = true;
            cpath.column = col_name;

            return get(csc, key, cpath);
        }

        bool get(ColumnOrSuperColumn &csc, const string &key, ColumnPath &cpath){
            try {
                client.get(csc, key, cpath, org::apache::cassandra::ConsistencyLevel::ONE);
            }
            catch (NotFoundException &nf)       { cerr << "NotFoundException ERROR: "<< nf.what() << endl; }
            catch (InvalidRequestException &re) { cerr << "InvalidRequest ERROR: " << re.why << endl;     }
            catch (UnavailableException &ue)    { cerr << "UnavailableException ERROR: " << ue.what() << endl; }
            catch (TException &tx)              { cerr << "TException ERROR: " << tx.what() << endl; }
        
            return 0;

        }

        ~CassandraConnection(){
            close();
        }


        bool getAll(string keyFrom, string keyTo, ColumnParent cparent, vector<KeySlice> &results){
            // get the entire row for a key
            SliceRange sr;
            sr.start = "";
            sr.finish = "";

            SlicePredicate sp;
            sp.slice_range = sr;
            sp.__isset.slice_range = true; // set __isset for the columns instead if you use them

            KeyRange range;
            range.start_key = keyFrom;
            range.end_key = keyTo;
            range.__isset.start_key = true;
            range.__isset.end_key = true;

            client.get_range_slices(results, cparent, sp, range, ConsistencyLevel::ONE);

            /*
            for(size_t i=0; i<results.size(); i++){
              printf("Key: %s\n", results[i].key.c_str());
              for(size_t x=0; x<results[i].columns.size(); x++){
                printf("Column: %s  Value: %s\n", results[i].columns[x].column.name.c_str(),
                  results[i].columns[x].column.value.c_str());
              }
            }
            */
            return true;
        }

    private:
        CassandraConnection(string host, int port) : 
                isopen(false),
                host(host), 
                port(port),  
                socket(new TSocket(host, port)),
                transport(new TFramedTransport(socket)),
                protocol(new TBinaryProtocol(transport)),
                client(protocol){


        };

        bool isopen;
        string host;
        int port;
        shared_ptr<TTransport> socket;
        shared_ptr<TTransport> transport;
        shared_ptr<TProtocol> protocol;
        CassandraClient client;
    };
}}
