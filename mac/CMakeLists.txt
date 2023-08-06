aux_source_directory(src SRC)
add_executable(mac ${SRC})

install(TARGETS mac DESTINATION socket)