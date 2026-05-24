qt_add_executable(lab3
    main_window.cpp
    main.cpp
)

target_link_libraries(lab3 PRIVATE 
    Qt6::Widgets
    sequences
    )