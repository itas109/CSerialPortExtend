project(CSerialPortHotSwapDemo)

cmake_minimum_required(VERSION 2.8)

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

include_directories(../src)

IF (CMAKE_HOST_WIN32)
	add_executable( ${PROJECT_NAME} main.cpp ../src/CSerialPortHotSwap.cpp)
ENDIF ()