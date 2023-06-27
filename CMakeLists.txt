project(tlv_save)
set (CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES
    src/*.h
    src/*.cpp
)

add_executable(Main ${SOURCES})


