#ifndef TEST_USERANALYSIS_CXX
#define TEST_USERANALYSIS_CXX

#include "DataOverlay.hh"
#include <iostream>

mix::DataOverlay::DataOverlay()
  : fAlgName("DataOverlay")
{}

void mix::DataOverlay::SetupOutputTree(TTree* tfs_tree){
  fTree = tfs_tree;

  std::string title = fAlgName + " Tree";
  fTree->SetObject(fTree->GetName(),title.c_str());
}

void mix::DataOverlay::RunAnalysis(){
  PrintInfo();
}

void mix::DataOverlay::PrintInfo(){
  std::cout << "\n================================== DataOverlay ==========================" << std::endl;
  std::cout << "This is a ub_DataOverlay class called " << fAlgName << std::endl;
  std::cout << "\tThere is an output tree called "
	    << fTree->GetName() << " (" << fTree->GetTitle() << ")" << std::endl;
  std::cout << "==========================================================================\n" << std::endl;
}

#endif
