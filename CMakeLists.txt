cmake_minimum_required(VERSION 3.15)
project(pq CXX)



add_executable(pq src/pq.cpp src/main.cpp)



install(TARGETS pq DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
