#ifndef __SUFFIXTREE__
#define __SUFFIXTREE__

#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>

#include "STNode.h"
#include "STStory.h"
#include "json/value.h"
#include "fries/language.h"
using namespace std;

namespace NLP {
    namespace SuffixTree {

        class STNode;

        //struct compare_stcluster_pointers {
        //    bool operator() (STClusterType *first, STClusterType *second) const;
        //};

	class TermFreq {
	public:
	TermFreq(string term, int freq, double kai_sq) : term(term), freq(freq), kai_sq(kai_sq) { }
	    
	  string term;
	  int freq;
	  double kai_sq;
	  bool operator< (const TermFreq &termFreq) const {
	    return (this->freq < termFreq.freq);
	  }

	  bool operator== (const TermFreq &termFreq) const {
	    return this->term == termFreq.term;
	  }
	};

        class SuffixTree {
            public:
	  SuffixTree();
		SuffixTree(const string &data_type);
		SuffixTree(const string &data_type, set<string> term_phrases);
                virtual ~SuffixTree();
                STNode* createNode(unsigned long uid);
                STStory createStory(string &storyId, int initArcCount);
                STStory getStory(string &storyId);
                unsigned long getUid(string &word, int level);
                string getString(unsigned long uid);
                void addSentence(vector<string> &sentence, string &storyId);
                void addSentence(vector<word> &sentence, string &storyId);
                void addSentence(vector<word> &sentence, unsigned long index, string &storyId);
                void addSentence(vector<string> &sentence, unsigned long index, string &storyId);
                void displaySuffixTree();
                set<STNode *> * getNodes(string &storyId);

		priority_queue<TermFreq> getPhrases();
		int getTotalNumberOfCoOccurrences(string &t, set<string> *coOccurSet=NULL);

                float sim(set<STNode *> *nodes1, set<STNode *> *nodes2);

		void getFrequencies(string g, string w, int &gfreq, int &wfreq, int &gwfreq);
		int getNumWordsInSentencesContaining(string term);

		void kaiSquared(NLP::SuffixTree::STNode *node1, NLP::SuffixTree::STNode *node2, double &kai_sq, int &gfreq);

		inline set<string> getUnstemmedWords(string stemmedWord) {
		  return (*this->stemWordMap)[stemmedWord];
		}

		double isPhrase(string &phrase);
		double ridf(STNode *node);
		
            private:
                void initialize();
                bool isNumeric( const char* pszInput);
                double idf(STNode *node);
                void addStoryToNodeMap(string &storyId, STNode *node);
                map<string, long> stories;
                map<string, set<STNode *>* > storyToNodeMap;
                vector<string> uidToStringMap;
                vector<map<string, unsigned long>* > stringToUidMapPerLevel;
                vector<STNode *> nodes;
		vector<vector<STNode *> > sentenceNodes;
                STNode *root;
                //NLP::SentenceAnalyzer *sentenceAnalyzer;
		int totalWordCount;
		//vector<TermFreq> phrases;
		priority_queue<TermFreq> phrases;
		map<string, set<string> > *stemWordMap;
		set<string> term_phrases;
		string data_type;
        };
    }
}

#endif
