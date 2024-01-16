set(NAME util)

add_library(${NAME} INTERFACE)

target_include_directories(${NAME} INTERFACE
    include/
)

target_sources(${NAME}
PRIVATE
    include/util/random.hpp
)