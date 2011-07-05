#include "SuffixTree.h"

#include <stdio.h> 

#include "mmap/SuffixTree.h"

#include "freeling.h"
#include "freeling/traces.h"
#include <iostream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

/*
#include <stdlib.h> 
#include <stdio.h> 
#include <fcntl.h> 
#include <sys/mman.h> 
#include <sys/stat.h> 
*/

char * tiberius::mmap::SuffixTree::ROOT_NAME= (char *) "__ROOT__";

  /*  
tiberius::mmap::SuffixTree::SuffixTree(void *memoryFile) : memoryFile(memoryFile) {
  this->offset = 0;
  this->llsize = sizeof(tiberius::mmap::LinkedList);
  tiberius::mmap::LinkedList *list = (tiberius::mmap::LinkedList *) memoryFile;
  strcpy(list->s,ROOT_NAME);
  list->next = NULL;
}
*/


tiberius::mmap::SuffixTree::SuffixTree(void *memoryFile, SuffixTreeVars *globalVars) : memoryFile(memoryFile), globalVars(globalVars) {
  this->llsize = sizeof(tiberius::mmap::Node);
  if (!globalVars->root) {
    cout << "globalVars->root == NULL" << endl;
    tiberius::mmap::Node *root = (tiberius::mmap::Node *) memoryFile+this->globalVars->offset;
    strcpy(root->term,ROOT_NAME);
    root->pos[0] = '\0';
    root->childList = NULL;
    root->lastChild = NULL;
    this->globalVars->offset = this->globalVars->offset+this->llsize;
    globalVars->root = root;
    cout << "root term: " << root->term << endl;
  }
}

tiberius::mmap::Node * tiberius::mmap::SuffixTree::getChildNode(tiberius::mmap::Node *node, string &candidateTerm) {
  tiberius::mmap::Node *childNode = NULL;
  tiberius::mmap::LinkedList *childList = node->childList;
  while((childNode = childList->node) != NULL) {
    string nodeTerm(childNode->term);
    if (nodeTerm == candidateTerm) {
      break;
    }
  }
  return childNode;
}

