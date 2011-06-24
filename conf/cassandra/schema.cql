
create keyspace InvertedIndex;

use InvertedIndex;

create column family Document ;

create column family TermInfo
    with column_type = 'Super';
 
