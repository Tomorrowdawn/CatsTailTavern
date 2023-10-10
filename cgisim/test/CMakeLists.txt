aux_source_directory(${PROJECT_SOURCE_DIR}/test  _testfile)
add_executable(test  ${_testfile})
target_link_libraries(test cgisim)