#include <iostream>
#include <sys/types.h>
#include <dirent.h>

#include "boost/filesystem.hpp"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
using namespace std;

#include "freeling.h"
#include "freeling/traces.h"

#include "Keywords/CompositeWord.h"
#include "Keywords/KeyPhrase.h"
#include "SuffixTree/SuffixTree.h"

// predeclarations
void PrintMorfo(list<sentence> &ls, map<string,int> &noun_freq);
void docCount(list<sentence> &ls, map<string,int> &noun_doc_freq, map<string, int> &noun_freq);
void PrintTree(list<sentence> &ls);
void PrintDepTree(list<sentence> &ls);
//map<string, int> noun_freq;

/////////   MAIN SAMPLE PROGRAM  -- begin

void readArticle(string fileName, string &article) {
  ifstream ifs(fileName.c_str());
  string line;
  string data;
  while(getline(ifs, line)) {
    data.append(line);
  }
  //cout << data << endl;
  vector<string> splitVec;
  //boost::split(splitVec, data, boost::is_any_of("\t"));
  boost::split(splitVec, data, boost::is_any_of("|"));
  //string article(splitVec[1]);
  article.append(splitVec[1]);
  //article.append(".  ");
  //article.append(splitVec[3]);
}

int main(int argc, char **argv) {
  boost::filesystem::path corpusPath(argv[1]);
  string text(argv[1]);
  //list<word> lw;
  //list<sentence> ls;

  string path="/usr/local/share/FreeLing/en/";

  // morphological analysis has a lot of options, and for simplicity they are packed up
  // in a maco_options object. First, create the maco_options object with default values.
  maco_options opt("en");  
  // then, set required options on/off  
  opt.QuantitiesDetection = false;  //deactivate ratio/currency/magnitudes detection 
  opt.AffixAnalysis = true; opt.MultiwordsDetection = true; opt.NumbersDetection = true; 
  opt.PunctuationDetection = true; opt.DatesDetection = true; opt.QuantitiesDetection = false; 
  opt.DictionarySearch = true; opt.ProbabilityAssignment = true; opt.NERecognition = NER_BASIC;   
  // alternatively, you can set active modules in a single call:
  //     opt.set_active_modules(true, true, true, true, true, false, true, true, 0);
  
  // and provide files for morphological submodules. Note that it is not necessary
  // to set opt.QuantitiesFile, since Quantities module was deactivated.
  opt.LocutionsFile=path+"locucions.dat"; opt.AffixFile=path+"afixos.dat";
  opt.ProbabilityFile=path+"probabilitats.dat"; opt.DictionaryFile=path+"maco.db";
  opt.NPdataFile=path+"np.dat"; opt.PunctuationFile=path+"../common/punct.dat"; 
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
  chart_parser parser(path+"grammar-dep.dat");
  // create dependency parser 
  dep_txala dep(path+"dep/dependences.dat", parser.get_start_symbol());
  


  int maxDocCount = 500;
  int numDocs=0;  
  map<string, int> noun_doc_freq;
  map<string, int> noun_freq;
  string data_type("corpus");
  NLP::SuffixTree::SuffixTree stree(data_type);
  for (boost::filesystem::directory_iterator it(corpusPath); it != boost::filesystem::directory_iterator(); ++it) {
    if (numDocs > maxDocCount) {
      break;
    }
    // create analyzers
    tokenizer tk(path+"tokenizer.dat"); 
    splitter sp(path+"splitter.dat");
    
    list<word> lw;
    list<sentence> ls;    
    boost::filesystem::directory_entry entry = *it;
    string article;
    readArticle(entry.path().string(), article);
    //cout << article << endl;

    lw=tk.tokenize(article);

    // accumulate list of words in splitter buffer, returning a list of sentences.
    // The resulting list of sentences may be empty if the splitter has still not 
    // enough evidence to decide that a complete sentence has been found. The list
    // may contain more than one sentence (since a single input line may consist 
    // of several complete sentences).
    ls=sp.split(lw, false);
    cout << "Number of sentences: " << ls.size() << endl;
    // perform and output morphosyntactic analysis and disambiguation
    morfo.analyze(ls);
    tagger.analyze(ls);
    /*
    for (list<sentence>::iterator it=ls.begin(); it!= ls.end(); it++) {
      vector<word> sent = it->get_words();
      string uid = entry.path().string();
      stree.addSentence(sent, uid);
    }
    */
    docCount(ls, noun_doc_freq, noun_freq);
    numDocs++;
    cout << "finished : " << numDocs << " files ..." << endl;
  }
    for (map<string, int>::iterator it=noun_freq.begin(); it!=noun_freq.end(); it++) {
    cout << it->first << " : " << it->second << endl;
  }

  cout << "Enter a file to analyze: " << endl;
  string filename;
  while (getline(cin,filename)) {
    if (filename.size() <= 0) {
      cout << "Please select a file to analyze: " << endl;
      continue;      
    }else if (!boost::filesystem::exists(filename)) {
      cout << "File does not exist. Please select another file to analyze: " << endl;
      continue;
    }
    // create analyzers
    tokenizer tk(path+"tokenizer.dat"); 
    splitter sp(path+"splitter.dat");

    list<word> lw;
    list<sentence> ls;

    string article;
    readArticle(filename, article);
    cout << article << endl;
    lw=tk.tokenize(article);
    cout << "number of words: " << lw.size() << endl;
    // accumulate list of words in splitter buffer, returning a list of sentences.
    // The resulting list of sentences may be empty if the splitter has still not 
    // enough evidence to decide that a complete sentence has been found. The list
    // may contain more than one sentence (since a single input line may consist 
    // of several complete sentences).
    ls=sp.split(lw, false);
    cout << "sentence size: " << ls.size() << endl;
    // perform and output morphosyntactic analysis and disambiguation
    morfo.analyze(ls);
    tagger.analyze(ls);
    cout << "sentence size: " << ls.size() << endl;
    map<string, int> articlePhrases;
    PrintMorfo(ls, articlePhrases);
    cout << "article phrases: " << articlePhrases.size() << endl;
    priority_queue<Keywords::KeyPhrase> pq;
    for (map<string, int>::iterator it=articlePhrases.begin(); it!=articlePhrases.end(); it++) {
      if (noun_doc_freq.find(it->first) != noun_doc_freq.end()) {
	double idf = log(numDocs / (double) noun_doc_freq[it->first]);
	double lambda = (double) noun_freq[it->first]/(double) numDocs;
	double ridf = idf +log(1-lambda);
	//double mi = ((double) it->second / (double) lw.size());
	//double tfidf = ((double) it->second * idf);
	Keywords::KeyPhrase kp(it->first, ridf);
	pq.push(kp);
	//cout << it->first << " : tfidf: " << tfidf << " : " << it->second << " total: " << noun_freq[it->first] << endl;
      }else{
	//cout << it->first << " : " << it->second << " phrase was not found in corpus." << endl;
      }
    }
    while (!pq.empty()) {
      Keywords::KeyPhrase kp = pq.top();
      cout << kp.getPhrase() << " : " << kp.getTfidf() << endl;
      pq.pop();
    }
  }
  if (1) {
    return 0;
  }
  /*
    // tokenize input line into a list of words
  lw=tk.tokenize(text);
    
    // accumulate list of words in splitter buffer, returning a list of sentences.
    // The resulting list of sentences may be empty if the splitter has still not 
    // enough evidence to decide that a complete sentence has been found. The list
    // may contain more than one sentence (since a single input line may consist 
    // of several complete sentences).
    ls=sp.split(lw, false);
    
    // perform and output morphosyntactic analysis and disambiguation
    
    morfo.analyze(ls);
    tagger.analyze(ls);
    PrintMorfo(ls);
  */
    /*
    // perform and output Chunking
    parser.analyze(ls);
    PrintTree(ls);

    // Dep. parsing
    dep.analyze(ls);
    PrintDepTree(ls);
    
    // clear temporary lists;
    lw.clear(); ls.clear();    
    //}
  
  // No more lines to read. Make sure the splitter doesn't retain anything  
  ls=sp.split(lw, true);   
 
  // analyze sentence(s) which might be lingering in the buffer, if any.
  morfo.analyze(ls);
  tagger.analyze(ls);
  PrintMorfo(ls);

  parser.analyze(ls);
  PrintTree(ls);

  dep.analyze(ls);
  PrintDepTree(ls);
    */
}

