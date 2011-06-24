
test:

	g++ -o cpptest -Wall -g -I gen-cpp/. -I/usr/local/include/thrift -L/usr/local/lib -lstdc++ -lboost_serialization -lboost_thread -lthrift TermFrequencyVector.cpp CassandraConnection.cpp gen-cpp/cassandra_constants.cpp gen-cpp/cassandra_types.cpp gen-cpp/Cassandra.cpp

example:

	g++ -o cpptest -Wall -g -I gen-cpp/. -I/usr/local/include/thrift -L/usr/local/lib -lstdc++ -lboost_serialization -lthrift example.cpp gen-cpp/cassandra_constants.cpp gen-cpp/cassandra_types.cpp gen-cpp/Cassandra.cpp

