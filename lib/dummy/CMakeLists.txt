set(NAME dummy)

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
    include/dummy/dummy.hpp
    src/dummy.cpp
)