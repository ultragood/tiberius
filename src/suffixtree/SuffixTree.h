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

#include <boost/archive/text_oarchive.hpp>

//#include <boost/serialization/list.hpp>
//#include <boost/serialization/string.hpp>

using namespace std;

namespace suffixtree {
  class STNode;
  class SuffixTree {
  public:
    SuffixTree();
    virtual ~SuffixTree();
    STNode* createNode(string &uid);
    string getFilename(STNode node);
    void addStory(vector<vector<word> > &story, string &storyId);
    void addSentence(vector<word> &sentence, string &storyId);
    void addSentence(vector<word> &sentence, unsigned long index, string &storyId);
    void displaySuffixTree();
    /*
     * Function that returns the residual IDF for a 
     * given phrase as long as it satisifes some 
     * basic parts of speach rules.
     * RIDF = IDF - EXPECTED_IDF
     * where IDF = log2(N/df) - log2(1- wf/N)
     */
    double calcInformativeness(string &phrase);

    STNode * getNode(string &uid);

    void save(suffixtree::STNode *node);
    string getFilename(suffixtree::STNode *node);
	  
  private:
  friend class boost::serialization::access;
    void initialize();
    void clearLocalMemory();
    double _ridf(STNode *node);
    //vector<STNode *> nodes;
    map<string, STNode *> nodes;
    STNode *root;
    unsigned long docCount;
    static string BASE_DIR;

    map<string, long> fileNameMap;
    long fileNameCount;

    int reads;
    int writes;

    template<class Archive> void serialize(Archive & ar, const unsigned int version) {
	ar & this->nodes;
    }

  };
}

#endif
