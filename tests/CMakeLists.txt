include_directories("${PROJECT_SOURCE_DIR}/src")

add_definitions(-DBOOST_TEST_DYN_LINK)

set(SOURCE_FILES_TEST
        test_main.cpp
        )

add_executable(${CMAKE_PROJECT_NAME}_test
        ${SOURCE_FILES_TEST})

target_link_libraries(${CMAKE_PROJECT_NAME}_test
        ${CMAKE_PROJECT_NAME}_lib
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )

add_test (NAME MyTest COMMAND Test)
