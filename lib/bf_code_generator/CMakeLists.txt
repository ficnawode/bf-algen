set(NAME bf_code_generator)

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
    include/bf_code_generator/BfCodeGenerator.hpp
    src/BfCodeGenerator.cpp
)