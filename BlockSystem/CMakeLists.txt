file(GLOB CONFIGURE_DEPENDS SOURCES "*")
target_sources(appPKM_03_qml PRIVATE 
	block.h block.cc
	textBlock.h textBlock.cc
	# ${SOURCES}
)
target_link_libraries(appPKM_03_qml
    PRIVATE Qt6::Quick
)
