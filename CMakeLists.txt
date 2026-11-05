cmake_minimum_required(VERSION 3.16)
project(NetGuardPacketAnalyzer VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)

add_executable(netguard
    src/dpi_mt.cpp
    src/pcap_reader.cpp
    src/packet_parser.cpp
    src/sni_extractor.cpp
    src/types.cpp
)

target_include_directories(netguard PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(netguard PRIVATE Threads::Threads)

if(MSVC)
    target_compile_options(netguard PRIVATE /W4 /permissive-)
else()
    target_compile_options(netguard PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS netguard RUNTIME DESTINATION bin)

enable_testing()
add_executable(classification_test
    tests/classification_test.cpp
    src/types.cpp
)
target_include_directories(classification_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_test(NAME domain_classification COMMAND classification_test)
