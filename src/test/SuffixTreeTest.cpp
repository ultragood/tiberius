#include "gtest/gtest.h"
#include <iostream>
#include <sys/types.h>
#include <dirent.h>

#include "../SuffixTree/SuffixTree.h"

#include "boost/filesystem.hpp"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "freeling.h"
#include "freeling/traces.h"
#include "../Keywords/CompositeWord.h"

using namespace std;

//namespace {
//class SuffixTreeTest : public ::testing::Test {
class SuffixTreeTest {
public:
    SuffixTreeTest() { 
      string article;
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
      for (unsigned int i=0; i<100; i++) {
	stringstream ss;
	ss << i;
	string filename(dir);
	filename.append(ss.str()).append(".txt");
	cout << "Processing: " << filename << endl;
	this->readArticle(filename, article);
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
	for (list<sentence>::iterator it=ls.begin(); it!= ls.end(); it++) {
	  vector<word> sent = it->get_words();
	  string uid(ss.str()+".txt");
	  stree.addSentence(sent, uid);
	}
      }
    }

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

    virtual ~SuffixTreeTest() { }
    virtual void setUp() { }

    virtual void tearDown() { }

    NLP::SuffixTree::SuffixTree stree;
  };


  TEST(SuffixTreeTest, isPhraseFindsEuropeanCentralBank) {
    SuffixTreeTest stt;
    cout << "Hello " << endl;
    string phrase("European Central Bank");
    //stt.stree.displaySuffixTree();
    double ridf = stt.stree.isPhrase(phrase);
    cout << "ridf of " << phrase << ": " << ridf << endl;
    string phrase2("European");
    ridf = stt.stree.isPhrase(phrase2);
    cout << "ridf of " << phrase2 << ": " << ridf << endl;
  }
  //}

/*
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
*/
