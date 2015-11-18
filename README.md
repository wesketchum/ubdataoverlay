# ubdataoverlay

Functionality for doing overlays with the art mixing modules. Here, we
focus on data overlays, with the hope to build out the following:
--- Add TPC RawDigits together
--- Add PMT OpDetWaveforms together
--- All else in due time...

As a start, right now, we just have the RawDigitAdder. It's a base
class that supports adding digit collections together where
(1) the addition of one RawDigit is independent of other RawDigits
(i.e., no correlations across wires), and
(2) the addition of adc values in time on a RawDigit is independent of
other adc values on that RawDigit (i.e., no complicated dependencies
on nearby ticks).
These can be worked around with a little effort.
