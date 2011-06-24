
#pragma once
#include "Streams/TokenStream.h"

namespace tiberius{ namespace analysis {

    class Analyzer{
        public:

        virtual ~Analyzer() {};

        virtual streams::TokenStream * getTokenStream(std::string &text) = 0;

    };
}}
