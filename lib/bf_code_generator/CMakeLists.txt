set(NAME bf_code_generator)

add_library(${NAME} STATIC)

target_link_libraries(${NAME} 
PUBLIC
    bf_interpreter
PRIVATE
    util
)

target_include_directories(${NAME}
PUBLIC
    include/
)

target_sources(${NAME}
PRIVATE
    include/bf_code_generator/GeneticCodeGenerator.hpp
    include/bf_code_generator/Genotype.hpp
    src/GeneticCodeGenerator.cpp
)