# basic source code CMakeLists.txt

art_make( BASENAME_ONLY
	  MODULE_LIBRARIES
			ub_RawDigitAdderAna
			ub_RawDigitMixer
			ub_OpDetWaveformMixer
                        RawData
			SimulationBase
			Simulation
			MCBase
			CalibrationDBI_IOVData
			CalibrationDBI_Providers
                        ${ART_FRAMEWORK_CORE}
			${ART_FRAMEWORK_IO_SOURCES}
			${ART_FRAMEWORK_IO_PRODUCTMIX}
			${ART_FRAMEWORK_IO_ROOT}
			${ART_FRAMEWORK_MODULES}
                        ${ART_FRAMEWORK_PRINCIPAL}
                        ${ART_FRAMEWORK_SERVICES_REGISTRY}
                        ${ART_FRAMEWORK_SERVICES_OPTIONAL}
                        ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
                        ${ART_PERSISTENCY_COMMON}
                        ${ART_PERSISTENCY_PROVENANCE}
                        ${ART_UTILITIES}
                        ${BOOST_LIB}
                        ${MF_MESSAGELOGGER}
                        ${MF_UTILITIES}
                        ${FHICLCPP}
                        ${CETLIB}
                        ${ROOT_BASIC_LIB_LIST}
)							   

install_headers()
install_source()
install_fhicl()
