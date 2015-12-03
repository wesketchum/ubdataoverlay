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

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <exception>
#include <sstream>


#include "DataOverlay/RawDigitMixer.h"
#include "RawData/RawDigit.h"
#include "SimpleTypesAndConstants/RawTypes.h"


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
  
  size_t nSecondaries() { return fEventsToMix; } 

  //void processEventIDs(art::EventIDSequence const& seq); //bookkepping for event IDs

  // Mixing Functions

  // For now, allow exactly one  input. Assume MC inputs have been merged
  // previously and one detsim output created if needed. This could be changed
  // but would require mixing functions for MC here.
  bool MixRawDigits( std::vector< std::vector<raw::RawDigit> const* > const& inputs,
  		     std::vector<raw::RawDigit> & output,
  		     art::PtrRemapper const &);
  /*
  //TODO: OpDetWaveform
  bool MixOpDetWaveform( std::vector< std::vector<raw::OpDetWaveform> const* > const& inputs,
                         std::vector<raw::OpDetWaveform> & output,
			 art::PtrRemapper const &);
  */
  
private:

  // Declare member data here.
  RawDigitMixer              fRDMixer;
  
  fhicl::ParameterSet  fpset;
  short                fDefaultRawDigitSatPoint;
  std::string          fRawDigitInputModuleLabel;
  size_t               fEventsToMix;
  float                fDefaultMCScale;
  
  art::Handle< std::vector<raw::RawDigit> > dataDigitHandle;

  void GenerateMCScaleMap();
  std::unordered_map<raw::ChannelID_t,float> fMCScaleMap;
};


mix::OverlayRawDataDetailMicroBooNE::OverlayRawDataDetailMicroBooNE(fhicl::ParameterSet const& p,
								    art::MixHelper &helper)
  :
  fRDMixer(false),
  fpset(p.get<fhicl::ParameterSet>("detail")),
  fDefaultRawDigitSatPoint(fpset.get<short>("DefaultRawDigitSaturationPoint",4096)),
  fRawDigitInputModuleLabel(fpset.get<std::string>("RawDigitInputModuleLabel")),
  fEventsToMix(fpset.get<size_t>("EventsToMix",1)),
  fDefaultMCScale(fpset.get<float>("DefaultMCScale",1))
{
  //If it produces something on its own, declare it here
  //helper.produces<>();

  helper.declareMixOp( art::InputTag(fRawDigitInputModuleLabel),
  		       &OverlayRawDataDetailMicroBooNE::MixRawDigits,
  		       *this );
}

//Initialize for each event
void mix::OverlayRawDataDetailMicroBooNE::startEvent(const art::Event& event) {

  event.getByLabel(fRawDigitInputModuleLabel,dataDigitHandle);
  if(!dataDigitHandle.isValid()) throw std::exception();
  
  fRDMixer.SetSaturationPoint(fDefaultRawDigitSatPoint);
  GenerateMCScaleMap();
}

//End each event
void mix::OverlayRawDataDetailMicroBooNE::finalizeEvent(art::Event& event) {
  //Nothing to be done?
}

void mix::OverlayRawDataDetailMicroBooNE::GenerateMCScaleMap(){
  //right now, assume the number of channels is the number in the collection
  //and, loop through the channels one by one to get the right channel number
  //note: we will put here access to the channel database to determine dead channels
  for(auto const& d : *dataDigitHandle)
    fMCScaleMap[d.Channel()] = fDefaultMCScale;
}

bool mix::OverlayRawDataDetailMicroBooNE::MixRawDigits( std::vector< std::vector<raw::RawDigit> const* > const& inputs,
							std::vector<raw::RawDigit> & output,
							art::PtrRemapper const & remap) {
  
  //make sure we only have two collections for now
  if(inputs.size()!=fEventsToMix){
    std::stringstream err_str;
    err_str << "ERROR! We have more the wrong number of collections of raw digits we are adding! " << inputs.size();
    throw std::runtime_error(err_str.str());
  }

  fRDMixer.DeclareData(*dataDigitHandle);
  for(auto const& icol : inputs)
    fRDMixer.Mix(*icol,fMCScaleMap);


  fRDMixer.FillRawDigitOutput(output);
  
  
  //check to make sure output size is same as input
  if(output.size()!=dataDigitHandle->size()){
    std::stringstream err_str;
    err_str << "ERROR! Output collection size not the same as input!" << std::endl;
    throw std::runtime_error(err_str.str());
  }
  
  return true;
}

DEFINE_ART_MODULE(mix::OverlayRawDataMicroBooNE)
