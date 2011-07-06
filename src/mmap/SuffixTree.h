#ifndef __TIBERIUS_MMAP_SUFFIXTREE__
#define __TIBERIUS_MMAP_SUFFIXTREE__

#include "SuffixTreeVars.h"

#include <string>
#include <list>

#include "freeling.h"
#include "freeling/traces.h"

using namespace std;

namespace tiberius {
  namespace mmap {
    struct WordAttributes {
      unsigned int frequencyCount;
      string pos;
      string parent;
    };
    class SuffixTree {
    public:
      SuffixTree(void *memoryFile, SuffixTreeVars *globalVars);
      void addStory(list<sentence> &sentences);
      Node * getChildNode(Node *node, string &candidateTerm);
      double calcInformativeness(string &phrase);
      double _ridf(Node *node);
      void persist();

      inline Node * getRoot() {
	tiberius::mmap::Node *root = (tiberius::mmap::Node *) memoryFile + this->globalVars->root_offset;	
	return root;
      }

    private:
      void parseSentence(list<sentence> &sentences);
      LinkedList *childList;
      void *memoryFile;
      int llsize;
      SuffixTreeVars *globalVars;
      static char *ROOT_NAME;
      vector<map<string, WordAttributes *>* > levels;      
      
    };
  }
}
#endif
