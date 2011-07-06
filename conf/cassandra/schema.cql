connect localhost/9160;

create keyspace InvertedIndex;

use InvertedIndex;

create column family Document with comparator = 'UTF8Type' and key_validation_class = 'UTF8Type' and comparator = 'UTF8Type' and default_validation_class = UTF8Type;

create column family TermInfo with column_type = 'Super' and  comparator = 'UTF8Type' and key_validation_class = 'UTF8Type' and comparator = 'UTF8Type' and default_validation_class = UTF8Type;
 
