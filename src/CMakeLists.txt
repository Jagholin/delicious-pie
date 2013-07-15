
set(target delicious)

set (Sources 
    main.cpp
    pythonhighlighter.h
    pythonhighlighter.cpp
)

add_executable(${target} ${Sources})
target_include_directories(${target} PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(${target} ${PYTHON_LIBRARIES})

qt5_use_modules(${target} Widgets Core Qml Quick)