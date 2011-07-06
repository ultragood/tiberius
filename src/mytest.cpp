#include <stdlib.h> 
#include <stdio.h> 
#include <fcntl.h> 
#include <sys/mman.h> 
#include <sys/stat.h> 
#include <time.h> 
#include <unistd.h> 
#include <string.h>
#include "mmap/Node.h"
#include "mmap/SuffixTree.h"

#include "freeling.h"
#include "freeling/traces.h"
#include <iostream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/filesystem.hpp>

#define FILE_LENGTH 4*1024*1024*1024
 
using namespace std;


void readArticle(string fileName, string &article) {
  ifstream ifs(fileName.c_str());
  string line;
  string data;
  while(getline(ifs, line)) {
    data.append(line);
  }
  vector<string> splitVec;
  boost::split(splitVec, data, boost::is_any_of("|"));
  article=splitVec[1];
}

void getPhrase(vector<string> &words, unsigned int &start, unsigned int &end, string &phrase) {
  for (unsigned int i=start; i<end; i++) {
    if (i > start) {
      phrase.append(" ");
    }
    phrase.append(words[i]);
  }
}

int main (int argc, char* const argv[])  {
  int fd; 
  void* file_memory; 
  /* Prepare a file large enough to hold an unsigned integer. */ 
  bool fileExists = boost::filesystem::exists(argv[1]);
    
  fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); 
  lseek (fd, (long) FILE_LENGTH, SEEK_SET); 
  write (fd, "", 1); 
  lseek (fd, 0, SEEK_SET); 
  /* Create the memory mapping. */ 
  file_memory = mmap (0, (long) FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0); 
  int stvsize = sizeof(tiberius::mmap::SuffixTreeVars);
  tiberius::mmap::SuffixTreeVars *globalVars = (tiberius::mmap::SuffixTreeVars *) file_memory;
  if (!fileExists) {
    globalVars->docCount = 0;
    globalVars ->offset = 0;
    globalVars->root = NULL;
    globalVars->offset = stvsize;
  }else{
    cout << "Offset: " << globalVars->offset << endl;
    tiberius::mmap::Node *root = (tiberius::mmap::Node *) file_memory + globalVars->root_offset;
    cout << "node name: " << root->term << endl;
  }
  tiberius::mmap::SuffixTree stree(file_memory, globalVars);

    string dir("/home/mikep/work/tiberius/canned_data/cnn/");
    // freeling path.
    string path="/usr/local/share/FreeLing/en/";
    
    // morphological analysis has a lot of options, and for simplicity they are packed up
    // in a maco_options object. First, create the maco_options object with default values.
    maco_options opt("en");  
    // then, set required options on/off  
    opt.QuantitiesDetection = false;  //deactivate ratio/currency/magnitudes detection 
    opt.AffixAnalysis = true; 
    opt.MultiwordsDetection = true; 
    opt.NumbersDetection = true; 
    opt.PunctuationDetection = true; 
    opt.DatesDetection = true; 
    opt.QuantitiesDetection = false; 
    opt.DictionarySearch = true; 
    opt.ProbabilityAssignment = true; 
    opt.NERecognition = NER_BASIC;   
    // alternatively, you can set active modules in a single call:
    //     opt.set_active_modules(true, true, true, true, true, false, true, true, 0);
    
    // and provide files for morphological submodules. Note that it is not necessary
    // to set opt.QuantitiesFile, since Quantities module was deactivated.
    //opt.LocutionsFile=path+"locucions.dat"; 
    opt.AffixFile=path+"afixos.dat";
    opt.ProbabilityFile=path+"probabilitats.dat"; 
    opt.DictionaryFile=path+"maco.db";
    opt.NPdataFile=path+"tiberius-np.dat"; 
    opt.PunctuationFile=path+"../common/punct.dat"; 
    // alternatively, you can set the files in a single call:
    // opt.set_data_files(path+"locucions.dat", "", path+"afixos.dat",
    //                   path+"probabilitats.dat", opt.DictionaryFile=path+"maco.db",
    //                   path+"np.dat", path+"../common/punct.dat");
    
    // create the analyzer with the just build set of maco_options
    maco morfo(opt); 
    // create a hmm tagger for english (with retokenization ability, and forced 
    // to choose only one tag per word)
    hmm_tagger tagger("en", path+"tagger.dat", true, true); 
    // create chunker
    //chart_parser parser(path+"grammar-dep.dat");
    // create dependency parser 
    //dep_txala dep(path+"dep/dependences.dat", parser.get_start_symbol());

  if (!fileExists) {
    string article;
    for (unsigned int i=0; i<1; i++) {
      stringstream ss;
      ss << i;
      string filename(dir);
      filename.append(ss.str()).append(".txt");
      cout << "Processing: " << filename << endl;
      readArticle(filename, article);
      tokenizer tk(path+"tokenizer.dat"); 
      splitter sp(path+"splitter.dat");
      
      list<word> lw;
      list<sentence> ls;    
      lw=tk.tokenize(article);
      
      // accumulate list of words in splitter buffer, returning a list of sentences.
      // The resulting list of sentences may be empty if the splitter has still not 
      // enough evidence to decide that a complete sentence has been found. The list
      // may contain more than one sentence (since a single input line may consist 
      // of several complete sentences).
      ls=sp.split(lw, false);
      // perform and output morphosyntactic analysis and disambiguation
      morfo.analyze(ls);
      //tagger.analyze(ls);
      stree.addStory(ls);
    }
    stree.persist();
    //tiberius::mmap::Node *root = stree.getRoot();
    //tiberius::mmap::LinkedList *child = root->childList;
    /*
      do {
      //child = child->childList;
      if (child) {
      cout << child->node->term << endl;
    }
    //  }while((child = child->next));
  }while((child = child->node->childList));
  */
  
  }else{
    cout << "Enter a file to analyze: " << endl;
    string filename;
    string article;
    while (getline(cin,filename)) {
      if (filename.size() <= 0) {
	cout << "Please select a file to analyze: " << endl;
	continue;      
      }else if (!boost::filesystem::exists(dir+filename)) {
	cout << "File does not exist. Please select another file to analyze: " << endl;
	continue;
      }
      readArticle(dir+filename, article);
      tokenizer tk(path+"tokenizer.dat"); 
      splitter sp(path+"splitter.dat");
      
      list<word> lw;
      list<sentence> ls;    
      lw=tk.tokenize(article);
      
      // accumulate list of words in splitter buffer, returning a list of sentences.
      // The resulting list of sentences may be empty if the splitter has still not 
      // enough evidence to decide that a complete sentence has been found. The list
      // may contain more than one sentence (since a single input line may consist 
      // of several complete sentences).
      ls=sp.split(lw, false);
      // perform and output morphosyntactic analysis and disambiguation
      morfo.analyze(ls);

      set<string> phrases;
      for (list<sentence>::iterator it=ls.begin(); it!=ls.end(); it++) {
	vector<word> sent = it->get_words();
	vector<string> words;
	for (unsigned int i=0; i<sent.size(); i++) {
	  words.push_back(sent[i].get_form());
	}
	for (unsigned int i=0; i<words.size(); i++) {
	  for (unsigned int j=i; j<=words.size(); j++) {	
	    string phrase;
	    if (i == j) {
	      continue;
	    }
	    getPhrase(words, i, j, phrase);
	    if (phrases.find(phrase) == phrases.end()) {
	      phrases.insert(phrase);
	      double ridf = stree.calcInformativeness(phrase);
	      //if (ridf > 0) {
	      //pq.push(Keywords::Phrase(phrase, ridf));
	      cout << phrase << " ridf: " << ridf << endl;
	      //}else{
	      //	break;
	      //}
	    }
	  }
	}
      }
    } // while (getline...

  }
  // ------------------------------------------




  close (fd); 
  /* Release the memory (unnecessary because the program exits). */ 
  munmap (file_memory, (long) FILE_LENGTH); 
  return 0; 
} 

