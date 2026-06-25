include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/sequence
    ${CMAKE_SOURCE_DIR}/src
)

add_executable(lab3
    main.cpp
    Tab1.hpp
    Tab2.hpp
    Tab3.hpp
    Tab4.hpp
)

target_link_libraries(lab3 PRIVATE 
    Qt6::Widgets
    ${include}
    )