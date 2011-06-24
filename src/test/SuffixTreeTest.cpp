#include "gtest/gtest.h"
#include <iostream>
#include <sys/types.h>
#include <dirent.h>

#include "../SuffixTree/SuffixTree.h"
#include "../Keywords/Phrase.h"

#include "boost/filesystem.hpp"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "freeling.h"
#include "freeling/traces.h"
#include "../Keywords/CompositeWord.h"
#include <boost/algorithm/string/split.hpp>
#include <queue>

using namespace std;

//namespace {
//class SuffixTreeTest : public ::testing::Test {
class SuffixTreeTest {
public:
    SuffixTreeTest() {
      if (stree) {
	return;
      }
      stree = new NLP::SuffixTree::SuffixTree();
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
	  stree->addSentence(sent, uid);
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

  virtual ~SuffixTreeTest() { if (stree) delete stree; stree = NULL; }
    virtual void setUp() { }

    virtual void tearDown() { }

    static NLP::SuffixTree::SuffixTree *stree;
  };

NLP::SuffixTree::SuffixTree * SuffixTreeTest::stree = NULL;
SuffixTreeTest stt;

TEST(SuffixTreeTest, ridfFindsEuropeanCentralBank) {
  //SuffixTreeTest stt;
  string phrase("European Central Bank");
  double ridf1 = SuffixTreeTest::stree->calcInformativeness(phrase);
  string phrase2("European");
  double ridf2 = SuffixTreeTest::stree->calcInformativeness(phrase2);
  EXPECT_TRUE(ridf1 > ridf2);
}


TEST(SuffixTreeTest, findPhrases) {
  string s1("huge");
  string s2("budget");
  string s3("budget deficit");
  string s4("huge budget deficit");
  double budget = SuffixTreeTest::stree->calcInformativeness(s2);
  double budget_deficit = SuffixTreeTest::stree->calcInformativeness(s3);
  EXPECT_TRUE(SuffixTreeTest::stree->calcInformativeness(s1) == 0.0);
  EXPECT_TRUE(budget > 0.0);
  EXPECT_TRUE(budget_deficit > budget);
  EXPECT_TRUE(SuffixTreeTest::stree->calcInformativeness(s4) == 0.0);
  //cout << s1 << " ridf: " << SuffixTreeTest::stree->calcInformativeness(s1) << endl;
  //cout << s2 << " ridf: " << SuffixTreeTest::stree->calcInformativeness(s2) << endl;
  //cout << s3 << " ridf: " << SuffixTreeTest::stree->calcInformativeness(s3) << endl;
  //cout << s4 << " ridf: " << SuffixTreeTest::stree->calcInformativeness(s4) << endl;
}


TEST(SuffixTreeTest, findPhrases2) {
  string s1("Lehman");
  string s2("Lehman Brothers");
  string s3("Lehman Brothers Investment");
  double l_ridf = SuffixTreeTest::stree->calcInformativeness(s1);
  double lb_ridf = SuffixTreeTest::stree->calcInformativeness(s2);
  double lbi_ridf = SuffixTreeTest::stree->calcInformativeness(s3);
  // because they appear same number of times.
  EXPECT_TRUE(l_ridf == lb_ridf);
  // because it appears in less then 0.05% of the corpus or only one time.
  EXPECT_TRUE(lbi_ridf == 0.0);
}

void getPhrase(vector<string> &words, unsigned int &start, unsigned int &end, string &phrase) {
  for (unsigned int i=start; i<end; i++) {
    if (i > start) {
      phrase.append(" ");
    }
    phrase.append(words[i]);
  }
}
    
TEST(SuffixTreeTest, analyzePhrase) {
  string sentence = "Syrian President Bashar al-Assad offered vague promises of reform and clear threats against protesters Monday as he addressed his nation and the rest of the world whose leaders called for swift changes some saying he had passed the point of no return";
  vector<string> words;
  boost::split(words, sentence, boost::is_any_of(" "));

  priority_queue<Keywords::Phrase> pq;
  for (unsigned int i=0; i<words.size(); i++) {
    for (unsigned int j=i; j<=words.size(); j++) {
      string phrase;
      if (i == j) {
	continue;
      }
      getPhrase(words, i, j, phrase);
      double ridf = SuffixTreeTest::stree->calcInformativeness(phrase);
      if (ridf > 0) {
	pq.push(Keywords::Phrase(phrase, ridf));
      }
    }
  }
  if (pq.empty()) {
    FAIL();
  }
  double ridf = pq.top().ridf;
  int tmp = (int) (ridf*100.0);
  ridf = (double) ((double) tmp / (double) 100.0);
  EXPECT_TRUE(pq.top().phrase == "Syrian");
  EXPECT_TRUE(ridf == 6.7);
  pq.pop();
  if (pq.empty()) {
    FAIL();
  }
  ridf = pq.top().ridf;
  tmp = (int) (ridf*100.0);
  ridf = (double) ((double) tmp / (double) 100.0);
  EXPECT_TRUE(pq.top().phrase == "Bashar");
  EXPECT_TRUE(ridf == 6.62);
  pq.pop();
  if (pq.empty()) {
    FAIL();
  }
  ridf = pq.top().ridf;
  tmp = (int) (ridf*100.0);
  ridf = (double) ((double) tmp / (double) 100.0);
  EXPECT_TRUE(pq.top().phrase == "threats");
  EXPECT_TRUE(ridf == 5.64);
  //  pq.top().phrase << " ridf: " << pq.top().ridf << endl;
  //while (!pq.empty()) {
  //  cout << 
  //  pq.pop();
  //}
}
  //

/*
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
*/
