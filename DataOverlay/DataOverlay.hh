/**
 * \file DataOverlay.h
 *
 * 
 * \brief Little sample program for establishing a user analysis space.
 *
 * @author wketchum
*/

#ifndef TEST_USERANALYSIS_H
#define TEST_USERANALYSIS_H

#include <string>

#include "TTree.h"

namespace mix{
  class DataOverlay;
}

class mix::DataOverlay{
  
public:
  
  /// Default constructor
  DataOverlay();

  /// Default destructor
  virtual ~DataOverlay(){};

  void RunAnalysis();
  void SetupOutputTree(TTree*);
  
 private:

  std::string fAlgName;
  TTree*      fTree;
  
  void PrintInfo();

  
};

#endif
