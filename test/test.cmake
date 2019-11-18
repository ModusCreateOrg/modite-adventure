
# CMake Module path (additional cmake files via find_package)
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)

ADD_COMPILE_DEFINITIONS(__MODUS_TARGET_SDL2_AUDIO__=true)
ADD_COMPILE_DEFINITIONS(__MODUS_TARGET_DESKTOP_CONTROLS__=true)
ADD_COMPILE_DEFINITIONS(__MODUS_TARGET_DESKTOP_DISPLAY__=true)
ADD_COMPILE_DEFINITIONS(LIBXMP_CORE_PLAYER=true)

FILE(GLOB_RECURSE TESTS "${CMAKE_CURRENT_SOURCE_DIR}/test/src/tests/*.cpp")
ADD_EXECUTABLE(test
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src/test.h
        ${TESTS}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CREATIVE_ENGINE_SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Resources.bin
        )

TARGET_INCLUDE_DIRECTORIES( test PUBLIC
        ${CREATIVE_ENGINE_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src
        )
TARGET_LINK_LIBRARIES(test ${CREATIVE_ENGINE_LINK_LIBRARIES})