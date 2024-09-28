cmake_minimum_required(VERSION 3.26)
project(bsit_rpc LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

set(MIDL_OUTPUT
        rpc/Interface_c.c
        rpc/Interface_s.c
        rpc/Interface.h
)

set(COMMON_SRC
        common/utils/logging.h
        common/utils/logging.cpp
        common/utils/rpc.c
        common/errors.hpp
)

set(MIDL_FILE ${CMAKE_CURRENT_LIST_DIR}/Interface.idl)
add_custom_command(
        PRE_BUILD
        OUTPUT ${MIDL_OUTPUT}
        COMMAND midl /win32 ${MIDL_FILE}
        WORKING_DIRECTORY rpc
        DEPENDS ${MIDL_FILE}
        MAIN_DEPENDENCY ${MIDL_FILE}
)

set_source_files_properties(
        ${MIDL_OUTPUT} PROPERTIES GENERATED TRUE
)

add_executable(server server/main.cpp rpc/Interface_s.c ${COMMON_SRC}
        server/file_manager/file_manager.hpp
        server/file_manager/file_manager.cpp
        server/rpc/rpc.cpp)
add_executable(client client/main.cpp rpc/Interface_c.c ${COMMON_SRC}
        client/cli/cli.cpp
        client/cli/cli.hpp
        client/file_manager/file_manager.cpp
        client/file_manager/file_manager.hpp)

target_link_libraries(server rpcrt4)
target_link_libraries(client rpcrt4)
