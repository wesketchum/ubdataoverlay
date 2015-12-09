# ubdataoverlay

Functionality for doing overlays with the art mixing modules. Here, we
focus on data overlays, with the hope to build out the following:
--- Add TPC RawDigits together
--- Add PMT OpDetWaveforms together
--- All else in due time...

Current release is v01_00_00, which depends on uboonecode v04_30_02. Contents will be merged into an upcoming release of uboonecode somehow.

The contents are:
 * DataOverlayProducts --- Products that are added to the event to summarize the event mixing.

   -- EventMixingSummary: a simple product containing just the event, subrun, and run number. Could be expanded as necessary

 * DataOverlay --- Algorithms/classes that provide utilities for doing the overlay, without depending on art

   -- RawDigitAdder: a base class for adding two waveforms (vectors of shorts) together, which by default does simple addition

   -- RawDigitAdder_HardSaturate: a specialized class which implements a "saturation" limit on adding waveforms, allows for pedestal subtraction, and allows for setting scale factors to the waveforms on addition.

   -- RawDigitMixer: a class that handles the addition of raw::RawDigit objects, using a RawDigitAdder_HardSaturate instance

   -- OpDetMixer: a class that handles the addition of raw::OpDetWaveform objects (one waveform per channel!), using a RawDigitAdder_HardSaturate instance

 * DataOverlayMixer --- Art modules, and sample fcl files, for running the mixing filter or analyzing results

   -- OverlayRawDataMicroBooNE_Module: a art mixing filter module that is intended to add one MC event onto one data event, or vice versa. Options in the fcl file include:

      *** RawDigitDataModuleLabel, OpDetDataModuleLabel, RawDigitMCModuleLabel, OpDetMCModuleLabel: input module label names

      *** InputFileIsData: true if the input file is a data even (and we are mixing on MC events), false if the other way around

      *** G4InputModuleLabel, GeneratorInputModuleLabel: If input file is data event, module label names for LArG4 and Generator products, so MC information can be copied into the event

      *** MCRecoInputModuleLabel: (Optional) If input file is data event, module label names for MCReco products, so they can be copied into event. If this is not specified, then these products, even if they exist, will not be copied into the event.

      *** EventsToMix: Number of events to mix in. Currently only allows 1.

      *** DefaultMCRawDigitScale: Default scale factor to apply to MC raw digits (for not-bad channels). Defaults to 1.

      *** DefaultMCOpDetScale: Default scale factor to apply to MC OpDetWaveforms. Defaults to 1.

      *** fileNames: list of files to use for mixing


See MicroBooNE DocDB 5119 and talk given by W. Ketchum in https://indico.fnal.gov/conferenceDisplay.py?confId=11066 for some more details/description.
