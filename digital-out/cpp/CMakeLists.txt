set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -O0 ${MRAA_CFLAGS}")
add_executable (digital-out digital-out.cpp)
add_custom_target (run ./digital-out)
target_link_libraries(digital-out ${MRAA_LDFLAGS})
