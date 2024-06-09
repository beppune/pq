cmake_minimum_required(VERSION 3.15)
project(pq C)



add_executable(pq src/main.c)



install(TARGETS pq DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
