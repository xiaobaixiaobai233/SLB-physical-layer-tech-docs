# SLB 6.2.2 基础帧结构与物理资源
# airMode = slb
#
# 支持两种打开方式：
#   1) 以 slb/ 为 CMake 根目录（Cursor 当前方式）
#   2) 以 nearlink-6.2.2/ 为根目录，通过 add_subdirectory(slb) 引入

cmake_minimum_required(VERSION 3.16)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(nearlink_slb_6222 C)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
endif()

set(SLB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SLB_LIB_SRCS
    src/slb-cp-ofdm-symbol.c
    src/slb-superframe.c
    src/slb-cot-tti.c
    src/slb-carrier.c
    src/slb-antenna-port.c
    src/slb-resource-element.c
    src/slb-resource-grid.c
    src/slb-comm-domain.c
    src/slb-link.c
    src/slb-passthrough-link.c
    src/slb-rx-resource-locate.c
)

add_library(slb_phy STATIC ${SLB_LIB_SRCS})
target_include_directories(slb_phy PUBLIC ${SLB_INCLUDE_DIR})
target_compile_features(slb_phy PUBLIC c_std_11)

add_executable(slb_phy_example example/slb-phy-example.c)
target_link_libraries(slb_phy_example PRIVATE slb_phy)

set(SLB_TEST_SRCS
    test/demo-6221-cp-ofdm-symbol.c
    test/demo-6222-superframe.c
    test/demo-6223-cot-tti.c
    test/demo-6224-carrier.c
    test/demo-6225-resource-grid.c
    test/demo-6228-comm-domain-link.c
    test/demo-rx-resource-locate.c
    test/slb-test-runner.c
)

add_executable(slb_test_runner ${SLB_TEST_SRCS})
target_include_directories(slb_test_runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test)
target_link_libraries(slb_test_runner PRIVATE slb_phy)
target_compile_features(slb_test_runner PUBLIC c_std_11)

enable_testing()
add_test(NAME slb_6222_demos COMMAND slb_test_runner)
