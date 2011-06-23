#include "STNode.h"
#include <iostream>

NLP::SuffixTree::STNode::STNode(NLP::SuffixTree::SuffixTree *tree, unsigned long uid) {
    this->tree = tree;
    this->wordUid = uid;
    this->parent = NULL;
    this->level = 0;
    this->root = uid == 0 ? true : false;
    this->count = 0;
}

NLP::SuffixTree::STNode::~STNode() { }

void NLP::SuffixTree::STNode::addStoryId(string uid) {
    stories[uid]++;
}

map<string, long> NLP::SuffixTree::STNode::getStories() {
    return stories;
}

unsigned long NLP::SuffixTree::STNode::getNumberOfStories() {
    return this->stories.size();
}

unsigned long NLP::SuffixTree::STNode::getNumberOfChildren() {
    return this->children.size();
}

vector<NLP::SuffixTree::STNode *> * NLP::SuffixTree::STNode::getChildren() {
    return &children;
}

NLP::SuffixTree::STNode * NLP::SuffixTree::STNode::getChildNode(int wordId) {
  NLP::SuffixTree::STNode *result = NULL;
  if (this->wordChildMap.find(wordId) != this->wordChildMap.end()) {
    result = this->wordChildMap[wordId];
  }
  return result;
}

string NLP::SuffixTree::STNode::getWord() {
    return this->tree->getString(this->wordUid);
}

string NLP::SuffixTree::STNode::getPhrase() {
    NLP::SuffixTree::STNode *node = this;
    string phrase;
    while (node->parent) {
        phrase = node->getWord() + " " + phrase;
        node = node->parent;
    }
    return phrase;
}

int NLP::SuffixTree::STNode::getFrequencyCount() {
  return this->count;
}

NLP::SuffixTree::STNode * NLP::SuffixTree::STNode::createNodeIfNeeded(unsigned long uid) {
    NLP::SuffixTree::STNode *node = NULL;
    map<int, STNode *>::iterator it=(this->wordChildMap).find(uid);
    if (it == this->wordChildMap.end()) {
        node = this->tree->createNode(uid);
        this->wordChildMap[uid] = node;
        this->children.push_back(node);
        node->parent = this;
        string parentWord;
        if (node->parent) {
            node->level = node->parent->level + 1;
            parentWord = this->tree->getString(node->parent->wordUid);
        }
    } else {
        node = this->wordChildMap[uid];
    }
    return node;
}

int NLP::SuffixTree::STNode::getLevel() {
    return this->level;
}

NLP::SuffixTree::STNode* NLP::SuffixTree::STNode::getParent() {
  return this->parent;
}

void NLP::SuffixTree::STNode::incrementFrequencyCount() {
  this->count++;
}

void NLP::SuffixTree::STNode::print() {
  string s = this->tree->getString(this->wordUid);
  int level = getLevel();
  for (int i = 0; i < level-1; ++i) {
    cout << "  ";
  }
  cout << "--|" << s;
  if (!this->stories.empty()) {
    cout << " ( Stories: ";
    for (map<string,long>::iterator it = this->stories.begin(); it != this->stories.end(); ++it) {
      string storyId = it->first;
      NLP::SuffixTree::STStory story = this->tree->getStory(storyId);
      cout << storyId << "(level: " << level << ") (" << story.getArcCount() << ")";
      if (this->w.get_form().size() > 0) {
	cout << "(pos: " << this->w.get_parole() << ")";
      }
    }
    cout << ")";
  }
  cout << endl;
  for (vector<NLP::SuffixTree::STNode *>::iterator it = children.begin(); it != children.end(); ++it) {
    NLP::SuffixTree::STNode *node = *it;
    node->print();
  }
}
