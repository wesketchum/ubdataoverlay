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

#include "DataOverlay/RawDigitAdder_HardSaturate.h"
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
  
  size_t nSecondaries() { return 1; } //We ALWAYS here have just one event we use.

  //void processEventIDs(art::EventIDSequence const& seq); //bookkepping for event IDs

  // Mixing Functions

  // For now, allow exactly one  input. Assume MC inputs have been merged
  // previously and one detsim output created if needed. This could be changed
  // but would require mixing functions for MC here.
  bool MixRawDigits( std::vector< std::vector<raw::RawDigit> const* > const& inputs,
  		     std::vector<raw::RawDigit> & output,
  		     art::PtrRemapper const &);

private:

  // Declare member data here.
  RawDigitAdder_HardSaturate fRDAdderAlg;

  fhicl::ParameterSet  fpset;
  short                fDefaultRawDigitSatPoint;
  std::string          fRawDigitInputModuleLabel;

  float SetMCScale(raw::ChannelID_t const& channel);
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

float mix::OverlayRawDataDetailMicroBooNE::SetMCScale(raw::ChannelID_t const& channel){

  //note: we will put here access to the channel database to determine dead channels
  if(channel>0) return 1.0;
  return 0.0;

}

bool mix::OverlayRawDataDetailMicroBooNE::MixRawDigits( std::vector< std::vector<raw::RawDigit> const* > const& inputs,
							std::vector<raw::RawDigit> & output,
							art::PtrRemapper const & remap) {
  
  //make sure we only have two collections for now
  if(inputs.size()!=2){
    std::cout << "ERROR! We have more than two collections of raw digits we are adding!" << std::endl;
  }
  
  std::vector<raw::RawDigit> const& col1(*inputs[0]);
  std::vector<raw::RawDigit> const& col2(*inputs[1]);
  
  //make sure collections have same size
  if(col1.size()!=col2.size()){
    std::cout << "ERROR! We have two collections that are not the same size!"
	      << "\nThis isn't going to end well!" << std::endl;
  }
  
  //loop over the channels in the first collection
  for(size_t i_ch=0; i_ch<col1.size(); i_ch++){
    
    //make sure we are adding the same channel
    if( col1[i_ch].Channel() != col2[i_ch].Channel() ){
      std::cout << "ERROR! Two collections don't have same channel order." << std::endl;
    }

    //make sure we aren't compressed
    if( (col1[i_ch].Compression()==raw::Compress_t::kNone) ||
	(col2[i_ch].Compression()==raw::Compress_t::kNone) ){
      std::cout << "ERROR! We have a compressed object here." << std::endl;
    }

    //make sure our sample sizes are the same
    if( col1[i_ch].Samples() != col2[i_ch].Samples() ){
      std::cout << "ERROR! Two collections don't have same number of samples." << std::endl;
    }

    //initialize adc vector for output
    std::vector<short> output_vec(col1[i_ch].Samples());

    //Assume first one is data, so set scales to one.
    fRDAdderAlg.SetScaleInputs(1.0,1.0);
    fRDAdderAlg.AddRawDigits(col1[i_ch].ADCs(),output_vec);
    
    //Assume second one is MC, so set get the proper scale.
    fRDAdderAlg.SetScaleInputs(SetMCScale(col2[i_ch].Channel()),1.0);
    fRDAdderAlg.AddRawDigits(col2[i_ch].ADCs(),output_vec);

    //now emplace back onto output collection...
    output.emplace_back(col1[i_ch].Channel(),
			col1[i_ch].Samples(),
			output_vec);

    //set pedestal and rms to be same as data (first collection)
    output.back().SetPedestal(col1[i_ch].GetPedestal(),col1[i_ch].GetSigma());
  }
  
  //check to make sure output size is same as input
  if(output.size()!=col1.size()){
    std::cout << "ERROR! Output collection size not the same as input!" << std::endl;
  }
  
  return true;
}

DEFINE_ART_MODULE(mix::OverlayRawDataMicroBooNE)
