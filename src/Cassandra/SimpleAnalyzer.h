
#ifndef SIMPLEANALYZER_H_INCLUDED
#define SIMPLEANALYZER_H_INCLUDED
#include "Streams/WhitespaceTokenStream.h"
#include "Analyzer.h"
#include <boost/shared_ptr.hpp>

namespace tiberius{ namespace analysis {
    class SimpleAnalyzer : public Analyzer{
        public:

            SimpleAnalyzer();
            virtual ~SimpleAnalyzer();

            streams::TokenStream * getTokenStream(std::string &text);

        private:
            boost::shared_ptr<streams::WhitespaceTokenStream> _tokenStream;
    };
} }

#endif // SIMPLEANALYZER_H_INCLUDED
