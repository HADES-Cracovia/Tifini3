set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB UnitTests_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*_tests.cpp" )

link_directories(${CMAKE_BINARY_DIR}/lib)

add_executable(tester test_runner.cpp ${UnitTests_SRCS})
target_link_libraries(tester ${CPPUNIT_LIBRARIES} ${ROOT_LIBRARIES} TifiniTools)

foreach(test ${UnitTests_SRCS})
	get_filename_component(TestName ${test} NAME_WE)
	add_test(${TestName} tester ${TestName})
endforeach(test)

enable_testing()
add_test(NAME "BasicTestCase" COMMAND tester)

# add_custom_command(TARGET MainTest
# 	POST_BUILD
# 	COMMAND ctest --output-on-failure)
# 
add_custom_target(MainTest ALL tester)