/////////   MAIN SAMPLE PROGRAM  -- end


//----------------------------------
/// Result processing functions
//----------------------------------


//---------------------------------------------
// print morphological information
//---------------------------------------------

void PrintMorfo(list<sentence> &ls, map<string, int> &noun_freq) {
  word::const_iterator a;
  sentence::const_iterator w;
  
  // print sentence XML tag
  for (list<sentence>::iterator is=ls.begin(); is!=ls.end(); is++) {

    //cout<<"<SENT>"<<endl;
    // for each word in sentence
    Keywords::CompositeWord cw;
    for (w=is->begin(); w!=is->end(); w++) {
      // print word form, with PoS and lemma chosen by the tagger
      if (w->get_parole().substr(0,2) != "NN") {
	if (cw.size() > 0) {
	  noun_freq[cw.toString()]++;
	  cw.clear();
	}
	continue;
      }
      cw.addWord(*w);
      //noun_freq[w->get_form()]++;
      //cout << w->get_form() << endl;
      /*
      cout<<"  <WORD form=\""<<w->get_form();
      cout<<"\" lemma=\""<<w->get_lemma();
      cout<<"\" pos=\""<<w->get_parole();
      string en("en");
      cout<<"\" shortpos=\""<<w->get_short_parole(en);
      cout<<"\">"<<endl;
      */
      // for each possible analysis in word, output lemma, parole and probability
      /*
      for (a=w->analysis_begin(); a!=w->analysis_end(); ++a) {
	
	// print analysis info
	cout<<"    <ANALYSIS lemma=\""<<a->get_lemma();
	cout<<"\" pos=\""<<a->get_parole();
	cout<<"\" prob=\""<<a->get_prob();
	cout<<"\"/>"<<endl;
      }
      */
      // close word XML tag after list of analysis
      //cout<<"  </WORD>"<<endl;
    }
    /*
    for (map<string, int>::iterator it=noun_freq.begin(); it!=noun_freq.end(); it++) {
      cout << it->first << " : " << it->second << endl;
    }
    */
    
    // close sentence XML tag
    //cout<<"</SENT>"<<endl;
  }
}

