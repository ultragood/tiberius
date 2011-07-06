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

        class SuffixTree {
	public:
	  SuffixTree();
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
	  
	  int getTotalNumberOfCoOccurrences(string &t, set<string> *coOccurSet=NULL);
	  
	  float sim(set<STNode *> *nodes1, set<STNode *> *nodes2);
	  
	  /*
	   * Function that returns the residual IDF for a 
	   * given phrase as long as it satisifes some 
	   * basic parts of speach rules.
	   * RIDF = IDF - EXPECTED_IDF
	   * where IDF = log2(N/df) - log2(1- wf/N)
	   */
	  double calcInformativeness(string &phrase);
	  
	private:
	  void initialize();
	  double _ridf(STNode *node);
	  void addStoryToNodeMap(string &storyId, STNode *node);
	  map<string, long> stories;
	  map<string, set<STNode *>* > storyToNodeMap;
	  vector<string> uidToStringMap;
	  vector<map<string, unsigned long>* > stringToUidMapPerLevel;
	  vector<STNode *> nodes;
	  vector<vector<STNode *> > sentenceNodes;
	  STNode *root;
	  int totalWordCount;
        };
    }
}

#endif
