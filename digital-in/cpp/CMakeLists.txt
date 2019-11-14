set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -O0 ${MRAA_CFLAGS}")
add_executable (digital-in digital-in.cpp)
add_custom_target (run ./digital-in)
target_link_libraries(digital-in ${MRAA_LDFLAGS})
