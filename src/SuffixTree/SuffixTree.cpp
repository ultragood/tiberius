#include "SuffixTree.h"

#include <iostream>
#include <sstream>
#include <queue>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include "../Keywords/CompositeWord.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

void NLP::SuffixTree::SuffixTree::initialize() {
    string root("__ROOT__");
    unsigned long uid = this->getUid(root, 0);
    this->root = this->createNode(uid);
    this->totalWordCount=0;
}

NLP::SuffixTree::SuffixTree::SuffixTree() {
  this->initialize();
}

NLP::SuffixTree::SuffixTree::~SuffixTree() {
    for (unsigned int i = 0, length = nodes.size(); i < length; ++i) {
        delete nodes[i];
    }
    nodes.clear();
}

void NLP::SuffixTree::SuffixTree::addSentence(vector<string> &sentence, string &storyId) {
    for (unsigned long i = 0; i < sentence.size(); ++i) {
        this->addSentence(sentence, i, storyId);
    }
}

void NLP::SuffixTree::SuffixTree::addSentence(vector<word> &sentence, string &storyId) {
    for (unsigned long i = 0; i < sentence.size(); ++i) {
        this->addSentence(sentence, i, storyId);
    }
}

void NLP::SuffixTree::SuffixTree::addSentence(vector<word> &lw, unsigned long index, string &storyId) {
  NLP::SuffixTree::STNode *node = this->root;
  int level = 1;
  if (index == 0) {
    this->totalWordCount += lw.size();
  }
  vector<NLP::SuffixTree::STNode *> nodeSentence;
  //word::const_iterator a;
  sentence::const_iterator w;
  Keywords::CompositeWord cw;
  vector<string> s;
  for (unsigned int i=index; i<lw.size(); i++) {
    cw.addWord(lw[i]);
    string w = cw.toString();
    unsigned long uid = this->getUid(w, level);
    NLP::SuffixTree::STNode *childNode = node->createNodeIfNeeded(uid);
    childNode->setFreeLingWord(lw[i]);
    childNode->incrementFrequencyCount();
    childNode->addStoryId(storyId);
    this->addStoryToNodeMap(storyId, childNode);
    node = childNode;
    ++level;
    if (index == 0) {
      nodeSentence.push_back(node); 
      //cout << word << " ";
    }
    cw.clear();
  }
  if (index == 0) {
    this->sentenceNodes.push_back(nodeSentence);

  }
}

void NLP::SuffixTree::SuffixTree::addSentence(vector<string> &sentence, unsigned long index, string &storyId) {
    NLP::SuffixTree::STNode *node = this->root;
    int level = 1;
    if (index == 0) {
      this->totalWordCount += sentence.size();
    }
    vector<NLP::SuffixTree::STNode *> nodeSentence;
    for (unsigned int i = index, length = sentence.size(); i < length; ++i) {
        string word = sentence[i];
        unsigned long uid = this->getUid(word, level);
        NLP::SuffixTree::STNode *childNode = node->createNodeIfNeeded(uid);
	childNode->incrementFrequencyCount();
        childNode->addStoryId(storyId);
        this->addStoryToNodeMap(storyId, childNode);
        node = childNode;
        ++level;
	if (index == 0) {
	  nodeSentence.push_back(node); 
	  //cout << word << " ";
	}
    }
    if (index == 0) {
      this->sentenceNodes.push_back(nodeSentence);
    }
}

void NLP::SuffixTree::SuffixTree::addStoryToNodeMap(string &storyId, NLP::SuffixTree::STNode *node) {
    NLP::SuffixTree::STStory story = this->createStory(storyId, 1);
    set<NLP::SuffixTree::STNode *> *nodeSet = this->storyToNodeMap[storyId];
    if (!nodeSet) {
        nodeSet = new set<NLP::SuffixTree::STNode *>();
        this->storyToNodeMap[storyId] = nodeSet;
    }
    nodeSet->insert(node);
}

NLP::SuffixTree::STNode * NLP::SuffixTree::SuffixTree::createNode(unsigned long uid) {
    NLP::SuffixTree::STNode *node = new NLP::SuffixTree::STNode(this, uid);
    this->nodes.push_back(node);
    return node;
}

NLP::SuffixTree::STStory NLP::SuffixTree::SuffixTree::getStory(string &storyId) {
    int count = this->stories[storyId];
    return NLP::SuffixTree::STStory(storyId, count);
}

