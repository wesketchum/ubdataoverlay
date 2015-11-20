////////////////////////////////////////////////////////////////////////
// Class:       RawDigitOverlayMicroBooNEAna
// Module Type: analyzer
// File:        RawDigitOverlayMicroBooNEAna_module.cc
//
// Generated at Tue Nov 10 13:06:09 2015 by Wesley Ketchum using artmod
// from cetpkgsupport v1_08_07.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "art/Framework/Services/Optional/TFileService.h"

#include "TTree.h"

#include "DataOverlay/RawDigitAdderAna.hh"

namespace mix {
  class RawDigitOverlayMicroBooNEAna;
}

class mix::RawDigitOverlayMicroBooNEAna : public art::EDAnalyzer {
public:
  explicit RawDigitOverlayMicroBooNEAna(fhicl::ParameterSet const & p);
  // The destructor generated by the compiler is fine for classes
  // without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  RawDigitOverlayMicroBooNEAna(RawDigitOverlayMicroBooNEAna const &) = delete;
  RawDigitOverlayMicroBooNEAna(RawDigitOverlayMicroBooNEAna &&) = delete;
  RawDigitOverlayMicroBooNEAna & operator = (RawDigitOverlayMicroBooNEAna const &) = delete;
  RawDigitOverlayMicroBooNEAna & operator = (RawDigitOverlayMicroBooNEAna &&) = delete;

  // Required functions.
  void analyze(art::Event const & e) override;

  // Selected optional functions.
  void reconfigure(fhicl::ParameterSet const & p) override;

private:

  // Declare member data here.
  mix::RawDigitAdderAna fAnaAlg;
  
};


mix::RawDigitOverlayMicroBooNEAna::RawDigitOverlayMicroBooNEAna(fhicl::ParameterSet const & p)
  :
  EDAnalyzer(p)  // ,
 // More initializers here.
{
}

void mix::RawDigitOverlayMicroBooNEAna::analyze(art::Event const & e)
{
  art::ServiceHandle<art::TFileService> tfs;
}

void mix::RawDigitOverlayMicroBooNEAna::reconfigure(fhicl::ParameterSet const & p)
{
  // Implementation of optional member function here.
}

DEFINE_ART_MODULE(mix::RawDigitOverlayMicroBooNEAna)