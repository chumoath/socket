aux_source_directory(. SRC)
list(APPEND SRC ../common/Calculator.cpp ../common/calculator_types.cpp)

add_executable(rpc_server ${SRC})

#target_link_libraries(server ${OPENSSL_DIR}/lib/libssl.so)
#target_link_libraries(rpc_server thrift ssl)
target_link_libraries(rpc_server thrift)
install(TARGETS rpc_server DESTINATION app)