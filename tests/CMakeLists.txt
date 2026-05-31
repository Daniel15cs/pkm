find_package(Qt6 REQUIRED COMPONENTS Test Quick)


function(addQtTest name)
	qt_add_executable(${name} ${name}.cc)
	target_include_directories(core PUBLIC ../src)
	target_link_libraries(${name} PRIVATE core Qt6::Test Qt6::Quick Qt6::Core Qt6::Sql)
	add_test(NAME ${name} COMMAND ${name})
endfunction()

addQtTest(tst_fileModel)
addQtTest(tst_blocks)
addQtTest(tst_pages)
addQtTest(tst_pkm)
# addQtTest()
