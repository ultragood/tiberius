
create keyspace InvertedIndex
    with strategy_options=[{replication_factor:1}];

use InvertedIndex;

create column family Document 
    with comparator = UTF8Type;

create column family TermInfo
    with comparator = UTF8Type;
 
