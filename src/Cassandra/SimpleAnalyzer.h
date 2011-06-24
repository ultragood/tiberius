
#ifndef SIMPLEANALYZER_H_INCLUDED
#define SIMPLEANALYZER_H_INCLUDED
#include "Streams/TokenStream.h"
#include "Analyzer.h"

namespace tiberius{ namespace analysis {
    class SimpleAnalyzer : public Analyzer{
        public:

            SimpleAnalyzer();
            virtual ~SimpleAnalyzer();

            streams::TokenStream * getTokenStream(std::string &text);
        private:
            streams::TokenStream * _tokenStream;
    };
} }

#endif // SIMPLEANALYZER_H_INCLUDED
