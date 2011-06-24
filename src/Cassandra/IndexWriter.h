#pragma once

#include "CassandraConnection.h"

namespace tiberius{ namespace index {
    class IndexWriter{
        public:

        void writeDocument(Document &doc, Analyzer &analyzer);

        private:


    };
}}