double tiberius::mmap::SuffixTree::_ridf(tiberius::mmap::Node *node) {
  unsigned int wf = node->frequencyCount;
  unsigned int df = node->docCount;
  unsigned long numDocs = this->globalVars->docCount;
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

double tiberius::mmap::SuffixTree::calcInformativeness(string &phrase) {
  vector<string> words;
  tiberius::mmap::Node *node = this->globalVars->root;
  boost::split(words, phrase, boost::is_any_of(" "));
  bool found = true;
  for (unsigned int i=0; i<words.size(); i++) {
    tiberius::mmap::Node *childNode = this->getChildNode(node, words[i]);
    if (childNode == NULL) {
      found = false;
      break;
    }
    if (i == 0) {
      // make sure the first word is a noun or verb
      string pos(childNode->pos);
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
    string pos(node->pos);
    if (pos.size() >= 2 && (pos.substr(0,2) == "NN")) {
      ridf = this->_ridf(node);
    }
  }
  return ridf;
}

void tiberius::mmap::SuffixTree::parseSentence(list<sentence> &sentences) {
  for (list<sentence>::iterator it=sentences.begin(); it!= sentences.end(); it++) {
    vector<word> sentence = it->get_words();
    for (unsigned int i=0; i<sentence.size(); i++) {
      string parent = "__ROOT__";
      unsigned int levelCount=0;
      for (unsigned int j=i; j<sentence.size(); j++) {
	map<string, tiberius::mmap::WordAttributes *> *level = NULL;
	string term = sentence[j].get_form();
	string pos = sentence[j].get_parole();
	tiberius::mmap::WordAttributes *wa = NULL;
	if (this->levels.size() > 0 && this->levels.size() > j) {
	  level = this->levels[levelCount];
	  if (level->find(term) == level->end()) {
	    wa = new tiberius::mmap::WordAttributes();
	    (*level)[term] = wa;
	    wa->frequencyCount = 0;
	    wa->pos = pos;
	  }else{
	    wa = (*level)[term];
	  }
	  wa->frequencyCount++;
	  wa->parent = parent;
	  //level[term] = wa;
	  this->levels[levelCount] = level;
	}else{
	  wa = new tiberius::mmap::WordAttributes();
	  wa->frequencyCount=1;
	  wa->pos = pos;
	  wa->parent = parent;
	  if (!level) {
	    level = new map<string, tiberius::mmap::WordAttributes *>();
	  }
	  (*level)[term] = wa;
	  this->levels.push_back(level);
	}
	levelCount += 1;
	parent = term;
      }
    }
  }
}

void tiberius::mmap::SuffixTree::addStory(list<sentence> &sentences) {
  parseSentence(sentences);
}

void tiberius::mmap::SuffixTree::persist() {
  long offset = this->globalVars->offset;  
  tiberius::mmap::Node *node = NULL;
  cout << "about to write to file ..." << endl;
  map<string, tiberius::mmap::Node *> nodeMap;
  nodeMap[string("__ROOT__")] = this->globalVars->root;
  for (unsigned int i=0; i<this->levels.size(); i++) {

    //    if (node == NULL) {
    //  node = this->globalVars->root;
    //}else{
    //  node = 
    cout << "Writing level " << i << endl;
    map<string, tiberius::mmap::WordAttributes *> *level = this->levels[i];
    for (map<string, tiberius::mmap::WordAttributes *>::iterator it=level->begin(); it!=level->end(); it++) {
      string term = it->first;
      tiberius::mmap::WordAttributes *wa = it->second;
      node = nodeMap[wa->parent];
      //      cout << "Going to write out children for node: " << wa->parent << endl;
      tiberius::mmap::LinkedList *childList = NULL;
      if (!node->childList) {
	tiberius::mmap::LinkedList *newChild = (tiberius::mmap::LinkedList *) this->memoryFile+offset;
	newChild->next = NULL;
	if (node) {
	  node->childList = newChild;
	  node->lastChild = newChild;
	}
	childList = newChild;
      }else{
	childList = node->lastChild;
	tiberius::mmap::LinkedList *newChild = (tiberius::mmap::LinkedList *) this->memoryFile+offset;
	// init next to NULL;
	newChild->next = NULL;
	childList->next = newChild;
	childList = newChild;
	if (node) {
	  node->lastChild = childList;
	}
      }
      offset = offset + sizeof(tiberius::mmap::LinkedList);
      tiberius::mmap::Node *child = (tiberius::mmap::Node *) this->memoryFile+offset;
      offset = offset + sizeof(tiberius::mmap::Node);
      strcpy(child->term, term.c_str());
      child->level = i;
      child->frequencyCount = wa->frequencyCount;
      childList->node = child;
      node = node->childList->node;
      //cout << "Adding term: " << term << " to nodeMap ... " << endl;
      nodeMap[term] = node;
    }
  }
  this->globalVars->offset = offset;
  
  /*
  long offset = this->globalVars->offset;
  set<tiberius::mmap::Node *> nodes;
  for (list<sentence>::iterator it=sentences.begin(); it!= sentences.end(); it++) {
    vector<word> sent = it->get_words();
    //this->globalVars->docCount++;
    for (unsigned int k=0; k<sent.size() && k<10; k++) {
      tiberius::mmap::Node *node = this->globalVars->root;
      if (nodes.find(node) == nodes.end()) {
	nodes.insert(node);
	node->docCount++;
      }
      for (unsigned int i=k; i<sent.size(); i++) {
      tiberius::mmap::LinkedList *childList = NULL;
      if (!node->childList) {
	//node->childList = (tiberius::mmap::LinkedList *) this->memoryFile+offset;
	//node->childList->next = NULL;
	//childList = node->childList;
	tiberius::mmap::LinkedList *newChild = (tiberius::mmap::LinkedList *) this->memoryFile+offset;
	// init next to NULL;
	newChild->next = NULL;
	node->childList = newChild;
	node->lastChild = newChild;
	childList = newChild;
      }else{
	//childList = node->childList;
	//while(childList->next) {
	//  childList = childList->next;
	//}
	childList = node->lastChild;
	tiberius::mmap::LinkedList *newChild = (tiberius::mmap::LinkedList *) this->memoryFile+offset;
	// init next to NULL;
	newChild->next = NULL;
	childList->next = newChild;
	childList = newChild;
	node->lastChild = childList;
      }
      offset = offset + sizeof(tiberius::mmap::LinkedList);
      tiberius::mmap::Node *child = (tiberius::mmap::Node *) this->memoryFile+offset;
      offset = offset + sizeof(tiberius::mmap::Node);
      strcpy(child->term, sent[i].get_form().c_str());
      child->level = i+1;
      child->frequencyCount++;
      childList->node = child;
      node = node->childList->node;
      //node = node->children;
      // add to offset the size of a linkedlist element
      //this->globalVars->offset = this->globalVars->offset+this->llsize;
      //      cout << sent[i].get_form() << " ";
    }
    }
    //sents.push_back(sent);
  }
  this->globalVars->offset = offset;
*/
}


