include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/sequence
    ${CMAKE_SOURCE_DIR}/src
)

add_executable(lab3
    main.cpp
    MainWindow.hpp
    MainWindow.cpp
    BaseTab.hpp
    BaseTab.cpp
    HanoyTab.hpp
    HanoyTab.cpp
    AnalysisTab.hpp
    AnalysisTab.cpp
    PrimeTab.hpp
    PrimeTab.cpp
    QueueController.hpp
    QueueControllerImpl.hpp
    QueueControllerImpl.cpp
)

target_link_libraries(lab3 PRIVATE 
    Qt6::Widgets
    ${include}
    )