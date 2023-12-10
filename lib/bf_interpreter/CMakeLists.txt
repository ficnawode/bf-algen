set(NAME bf_interpreter)

add_library(${NAME} STATIC)

target_link_libraries(${NAME} 
PUBLIC
    Microsoft.GSL::GSL
)

target_include_directories(${NAME}
PUBLIC
    include/
)

target_sources(${NAME}
PRIVATE
    include/bf/Interpreter.hpp
    src/Interpreter.cpp
)