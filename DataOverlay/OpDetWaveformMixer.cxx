#ifndef OVERLAY_DATAOVERLAY_OPDETWAVEFORMMIXER_CXX
#define OVERLAY_DATAOVERLAY_OPDETWAVEFORMMIXER_CXX

#include "OpDetWaveformMixer.h"
#include <limits>
#include <iostream>
#include <stdexcept>


void mix::OpDetWaveformMixer::DeclareData(std::vector<raw::OpDetWaveform> const& dataVector,
					  std::vector<raw::OpDetWaveform> & outputVector){

  outputVector.reserve(dataVector.size());

  for(auto const& od : dataVector){
  
    auto my_channel = fChannelIndexMap.find(od.ChannelNumber());
    if(my_channel != fChannelIndexMap.end() && outputVector[my_channel->second].size() > od.size())
      continue;

    fChannelIndexMap[od.ChannelNumber()] = outputVector.size();
    outputVector.emplace_back(od);

    std::cout << "\tAdded channel " << outputVector.back().ChannelNumber()
	      << " with size " << outputVector.back().size() << " to the output." << std::endl;
  }
  
}

void mix::OpDetWaveformMixer::Mix(std::vector<raw::OpDetWaveform> const& mcVector,
				  std::unordered_map<raw::Channel_t,float> const& scale_map,
				  std::vector<raw::OpDetWaveform> & outputVector){


  for( auto const& od : mcVector){

    std::cout << "\tSize of waveform in channel " << od.ChannelNumber() << " at time " << od.TimeStamp() << " is " << od.size() << std::endl;
    
    auto it_ch = fChannelIndexMap.find(od.ChannelNumber());

    //if this channel is not in the data, skip this channel!
    if(it_ch==fChannelIndexMap.end())
      continue;

    size_t i_output = it_ch->second;

    fRDAdderAlg.SetPedestalInputs(0.0,0.0);
    fRDAdderAlg.SetScaleInputs(scale_map.at(od.ChannelNumber()),1.0);
    
    //If the sizes are not the same...
    if(od.size() != outputVector[i_output].size()){
      if(_printWarnings && od.size()>100)
	std::cout << "WARNING! Two collections don't have same number of samples:\t"
		  << outputVector[i_output].size() << " " << od.size() << std::endl;
      //throw std::runtime_error("BADBADBADBADBAD");
      continue;
    }

    std::cout << "\t\tWaveform size for channel " << od.ChannelNumber() << " is " << od.size() << std::endl;
    
    fRDAdderAlg.AddRawDigits(od,outputVector[i_output]);
      
  }

}

#endif
