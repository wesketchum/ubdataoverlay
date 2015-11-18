////////////////////////////////////////////////////////////////////////
// Class:       OverlayRawDataDetailMicroBooNE
// Module Type: producer
// File:        OverlayRawDataDetailMicroBooNE_module.cc
//
// This borrows a lot from the Mu2e mixing module:
//      EventMixing/src/MixMCEvents_module.cc
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Modules/MixFilter.h"
#include "art/Framework/IO/ProductMix/MixHelper.h"
#include "art/Framework/Core/PtrRemapper.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Persistency/Common/CollectionUtilities.h"
#include "art/Utilities/InputTag.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

//#ifndef BOOST_SYSTEM_NO_DEPRECATED
//#define BOOST_SYSTEM_NO_DEPRECATED 1
//#endif

//#include "boost/noncopyable.hpp"

#include <memory>
#include <string>

#include "DataOverlay/RawDigitAdder_HardSaturate.h"
#include "RawData/RawDigit.h"


namespace mix {
  class OverlayRawDataDetailMicroBooNE;
  typedef art::MixFilter<OverlayRawDataDetailMicroBooNE> OverlayRawDataMicroBooNE;
}

class mix::OverlayRawDataDetailMicroBooNE : public boost::noncopyable {
public:

  OverlayRawDataDetailMicroBooNE(fhicl::ParameterSet const& p,
				 art::MixHelper &helper);

  void startEvent(const art::Event&);  //called at the start of every event
  void finalizeEvent(art::Event &);    //called at the end of every event
  
  size_t nSecondaries() { return 1; } //We ALWAYS here have just one event we use.

  //void processEventIDs(art::EventIDSequence const& seq); //bookkepping for event IDs

  // Mixing Functions

  // For now, allow exactly one  input. Assume MC inputs have been merged
  // previously and one detsim output created if needed. This could be changed
  // but would require mixing functions for MC here.
  bool MixRawDigits( std::vector< std::vector<raw::RawDigit> const* > const& inputs,
  		     std::vector<raw::RawDigit> & output,
  		     art::PtrRemapper const &);
  //bool MixRawDigits( std::vector< std::vector<short> const* > const& inputs,
  //		     std::vector<short> & output,
  //		     art::PtrRemapper const &);

private:

  // Declare member data here.
  RawDigitAdder_HardSaturate fRDAdderAlg;

  fhicl::ParameterSet  fpset;
  short                fDefaultRawDigitSatPoint;
  std::string          fRawDigitInputModuleLabel;
};


mix::OverlayRawDataDetailMicroBooNE::OverlayRawDataDetailMicroBooNE(fhicl::ParameterSet const& p,
								    art::MixHelper &helper)
  :
  fRDAdderAlg(true),
  fpset(p.get<fhicl::ParameterSet>("detail")),
  fDefaultRawDigitSatPoint(fpset.get<short>("DefaultRawDigitSaturationPoint",4096)),
  fRawDigitInputModuleLabel(fpset.get<std::string>("RawDigitInputModuleLabel"))
{
  //If it produces something on its own, declare it here
  //helper.produces<>();

  helper.declareMixOp( art::InputTag(fRawDigitInputModuleLabel),
  		       &OverlayRawDataDetailMicroBooNE::MixRawDigits,
  		       *this );
}

//Initialize for each event
void mix::OverlayRawDataDetailMicroBooNE::startEvent(const art::Event& event) {

  fRDAdderAlg.SetSaturationPoint(fDefaultRawDigitSatPoint);
  fRDAdderAlg.SetScaleInputs(1.0,1.0);
  
}

//End each event
void mix::OverlayRawDataDetailMicroBooNE::finalizeEvent(art::Event& event) {
  //Nothing to be done?
}

bool MixRawDigits( std::vector< std::vector<raw::RawDigit> const* > const& inputs,
		   std::vector<raw::RawDigit> & output,
		   art::PtrRemapper const & remap) {
  if(inputs.size()!=2){
    std::cout << "ERROR! We have more than two collections of raw digits we are adding!" << std::endl;
  }

  std::vector<raw::RawDigit> const& col1(*inputs[0]);
  std::vector<raw::RawDigit> const& col2(*inputs[1]);

  if(col1.size()!=col2.size()){
    std::cout << "ERROR! We have two collections that are not the same size!"
	      << "\nThis isn't going to end well!" << std::endl;
  }
  
  output.resize(col1.size());
  
  return true;
  
  
  //for(size_t i_rd=0
}

/*
bool MixRawDigits( std::vector< std::vector<short> const* > const& inputs,
		   std::vector<short> & output,
		   art::PtrRemapper const & remap) {

  output.resize(inputs.at(0)->size());
  return true;
}
*/
DEFINE_ART_MODULE(mix::OverlayRawDataMicroBooNE)
