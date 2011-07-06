#ifndef __STNode__
#define __STNode__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "SuffixTree.h"
#include "fries/language.h"

using namespace std;

namespace NLP {
    namespace SuffixTree {

        class SuffixTree;

        class STNode {
            public:
                STNode(SuffixTree *tree, unsigned long uid);
		STNode(){}
                virtual ~STNode();
                inline bool isRoot() const {
                    return root;
                }

                STNode * createNodeIfNeeded(unsigned long uid);
                int getLevel();
                void print();
                void addStoryId(string uid);
                map<string, long> getStories();
                unsigned long getNumberOfStories();
                unsigned long getNumberOfChildren();
		int getFrequencyCount();
                vector<STNode *> * getChildren();
		STNode * getChildNode(int wordId);
                string getWord();
                string getPhrase();
		void incrementFrequencyCount();
		STNode* getParent();
		inline void setFreeLingWord(word w) {
		  this->w = w;
		}

		//typedef (visitor_function) void(STNode &node);
		//void visit(visitor_function func);

		inline string getPos() {
		  //cout << "about to get form: ";
		  //cout << w.get_form() << endl;
		  return w.get_parole();
		}

            private:
                SuffixTree *tree;
                bool root;
                unsigned long wordUid;
                STNode *parent;
                vector<STNode *> children;
                map<int, STNode *> wordChildMap;
                map<string, long> stories;
                int level;
		int count;
		word w;

        };
    }
}

#endif
