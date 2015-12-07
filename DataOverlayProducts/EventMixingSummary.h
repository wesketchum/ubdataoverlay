/** ****************************************************************************
 * @file EventMixingSummary.h
 * @brief Definition of basic mixing information
 * @author wketchum@fnal.gov
 * 
 * ****************************************************************************/

#ifndef DATAOVERLAYPRODUCTS_EVENTMIXINGSUMMARY_H
#define DATAOVERLAYPRODUCTS_EVENTMIXINGSUMMARY_H

namespace mix {

  //This is just gonna be a stupid copy of the basic
  //info contained in the art event. Nothing special,
  //though more could be added as necessary
  class EventMixingSummary {

  public:
    EventMixingSummary(){}
    
#ifndef __GCCXML__
  public:
      
    EventMixingSummary(uint32_t e,uint32_t s,uint32_t r)
      {
	fEvent  = e;
	fSubrun = s;
	fRun    = r;
      }
      
    uint32_t Event()  const { return fEvent; }
    uint32_t SubRun() const { return fSubrun; }
    uint32_t Run()    const { return fRun; }

    
    
#endif // !__GCCXML__
  private:
    uint32_t fEvent;
    uint32_t fSubrun;
    uint32_t fRun;
  }; // class EventMixingSummary()
  
  
} // namespace mix


#endif // DATAOVERLAYPRODUCTS_EVENTMIXINGSUMMARY_H

////////////////////////////////////////////////////////////////////////
