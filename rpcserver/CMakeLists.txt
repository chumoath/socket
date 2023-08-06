include_directories(inc)
include_directories(${THRIFT_DIR}/include)

#link_directories(/home/wjxh/linux/3rd/tool/openssl/lib)
#link_directories(/home/wjxh/linux/3rd/tool/thrift/lib)

#aux_source_directory(src SRC)

# 真正的实现在 server.cpp 中
#   server 和 client 都要和 生成的 .cpp 文件一起编
#   生成该 .cpp   thrift -r --gen cpp calculator.thrift
#   需要先编写 .thrift

# 编译 thrift
#   缺什么，查什么，在文件上面包含该头文件即可

# hostname 无法解析
#   /etc/hosts
#   添加 127.0.0.1 localhost

#set(CLIENT_SRC src/rpc_client_bak.cpp src/common/Calculator.cpp)
#set(SERVER_SRC src/rpc_server.cpp src/common/Calculator.cpp)
#
#add_executable(client ${CLIENT_SRC})
#add_executable(server ${SERVER_SRC})

# 还是要链接 ssl，否则执行时 还是找不到符号

#target_link_libraries(client ${OPENSSL_DIR}/lib/libssl.so)
#target_link_libraries(client thrift ssl)


#target_link_libraries(server ${OPENSSL_DIR}/lib/libssl.so)
#target_link_libraries(server thrift ssl)
#
#
#install(TARGETS client DESTINATION app)
#install(TARGETS server DESTINATION app)

include_directories(inc)

add_subdirectory(src/client)
add_subdirectory(src/server)