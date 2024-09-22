cmake_minimum_required(VERSION 3.26)
project(bsit_rpc LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

set(MIDL_OUTPUT
        ${CMAKE_CURRENT_LIST_DIR}/rpc/Interface_c.c
        ${CMAKE_CURRENT_LIST_DIR}/rpc/Interface_s.c
)

set(MIDL_FILE ${CMAKE_CURRENT_LIST_DIR}/Interface.idl)
add_custom_command(
        PRE_BUILD
        OUTPUT ${MIDL_OUTPUT}
        COMMAND midl /win32 ${MIDL_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/rpc
        DEPENDS ${MIDL_FILE}
        MAIN_DEPENDENCY ${MIDL_FILE}
)

set_directory_properties(${CMAKE_CURRENT_LIST_DIR}/rpc
        GENERATED true
)

add_executable(server server/main.cpp rpc/Interface.h rpc/Interface_s.c)
add_executable(client client/main.cpp rpc/Interface.h rpc/Interface_c.c)

target_link_libraries(server rpcrt4)
target_link_libraries(client rpcrt4)