NLP::SuffixTree::STStory NLP::SuffixTree::SuffixTree::createStory(string &storyId, int initArcCount) {
    this->stories[storyId] += initArcCount;
    return NLP::SuffixTree::STStory(storyId,this->stories[storyId]);
}

unsigned long NLP::SuffixTree::SuffixTree::getUid(string &word, int level) {
    unsigned long uid;
    map<string, unsigned long> *stringToUidMapPtr = NULL;
    if (level > (int) this->stringToUidMapPerLevel.size()) {
        ostringstream oss;
        oss << "SuffixTree::getUid is asking for a level that is at least two above the highest level." << endl;
        oss << "Level requested: " << level << endl;
        oss << "Highest known level: " << (this->stringToUidMapPerLevel.size() - 1);
        throw runtime_error(oss.str());
    }

    if (level < (int) this->stringToUidMapPerLevel.size()) {
        stringToUidMapPtr = this->stringToUidMapPerLevel.at(level);
    }
    if (!stringToUidMapPtr) {
        stringToUidMapPtr = new map<string, unsigned long>();
        this->stringToUidMapPerLevel.push_back(stringToUidMapPtr);
    }
    map<string, unsigned long>::iterator it = stringToUidMapPtr->find(word);
    if (it == stringToUidMapPtr->end()) {
        this->uidToStringMap.push_back(word);
        uid = (unsigned long) this->uidToStringMap.size()-1;
        (*stringToUidMapPtr)[word] = uid;
    } else {
        uid = (*stringToUidMapPtr)[word];
    }
    return uid;
}

string NLP::SuffixTree::SuffixTree::getString(unsigned long uid) {
    if (uid >= uidToStringMap.size()) {
        ostringstream oss;
        oss << "Requested uid " << uid << " exceeds size of uid-to-string map.";
        throw runtime_error(oss.str());
    }
    return uidToStringMap.at(uid);
}

void NLP::SuffixTree::SuffixTree::displaySuffixTree() {
    root->print();
}

set<NLP::SuffixTree::STNode *> * NLP::SuffixTree::SuffixTree::getNodes(string &storyId) {
    set<NLP::SuffixTree::STNode *> *nodeSet = storyToNodeMap[storyId];
    return nodeSet;
}

double NLP::SuffixTree::SuffixTree::_ridf(NLP::SuffixTree::STNode *node) {
  int wf = node->getFrequencyCount();
  unsigned long df = node->getNumberOfStories();
  unsigned int numDocs = this->stories.size();
  double oneHalfPercentNumDocs =  (double) numDocs * 0.005;
  if (wf == 1 || oneHalfPercentNumDocs > wf) {
    return 0;
  }
  double idfFrac = (double) (numDocs / (double) df);
  double possion_fraq = (double) (wf / (double) numDocs);
  // just a safe guard
  if (possion_fraq >= 1) {
    possion_fraq = 0;
  }
  //cout << "wf: " << wf << " df: " << df << " D: " << numDocs << " idfFrac: " << idfFrac << " possion_fraq: " << possion_fraq <<  " idf: " << log2(idfFrac) << endl;
  
  double ridf = log2(idfFrac) - log2(1-possion_fraq);
  return ridf;
}
  

double NLP::SuffixTree::SuffixTree::calcInformativeness(string &phrase) {
  vector<string> words;
  NLP::SuffixTree::STNode *node = this->root;
  boost::split(words, phrase, boost::is_any_of(" "));
  //vector<pair<string, int> > wordProbs;
  bool found = true;
  for (unsigned int i=0; i<words.size(); i++) {
    unsigned long uid = this->getUid(words[i], i+1);
    //cout << "uid of phrase (" << phrase << " ) is: " << uid << endl;
    NLP::SuffixTree::STNode *childNode = node->getChildNode((int) uid);
    if (childNode == NULL) {
      found = false;
      break;
    }
    if (i == 0) {
      // make sure the first word is a noun or verb
      string pos = childNode->getPos();
      if (pos.size() >= 2 && (pos.substr(0,2) == "NN"  
			      || pos.substr(0,2) == "JJ"  
			      //|| pos.substr(0,2) == "RB"  
			      //|| pos.substr(0,2) == "VB"
			      )) {      
	// this is fine
      }else{
	break;
      }
    }
    node = childNode;
    //cout << words[i] << " : " << node->getFrequencyCount() << endl;
  }
  double ridf = 0;

  if (found) {
    // make sure the last node is a noun or verb
    string pos = node->getPos();
    if (pos.size() >= 2 && (pos.substr(0,2) == "NN")) {
      ridf = this->_ridf(node);
    }
  }
  return ridf;
}
