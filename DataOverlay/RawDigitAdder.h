/**
 * \file RawDigitAdder.h
 *
 * \ingroup DataOverlay
 * 
 * \brief Defintion for a templated base class to add two vectors together,
 *        and give an "added" waveform.
 *
 * @author wketchum
 */

/** \addtogroup DataOverlay

    @{*/
#ifndef OVERLAY_DATAOVERLAY_RAWDIGITADDER_H
#define OVERLAY_DATAOVERLAY_RAWDIGITADDER_H

#include <vector>
#include <string>

/**
   \class RawDigitAdder
   Add two vectors together. This is a base class
   that just assumes things are linear, and that you add signed integers. Experiments can 
   have a class inherit from this one, and use that.
   
*/

namespace mix {
  class RawDigitAdder;
}

class mix::RawDigitAdder{

public:

  /// Default constructor
  RawDigitAdder(bool t=true):
  _throw(t){};
  
  void AddRawDigits( std::vector<short> const&,
		     std::vector<short> const&,
		     std::vector<short>&);
  void AddRawDigits( std::vector<short> const&,
		     std::vector<short>&);
  void AddRawDigits( std::vector< std::vector<short> > const&,
		     std::vector<short>&);

  virtual std::string Name() { return "RawDigitAdder_Base"; }
  
  /// Default destructor
  virtual ~RawDigitAdder(){};

 protected:
  bool _throw;
  void FixOverflow(short&);
  
 private:

  virtual void AddRawDigit( short const&, short const&, short&);
  void AddRawDigit( short const&, short&);

  void CheckVectorSize(std::vector<short> const&, std::vector<short> const&);
  

};

#endif
/** @} */ // end of doxygen group 

