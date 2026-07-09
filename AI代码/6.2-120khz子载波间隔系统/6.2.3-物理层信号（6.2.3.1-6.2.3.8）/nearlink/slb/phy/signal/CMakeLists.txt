cmake_minimum_required(VERSION 3.14)
project(slb_phy_signal C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(slb_phy_signal STATIC
    slb-phy-types.c
    slb-gold-seq.c
    slb-zc-seq.c
    slb-ref-signal-common.c
    slb-fts.c
    slb-sts.c
    slb-srs.c
    slb-csi-rs.c
    slb-g-dmrs-c.c
    slb-p-dmrs-c.c
    slb-dmrs-d.c
    slb-t-dmrs-c.c
    slb-pas.c
    slb-phy-signal.c
)

target_include_directories(slb_phy_signal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# MSVC 数学库已内置；MinGW/GCC 需链接 libm
if(NOT MSVC)
    target_link_libraries(slb_phy_signal PUBLIC m)
endif()

# ---------------------------------------------------------------------------
# 6.2.3 验证 demo（对照标准公式独立参考实现）
# ---------------------------------------------------------------------------
set(SLB_DEMO_SUPPORT
    demo/demo-test-common.c
    demo/demo-ref-formula.c
)

function(slb_add_demo demoTarget demoSource)
    add_executable(${demoTarget} ${demoSource} ${SLB_DEMO_SUPPORT})
    target_include_directories(${demoTarget} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/demo)
    target_link_libraries(${demoTarget} PRIVATE slb_phy_signal)
    if(MSVC)
        target_compile_options(${demoTarget} PRIVATE /utf-8)
    endif()
endfunction()

slb_add_demo(demo_6_2_3_1_sync  demo/demo-6-2-3-1-sync.c)
slb_add_demo(demo_6_2_3_2_3_ref demo/demo-6-2-3-2-3-ref.c)
slb_add_demo(demo_6_2_3_4_7_dmrs demo/demo-6-2-3-4-7-dmrs.c)
slb_add_demo(demo_6_2_3_8_pas    demo/demo-6-2-3-8-pas.c)
