file(GLOB CONFIGURE_DEPENDS SOURCES "*")
target_sources(appPKM_03_qml PRIVATE 
	block.h 
	textBlock.h textBlock.cc
	checkboxBlock.h checkboxBlock.cc
	pageBlock.h pageBlock.cc
	toggleBlock.h toggleBlock.cc
	bulletBlock.h bulletBlock.cc
	# ${SOURCES}
)
target_link_libraries(appPKM_03_qml
    PRIVATE Qt6::Quick
)
