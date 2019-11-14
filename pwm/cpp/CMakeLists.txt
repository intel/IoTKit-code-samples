set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -O0 ${MRAA_CFLAGS}")
add_executable (pwm pwm.cpp)
add_custom_target (run ./pwm)
target_link_libraries(pwm ${MRAA_LDFLAGS})