void docCount(list<sentence> &ls, map<string, int> &noun_doc_freq, map<string, int> &noun_freq) {
  word::const_iterator a;
  sentence::const_iterator w;
  
  // print sentence XML tag
  set<string> wordsFoundInDoc;
  for (list<sentence>::iterator is=ls.begin(); is!=ls.end(); is++) {

    //cout<<"<SENT>"<<endl;
    // for each word in sentence
    Keywords::CompositeWord cw;
    for (w=is->begin(); w!=is->end(); w++) {
      // print word form, with PoS and lemma chosen by the tagger
      if (w->get_parole().substr(0,2) != "NN") {
	if (cw.size() > 0) {
	  string phrase = cw.toString();
	  if (wordsFoundInDoc.find(phrase) == wordsFoundInDoc.end()) {
	    noun_doc_freq[phrase]++;
	    wordsFoundInDoc.insert(phrase);
	  }
	  noun_freq[phrase]++;
	  cw.clear();
	}
	continue;
      }
      cw.addWord(*w);
    }
  }
}


//---------------------------------------------
// print syntactic tree
//---------------------------------------------

void rec_PrintTree(parse_tree::iterator n, int depth) {
  parse_tree::sibling_iterator d;
  
  cout<<string(depth*2,' '); 
  if (n->num_children()==0) { 
    if (n->info.is_head()) { cout<<"+";}
    word w=n->info.get_word();
    cout<<"("<<w.get_form()<<" "<<w.get_lemma()<<" "<<w.get_parole();
    cout<<")"<<endl;
  }
  else { 
    if (n->info.is_head()) { cout<<"+";}
    cout<<n->info.get_label()<<"_["<<endl;
    for (d=n->sibling_begin(); d!=n->sibling_end(); ++d)
      rec_PrintTree(d, depth+1);
    cout<<string(depth*2,' ')<<"]"<<endl;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - -
void PrintTree(list<sentence> &ls) {

  cout<<"----------- CHUNKING -------------"<<endl; 
  for (list<sentence>::iterator is=ls.begin(); is!=ls.end(); is++) {
    parse_tree &tr = is->get_parse_tree();
    rec_PrintTree(tr.begin(), 0);
  }
}

//---------------------------------------------
// print dependency tree
//---------------------------------------------

void rec_PrintDepTree(dep_tree::iterator n, int depth) {
  dep_tree::sibling_iterator d,dm;
  int last, min;
  bool trob;
  
    cout<<string(depth*2,' '); 

    cout<<n->info.get_link()->info.get_label()<<"/"<<n->info.get_label()<<"/";
    word w=n->info.get_word();
    cout<<"("<<w.get_form()<<" "<<w.get_lemma()<<" "<<w.get_parole()<<")";  

    if (n->num_children()>0) { 
       cout<<" ["<<endl; 

       //Print Nodes
       for (d=n->sibling_begin(); d!=n->sibling_end(); ++d)
	 if(!d->info.is_chunk())
	   rec_PrintDepTree(d, depth+1);

       // print CHUNKS (in order)
       last=0; trob=true;
       while (trob) { 
	 // while an unprinted chunk is found look, for the one with lower chunk_ord value
	 trob=false; min=9999;  
	 for (d=n->sibling_begin(); d!=n->sibling_end(); ++d) {
	   if(d->info.is_chunk()) {
	     if (d->info.get_chunk_ord()>last && d->info.get_chunk_ord()<min) {
	       min=d->info.get_chunk_ord();
	       dm=d;
	       trob=true;
	     }
	   }
	 }
	 if (trob) rec_PrintDepTree(dm, depth+1);
	 last=min;
       }

       cout<<string(depth*2,' ')<<"]"; 
    }
    cout<<endl;
}

// - - - - - - - - - - - - - - - - - - - - - - -
void PrintDepTree(list<sentence> &ls) {

  cout<<"----------- DEPENDENCIES -------------"<<endl;
  for (list<sentence>::iterator is=ls.begin(); is!=ls.end(); is++) {
    dep_tree &dep = is->get_dep_tree();
    rec_PrintDepTree(dep.begin(), 0);
  }
}


