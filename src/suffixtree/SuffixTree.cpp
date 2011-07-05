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

// used for boost serialization and deserialization.
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>
#include <fstream>
#include "Cassandra/Serializer.h"
#include "Cassandra/CassandraConnection.h"
#include "Cassandra/Constants.h"
// --
#include <boost/filesystem.hpp>

#define CASSANDRA
using namespace tiberius::index;

void suffixtree::SuffixTree::initialize() {
    this->fileNameCount=0;

    string root("__ROOT__");
    this->root = this->createNode(root);

#ifdef CASSANDRA    
    CassandraConnection::instance().open();
#endif
}

suffixtree::SuffixTree::SuffixTree() {
  this->initialize();
}

suffixtree::SuffixTree::~SuffixTree() {
  //for (unsigned int i = 0, length = nodes.size(); i < length; ++i) {
  //      delete nodes[i];
  //  }
//  this->clearLocalMemory();
}

void suffixtree::SuffixTree::clearLocalMemory() {

  cout << "Saving to disk and clearing local memory... (" << nodes.size() << ")" << endl;
  string col = "st";
  string key = "foo";
//  CassandraConnection::instance().insert(key, SUFFIX_TREE_COLUMN_FAMILY, col, tiberius::serialize(*this));
	
  for (map<string, suffixtree::STNode *>::iterator it=this->nodes.begin(); it!=this->nodes.end(); it++) {
    if (!it->second->isRoot()) {
	save(it->second);
        delete it->second;
    }
  }
  cout << "Done saving everything " << endl;

  this->nodes.clear();
  cout << "Done clearing memory." << endl;
  cout << "Adding back the root node." << endl;
  this->nodes[this->root->getWord()] = this->root;
}

void suffixtree::SuffixTree::addStory(vector<vector<word> > &story, string &storyId) {
  for (unsigned int i=0; i<story.size(); i++) {
    this->addSentence(story[i], storyId);
  }
  this->docCount++;
  
  if (this->docCount % 10 == 0) {
    this->clearLocalMemory();
  }	
   
  
}

void suffixtree::SuffixTree::addSentence(vector<word> &sentence, string &storyId) {
    for (unsigned long i = 0; i < sentence.size(); ++i) {
        this->addSentence(sentence, i, storyId);
    }
}

void suffixtree::SuffixTree::addSentence(vector<word> &lw, unsigned long index, string &storyId) {
  suffixtree::STNode *node = this->root;
  int level = 1;
  //  vector<suffixtree::STNode *> nodeSentence;
  sentence::const_iterator w;
  Keywords::CompositeWord cw;
  vector<string> s;
  string uid;
  string sep("-");
  bool alwaysCreate = false;
  for (unsigned int i=index; i<lw.size() && i<8; i++) {
    if (i>index) {
      uid.append(sep);
    }
    uid.append(lw[i].get_form());
    cw.addWord(lw[i]);
    string w = cw.toString();
    //suffixtree::STNode *childNode = node->createNodeIfNeded(uid);
    suffixtree::STNode *childNode = NULL; // = node->createNodeIfNeeded(w);

    if(!alwaysCreate) 
	childNode = this->getNode(uid);
    if(childNode == NULL){
	alwaysCreate = true;
	childNode = this->createNode(uid);
    }
    childNode->setPos(lw[i].get_parole());
    childNode->incrementFrequencyCount();
    childNode->addStoryId(storyId);
    node = childNode;
    ++level;

//    this->save(node);

    //if (index == 0) {
    //  nodeSentence.push_back(node); 
    //}
    cw.clear();
  }

}

void suffixtree::SuffixTree::save(suffixtree::STNode *node) {
  string filename = this->getFilename(node);

  if(++writes % 1000 == 0){
	cout << "executed " << writes << " writes" << endl;
  }
#ifdef CASSANDRA
  CassandraConnection::instance().insert(filename, SUFFIX_TREE_COLUMN_FAMILY, COLUMN_KEY_OBJECT, tiberius::serialize(*node));
#else
  ofstream ofs(filename.c_str());
  boost::archive::text_oarchive ia(ofs);
  ia & *node;
#endif

  //	cout << "saved " << filename << endl;
} 

string suffixtree::SuffixTree::getFilename(suffixtree::STNode *node) {
  string uid = node->getUid();
  return uid;
/*
  if (this->fileNameMap.find(uid) != this->fileNameMap.end()) {
    this->fileNameMap[uid]= this->fileNameCount++;
  }
  stringstream ss;
  ss << "/rt/store/suffixtree/" << this->fileNameMap[uid];
    //string filename = BASE_DIR+node->getUid()  +".ser";
    //return filename;
  return ss.str();
*/
}

suffixtree::STNode * suffixtree::SuffixTree::createNode(string &uid) {
    suffixtree::STNode *node = new suffixtree::STNode(this, uid);
    this->nodes[uid] =  node;
    //this->nodes.push_back(node);
    return node;
}

void suffixtree::SuffixTree::displaySuffixTree() {
    root->print();
}

double suffixtree::SuffixTree::_ridf(suffixtree::STNode *node) {
  int wf = node->getFrequencyCount();
  unsigned long df = node->getNumberOfStories();
  unsigned long numDocs = this->docCount;
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
  

double suffixtree::SuffixTree::calcInformativeness(string &phrase) {
  vector<string> words;
  suffixtree::STNode *node = this->root;
  boost::split(words, phrase, boost::is_any_of(" "));
  //vector<pair<string, int> > wordProbs;
  bool found = true;
  for (unsigned int i=0; i<words.size(); i++) {
    suffixtree::STNode *childNode = node->getChildNode(words[i]);
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


string suffixtree::SuffixTree::BASE_DIR = ""; // "/rt/store/suffixtree/";
suffixtree::STNode * suffixtree::SuffixTree::getNode(string &uid) {
  suffixtree::STNode *node = NULL; // new suffixtree::STNode();
  if (this->nodes.find(uid) != this->nodes.end()) {
    node = this->nodes[uid];
  }else{
	if(++reads % 1000 == 0){
		cout << "executed " << reads << " reads" << endl;
        }

#ifdef CASSANDRA
    // Cassandra desrialization
	ColumnOrSuperColumn col;
	if(CassandraConnection::instance().get(col, uid, SUFFIX_TREE_COLUMN_FAMILY, COLUMN_KEY_OBJECT)){
		string value = col.column.value;
		node = this->createNode(uid);
		tiberius::deserialize(value, *node);
	
	  }
#else
    if (this->fileNameMap.find(uid) != this->fileNameMap.end()) {
      cout << "About to deserialize node: " << uid << endl;
      stringstream ss;
      ss << "/rt/store/suffixtree/" << this->fileNameMap[uid];
      tiberius::deserialize(ss.str(), node);
    }
#endif			
  }
  return node;
}
