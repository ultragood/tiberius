#pragma once

#include "CassandraConnection.h"
#include "Analyzer.h"
namespace tiberius{ namespace index {
    class IndexWriter{
        public:

        IndexWriter();
        void writeDocument(string &doc_id, string &text, tiberius::analysis::Analyzer &analyzer);

        private:

        static const string FREQ_KEY;
        static const string POS_KEY;
/*
        static const string TERMINFO_COLUMN_FAMILY;
        static const string DOCUMENT_COLUMN_FAMILY;
        static const string COLUMN_KEY_TERMS;
        static const string COLUMN_KEY_POSITIONS;
        static const string COLUMN_KEY_FREQ;
        static const string COLUMN_KEY_TOKEN_COUNT;
*/
    };
}}
