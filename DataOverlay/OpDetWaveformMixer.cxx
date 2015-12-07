#ifndef OVERLAY_DATAOVERLAY_OPDETWAVEFORMMIXER_CXX
#define OVERLAY_DATAOVERLAY_OPDETWAVEFORMMIXER_CXX

#include "OpDetWaveformMixer.h"
#include <limits>
#include <iostream>
#include <stdexcept>


void mix::OpDetWaveformMixer::DeclareData(std::vector<raw::OpDetWaveform> const& dataVector,
					  std::vector<raw::OpDetWaveform> & outputVector){

  outputVector.reserve(dataVector.size());

  for(size_t i_od=0; i_od<dataVector.size(); i_od++){
    fChannelIndexMap[dataVector[i_od].ChannelNumber()] = i_od;
    outputVector.emplace_back(dataVector[i_od]);
  }
  
}

void mix::OpDetWaveformMixer::Mix(std::vector<raw::OpDetWaveform> const& mcVector,
				  std::unordered_map<raw::Channel_t,float> const& scale_map,
				  std::vector<raw::OpDetWaveform> & outputVector){


  for( auto const& od : mcVector){

    auto it_ch = fChannelIndexMap.find(od.ChannelNumber());

    //if this channel is not in the data, skip this channel!
    if(it_ch==fChannelIndexMap.end())
      continue;

    size_t i_output = it_ch->second;

    fRDAdderAlg.SetPedestalInputs(0.0,0.0);
    fRDAdderAlg.SetScaleInputs(scale_map.at(od.ChannelNumber()),1.0);
    
    //If the sizes are not the same...
    if(od.size() != outputVector[i_output].size()){
      if(_printWarnings)
	std::cout << "WARNING! Two collections don't have same number of samples:\t"
		  << outputVector[i_output].size() << " " << od.size() << std::endl;
      throw std::runtime_error("BADBADBADBADBAD");
      /*
      //if the samples is larger, we make a new vector of the right size, trimmed down appropriately
      if(rd.Samples() > fOutputWaveforms[i_output].waveform.size()){
	std::vector<short> const& mc_trimmed = std::vector<short>(rd.ADCs().begin(),
								  rd.ADCs().begin()+fOutputWaveforms[i_output].waveform.size());
	fRDAdderAlg.AddRawDigits(mc_trimmed,fOutputWaveforms[i_output].waveform);
      }
      //if the samples is shorter, pad it out with the pedestal
      else if(rd.Samples() < fOutputWaveforms[i_output].waveform.size()){
	std::vector<short> mc_trimmed(fOutputWaveforms[i_output].waveform.size(),rd.GetPedestal());
	mc_trimmed.insert(mc_trimmed.begin(),rd.ADCs().begin(),rd.ADCs().end());
	fRDAdderAlg.AddRawDigits(mc_trimmed,fOutputWaveforms[i_output].waveform);
      }
      */
    }
    //Sizes are the same? Easy!
    //else{
    fRDAdderAlg.AddRawDigits(od,outputVector[i_output]);
    //}
      
  }

}

#endif
