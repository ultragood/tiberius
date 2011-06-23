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

NLP::SuffixTree::SuffixTree::SuffixTree(const string &data_type) : data_type(data_type) {
  this->initialize();
}

NLP::SuffixTree::SuffixTree::SuffixTree(const string &data_type, set<string> term_phrases) : data_type(data_type) {
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

float NLP::SuffixTree::SuffixTree::sim(set<NLP::SuffixTree::STNode *> *nodes1,
				                   set<NLP::SuffixTree::STNode *> *nodes2) {
    set<NLP::SuffixTree::STNode *> result;
    set<NLP::SuffixTree::STNode *>::iterator it_result;
    set<NLP::SuffixTree::STNode *> result_intersect;
    set<NLP::SuffixTree::STNode *>::iterator it_result_intersect;
    insert_iterator< set<NLP::SuffixTree::STNode *> > insert_it(result, result.begin());
    insert_iterator< set<NLP::SuffixTree::STNode *> > insert_intersect_it(result_intersect, result_intersect.begin());
    set_union(nodes1->begin(), nodes1->end(), nodes2->begin(), nodes2->end(), insert_it);
    set_intersection(nodes1->begin(), nodes1->end(), nodes2->begin(), nodes2->end(), insert_intersect_it);
    float similarity = static_cast<float>(result_intersect.size()) / result.size();
    return similarity;
}

bool NLP::SuffixTree::SuffixTree::isNumeric(const char* pszInput) {
    istringstream iss(pszInput);
    double dTestSink;
    iss >> dTestSink;
    if (!iss) {
        return false;
    }
    return true;
}

double NLP::SuffixTree::SuffixTree::idf(NLP::SuffixTree::STNode *node) {
    unsigned long df = node->getNumberOfStories();
    unsigned long numDocs = nodes.size();
    return log( numDocs / df );
}

int NLP::SuffixTree::SuffixTree::getNumWordsInSentencesContaining(string term) {
  int count=0;
  for (unsigned int i=0; i<this->sentenceNodes.size(); i++) {
    vector<NLP::SuffixTree::STNode *> sentence = this->sentenceNodes[i];
    //set<string> terms;
    bool foundTerm=false;
    int sentenceCount=0;
    for (unsigned int j=0; j<sentence.size(); j++) {  
      if (sentence[j]->getWord() == term) { 
	foundTerm=true;
      }else{
	sentenceCount++;
      }
    }
    if (foundTerm) {
      count += sentenceCount;
    }
  }
  return count;
}
    

/*
void NLP::SuffixTree::SuffixTree::findPhrases(string uid) {
  set<string> phraseSet;
  for (unsigned int i=0; i<this->sentenceNodes.size(); i++) {
    vector<NLP::SuffixTree::STNode *> sentence = this->sentenceNodes[i];
    //int prevFreq=0;
    double prev_kai;
    vector<NLP::SuffixTree::STNode *> phraseNodeList;
    for (unsigned int j=0; j<sentence.size(); j++) {
      NLP::SuffixTree::STNode *node = sentence[j];
      if (phraseNodeList.size() > 0) {
	NLP::SuffixTree::STNode *prevNode = phraseNodeList[phraseNodeList.size()-1];
	double kai_sq=0;
	int gfreq=0;
	this->kaiSquared(prevNode, node, kai_sq, gfreq);
	if (kai_sq > 0.5) {
	  //phraseNodeList.push_back(node);
	}else{
	  //cout << phrase << " : " << prevFreq << endl;
	  string phrase;
	  phrase.append(phraseNodeList[0]->getWord());
	  for (unsigned int k=1; k<phraseNodeList.size(); k++) {
	    phrase.append(" ").append(phraseNodeList[k]->getWord());
	  }
	  if (phraseNodeList.size() == 1) {
	    prev_kai=0;
	  }
	  NLP::SuffixTree::TermFreq tf(phrase, gfreq, prev_kai);
	  if (phraseSet.find(phrase) == phraseSet.end()) {
	    phraseSet.insert(phrase);
	    this->phrases.push(tf);
	  }
	  phraseNodeList.clear();

	}
      prev_kai = kai_sq;
      }
      phraseNodeList.push_back(node);
    }
  }
}
*/
void NLP::SuffixTree::SuffixTree::getFrequencies(string g, string w, int &gfreq, int &wfreq, int &gwfreq) {
  for (unsigned int i=0; i<this->sentenceNodes.size(); i++) {
    vector<NLP::SuffixTree::STNode *> sentence = this->sentenceNodes[i];
    NLP::SuffixTree::STNode *prevNode=NULL;
    for (unsigned int j=0; j<sentence.size(); j++) {
      NLP::SuffixTree::STNode *node = sentence[j];
      if (node->getWord() == g) {
	gfreq++;
      }else if((w.size()) > 0 && node->getWord() == w) {
	wfreq++;
	if (prevNode && (prevNode->getWord() == g)) {
	  gwfreq++;
	}
      }
      prevNode = node;
    }
  }
}

void NLP::SuffixTree::SuffixTree::kaiSquared(NLP::SuffixTree::STNode *node1, NLP::SuffixTree::STNode *node2, double &kai_sq, int &gfreq) {
  string g = node1->getWord();
  string w;
  if (node2) {
    w = node2->getWord();
  }
  int wfreq=0;
  int gwfreq=0;
  this->getFrequencies(g, w, gfreq, wfreq, gwfreq);
  if (gfreq <= 1) {
    return;
  }
  //int totalFreq = gfreq+wfreq;
  kai_sq = (double) (gwfreq / (float) gfreq);
  //cout << "Comparing: " << g << " with " << w << endl;
  //cout << "\t percent " <<  kai_sq << endl;
}
/*
priority_queue<NLP::SuffixTree::TermFreq> NLP::SuffixTree::SuffixTree::getPhrases() {
  return this->phrases;
}
*/


double NLP::SuffixTree::SuffixTree::ridf(NLP::SuffixTree::STNode *node) {
  int wf = node->getFrequencyCount();
  unsigned long df = node->getNumberOfStories();
  unsigned int numDocs = this->stories.size();
  double idfFrac = (double) (numDocs / (double) df);
  double possion_fraq = (double) (wf / (double) numDocs);
  // just a safe guard
  if (possion_fraq >= 1) {
    possion_fraq = 0;
  }
  cout << "wf: " << wf << " df: " << df << " D: " << numDocs << " idfFrac: " << idfFrac << " possion_fraq: " << possion_fraq << endl;
  
  double ridf = log2(idfFrac) - log2(1-possion_fraq);
  return ridf;
}
  

double NLP::SuffixTree::SuffixTree::isPhrase(string &phrase) {
  vector<string> words;
  NLP::SuffixTree::STNode *node = this->root;
  boost::split(words, phrase, boost::is_any_of(" "));
  //vector<pair<string, int> > wordProbs;
  bool found = true;
  for (unsigned int i=0; i<words.size(); i++) {
    unsigned long uid = this->getUid(words[i], i+1);
    cout << "uid of phrase (" << phrase << " ) is: " << uid << endl;
    NLP::SuffixTree::STNode *childNode = node->getChildNode((int) uid);
    if (childNode == NULL) {
      found = false;
      break;
    }
    node = childNode;
    cout << words[i] << " : " << node->getFrequencyCount() << endl;
  }
  double ridf = 0;
  if (found) {
    ridf = this->ridf(node);
  }
  return ridf;
}
