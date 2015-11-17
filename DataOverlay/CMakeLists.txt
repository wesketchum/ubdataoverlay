# basic source code CMakeLists.txt

art_make_library( LIBRARY_NAME  ub_DataOverlay
	SOURCE DataOverlay.cxx
	LIBRARIES
                 ${ROOT_BASIC_LIB_LIST}

)

install_headers()
install_source()
