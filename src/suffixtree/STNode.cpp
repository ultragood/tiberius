#include "STNode.h"
#include <iostream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

suffixtree::STNode::STNode(suffixtree::SuffixTree *tree, string &uid) {
  this->tree = tree;
//  vector<string> words;
//  boost::split(words, uid, boost::is_any_of(" "));    
//  this->term = words[words.size()-1];
  this->uid = uid;
  //this->parent = NULL;
  this->root = 0;
  this->level = 0;
  this->count = 0;
}

string suffixtree::STNode::getUid() {
  return this->uid;
}

/*
void suffixtree::STNode:constructtUid(vector<string> &words, string &uid) {
  string sep("_");
  for (unsigned int i=0; i<words.size(); i++) {
    if (i>0) {
      uid.append(sep);
    }
    uid.append(words[i]);
  }
}
*/

string suffixtree::STNode::getParentUid() {
  string uid;
  vector<string> words;
  boost::split(words, this->uid, boost::is_any_of("- "));    
  string sep("_");
  for (unsigned int i=0; i<words.size()-1; i++) {
    if (i>0) {
      uid.append(sep);
    }
    uid.append(words[i]);
  }
  return uid;
}

suffixtree::STNode::~STNode() { }

void suffixtree::STNode::addStoryId(string uid) {
    stories[uid]++;
}

unsigned long suffixtree::STNode::getNumberOfStories() {
    return this->stories.size();
}

suffixtree::STNode * suffixtree::STNode::getChildNode(string &term) {
  suffixtree::STNode *result = NULL;
  //string childUid = this->uid+"-"+term;
  //if (this->childWordSet.find(childUid) != this->childWordSet.end()) {
  //  result = this->tree->getNode(childUid);
  //}
//  if (this->childWordSet.find(term) != this->childWordSet.end()) {
    string childUid = this->uid+"-"+term;
    result = this->tree->getNode(childUid);
//  }
  return result;
}

string suffixtree::STNode::getWord() {
//  return this->term;
	return "";
}

int suffixtree::STNode::getFrequencyCount() {
  return this->count;
}

suffixtree::STNode * suffixtree::STNode::createNodeIfNeeded(string &word) {
    suffixtree::STNode *node = NULL;
    //map<string, STNode *>::iterator it=(this->wordChildMap).find(term);
    //if (it == this->wordChildMap.end()) {
    string uid = this->uid+"-"+word;
/*
    if (this->childWordSet.find(word) == this->childWordSet.end()) {
        node = this->tree->createNode(uid);
        //this->wordChildMap[term] = node;
        //this->wordChildMap[term] = term;
	this->childWordSet.insert(word);
        //node->parent = this;
        //if (node->parent) {
	node->level = this->level + 1;
	    //}
	node->parentTerm = this->term;
    } else {
*/
      //node = this->wordChildMap[term];
      node = this->tree->getNode(uid);
//    }
    return node;
}

int suffixtree::STNode::getLevel() {
    return this->level;
}


suffixtree::STNode* suffixtree::STNode::getParent() {
  string parentUid = this->getParentUid();
  return this->tree->getNode(parentUid);
}

void suffixtree::STNode::incrementFrequencyCount() {
  this->count++;
}

void suffixtree::STNode::print() {
  string s = this->uid;;
  int level = getLevel();
  for (int i = 0; i < level-1; ++i) {
    cout << "  ";
  }
  cout << "--|" << s;
  if (!this->stories.empty()) {
    cout << " ( Stories: ";
    for (map<string,long>::iterator it = this->stories.begin(); it != this->stories.end(); ++it) {
      string storyId = it->first;
      cout << storyId << "(level: " << level << ")";
      cout << "(pos: " << this->pos << ")";
    }
    cout << ")";
  }
  cout << endl;
  //for (map<string, suffixtree::STNode *>::iterator it = wordChildMap.begin(); it != wordChildMap.end(); ++it) {
  //  suffixtree::STNode *node = it->second;
  //  node->print();
  //}
}

  
