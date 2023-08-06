aux_source_directory(. SRC)
list(APPEND SRC ../common/Calculator.cpp ../common/calculator_types.cpp)

add_executable(rpc_client ${SRC})

target_link_libraries(rpc_client thrift)

install(TARGETS rpc_client DESTINATION app)