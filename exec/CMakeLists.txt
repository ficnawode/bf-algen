set(NAME exec)

add_executable(${NAME})

target_link_libraries(${NAME}
PRIVATE
    bf_interpreter
    bf_code_generator
)

target_sources(${NAME}
PRIVATE
    src/main.cpp
)

target_include_directories(${NAME}
PRIVATE
    include/
)

add_custom_target(${NAME}.run
    COMMAND ${CMAKE_EXECUTE_PROCESS_COMMAND_ECHO}
    ${NAME}
    DEPENDS ${NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    USES_TERMINAL
)