include_directories(inc)

aux_source_directory(src SRC)
add_executable(rpc_server ${SRC})

target_link_libraries(rpc_server thrift)
install(TARGETS rpc_server DESTINATION app)