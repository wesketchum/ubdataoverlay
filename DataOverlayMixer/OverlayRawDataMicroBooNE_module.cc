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

#include "SimulationBase/MCTruth.h"
#include "SimulationBase/MCParticle.h"

#include "Simulation/SimChannel.h"
#include "Simulation/SimPhotons.h"
#include "Simulation/AuxDetSimChannel.h"

#include "MCBase/MCTrack.h"
#include "MCBase/MCShower.h"

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

  //a lot of MC collections are just simple copies of the collections...
  template<typename T>
  bool MixSimpleCopy( std::vector< std::vector<T> const*> const& inputs,
		      std::vector< T > & output,
		      art::PtrRemapper const &);
  
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
  bool                 fInputFileIsData;
  std::string          fRawDigitInputModuleLabel;
  std::string          fG4InputModuleLabel;
  std::string          fGeneratorInputModuleLabel;
  bool                 fDoMCReco;
  std::string          fMCRecoInputModuleLabel;
  size_t               fEventsToMix;
  float                fDefaultMCScale;
  
  art::Handle< std::vector<raw::RawDigit> > inputDigitHandle;

  void GenerateMCScaleMap(std::vector<raw::RawDigit> const&);
  std::unordered_map<raw::ChannelID_t,float> fMCScaleMap;
};


mix::OverlayRawDataDetailMicroBooNE::OverlayRawDataDetailMicroBooNE(fhicl::ParameterSet const& p,
								    art::MixHelper &helper)
  :
  fRDMixer(false), //print warnings turned off
  fpset(p.get<fhicl::ParameterSet>("detail")),
  fDefaultRawDigitSatPoint(fpset.get<short>("DefaultRawDigitSaturationPoint",4096)),
  fInputFileIsData(fpset.get<bool>("InputFileIsData")),
  fRawDigitInputModuleLabel(fpset.get<std::string>("RawDigitInputModuleLabel")),
  fEventsToMix(fpset.get<size_t>("EventsToMix",1)),
  fDefaultMCScale(fpset.get<float>("DefaultMCScale",1))
{
  
  if(fEventsToMix!=1){
    std::stringstream err_str;
    err_str << "ERROR! Really sorry, but we can only do mixing for one collection right now! ";
    err_str << "\nYep. We're gonna throw an exception now. You should change your fcl to set 'EventsToMix' to 1";
    throw std::runtime_error(err_str.str());
  }

  if(fInputFileIsData){
    fDoMCReco = fpset.get_if_present<std::string>("MCRecoInputModuleLabel",fMCRecoInputModuleLabel);
    fG4InputModuleLabel = fpset.get<std::string>("G4InputModuleLabel");
    fGeneratorInputModuleLabel = fpset.get<std::string>("GeneratorInputModuleLabel");

    //If it produces something on its own, declare it here
    //helper.produces<>();
    
    //MC generator info is a simple copy
    helper.declareMixOp( art::InputTag(fGeneratorInputModuleLabel),
			 &OverlayRawDataDetailMicroBooNE::MixSimpleCopy<simb::MCTruth>,
			 *this );
    
    //Simple copies of G4 SimPhotons, MCParticles, SimChannels, and SimAuxDetChannel
    helper.declareMixOp( art::InputTag(fG4InputModuleLabel),
			 &OverlayRawDataDetailMicroBooNE::MixSimpleCopy<simb::MCParticle>,
			 *this );
    helper.declareMixOp( art::InputTag(fG4InputModuleLabel),
			 &OverlayRawDataDetailMicroBooNE::MixSimpleCopy<sim::SimPhotons>,
			 *this );
    helper.declareMixOp( art::InputTag(fG4InputModuleLabel),
			 &OverlayRawDataDetailMicroBooNE::MixSimpleCopy<sim::SimChannel>,
			 *this );
    helper.declareMixOp( art::InputTag(fG4InputModuleLabel),
			 &OverlayRawDataDetailMicroBooNE::MixSimpleCopy<sim::AuxDetSimChannel>,
			 *this );
    /*
    //Associations of MCParticles to MCTruth...hopefully a simple copy is enough
    helper.declareMixOp( art::InputTag(fG4InputModuleLabel),
    &OverlayRawDataDetailMicroBooNE::MixSimpleCopy
		       < art::Assns<simb::MCTruth,simb::MCParticle,void> >,
		       *this );
		       */
    //Copies of MCShower and MCTrack
    if(fDoMCReco){
      helper.declareMixOp( art::InputTag(fMCRecoInputModuleLabel),
			 &OverlayRawDataDetailMicroBooNE::MixSimpleCopy<sim::MCShower>,
			   *this );
      helper.declareMixOp( art::InputTag(fMCRecoInputModuleLabel),
			   &OverlayRawDataDetailMicroBooNE::MixSimpleCopy<sim::MCTrack>,
			   *this );
    }
  }
  
  helper.declareMixOp( art::InputTag(fRawDigitInputModuleLabel),
  		       &OverlayRawDataDetailMicroBooNE::MixRawDigits,
  		       *this );
}

//Initialize for each event
void mix::OverlayRawDataDetailMicroBooNE::startEvent(const art::Event& event) {

  event.getByLabel(fRawDigitInputModuleLabel,inputDigitHandle);
  if(!inputDigitHandle.isValid()) throw std::exception();
  
  fRDMixer.SetSaturationPoint(fDefaultRawDigitSatPoint);
}

//End each event
void mix::OverlayRawDataDetailMicroBooNE::finalizeEvent(art::Event& event) {
  //Nothing to be done?
}

void mix::OverlayRawDataDetailMicroBooNE::GenerateMCScaleMap(std::vector<raw::RawDigit> const& dataDigitVector){
  //right now, assume the number of channels is the number in the collection
  //and, loop through the channels one by one to get the right channel number
  //note: we will put here access to the channel database to determine dead channels
  for(auto const& d : dataDigitVector)
    fMCScaleMap[d.Channel()] = fDefaultMCScale;
}

template<typename T>
bool mix::OverlayRawDataDetailMicroBooNE::MixSimpleCopy( std::vector< std::vector<T> const*> const& inputs,
							 std::vector< T > & output,
							 art::PtrRemapper const &){
  art::flattenCollections(inputs,output);
  return true;
}

bool mix::OverlayRawDataDetailMicroBooNE::MixRawDigits( std::vector< std::vector<raw::RawDigit> const* > const& inputs,
							std::vector<raw::RawDigit> & output,
							art::PtrRemapper const & remap) {
  
  //make sure we only have two collections for now
  if(inputs.size()!=fEventsToMix || (inputs.size()!=1 && !fInputFileIsData)){
    std::stringstream err_str;
    err_str << "ERROR! We have the wrong number of collections of raw digits we are adding! " << inputs.size();
    throw std::runtime_error(err_str.str());
  }


  if(fInputFileIsData){
    GenerateMCScaleMap(*inputDigitHandle);  
    fRDMixer.DeclareData(*inputDigitHandle);
    for(auto const& icol : inputs)
      fRDMixer.Mix(*icol,fMCScaleMap);
  }
  else if(!fInputFileIsData){
    GenerateMCScaleMap(*(inputs[0]));  
    fRDMixer.DeclareData(*(inputs[0]));
    fRDMixer.Mix(*inputDigitHandle,fMCScaleMap);
  }
  
  fRDMixer.FillRawDigitOutput(output);
  
  return true;
}

DEFINE_ART_MODULE(mix::OverlayRawDataMicroBooNE)
