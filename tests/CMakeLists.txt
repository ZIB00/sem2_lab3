find_package(GTest REQUIRED)

add_executable(tests 
    PriorityQueueTests.cpp
    BinaryHeapTests.cpp
    SeqCollectionTests.cpp
    IGroupIRingTests.cpp
    LinkedListTests.cpp
    StackTests.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}
    include
)

target_link_libraries(tests PRIVATE 
    GTest::gtest
    GTest::gtest_main
    Sequence
)

include(GoogleTest)

gtest_discover_tests(tests)