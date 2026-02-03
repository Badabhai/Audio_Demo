message([STATUS] "[APP]audioDemo cmake start build")
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
enable_language(ASM)
add_executable(audioDemo " ")
set(build_target_list ${build_target_list} "audioDemo" CACHE INTERNAL "build target list")

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path})
file(GLOB OUT_FILES   ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/*MP*.bin)
if(NOT "${OUT_FILES}" STREQUAL "")
file(REMOVE ${OUT_FILES})
endif()
endif()

set_target_properties(audioDemo PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${lib_output_path}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}
                      OUTPUT_NAME audioDemo_${build_bank})

if(compile_lib)
target_link_libraries(audioDemo
                      PUBLIC ${audioLib_link}
                      PUBLIC ${consoleLib_link}
                      PUBLIC ${sysmLib_link}
                      PUBLIC ${remoteLib_link}
					  PUBLIC ${btmLib_link}
					  PUBLIC ${halLib_link}
                      PUBLIC libm.a
                      )
endif()

#remove to sdk/bin
target_link_libraries(audioDemo
                      PUBLIC romLib
                      PUBLIC btmLib
                      PUBLIC audioLib
                      PUBLIC consoleLib
                      PUBLIC sysmLib
                      PUBLIC remoteLib
                      PUBLIC gapLib
                      PUBLIC upperstackLib
					  PUBLIC halLib
                      PUBLIC m
                      PUBLIC ${lib_gap}
                      PUBLIC ${lib_rom}
                      PUBLIC ${lib_upperstack}
                      )

#io srcs and headers
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io_hal/common/hw_tim.c)
if(CONFIG_RTL87X3D)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io_hal/common/hal_i2c.c)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io_hal/common/hal_gpio.c)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io_hal/common/hal_dma_internal.c)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io_hal/common/hal_pinmux.c)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io_hal/rtl87x3d/hal_gpio_int.c)

target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io/rtl87x3d/rtl876x_uart.c)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io/rtl87x3d/rtl876x_gdma.c)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io/rtl87x3d/rtl876x_rcc.c)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io/rtl87x3d/rtl876x_tim.c)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io/rtl87x3d/rtl876x_pinmux.c)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io/rtl87x3d/rtl876x_i2c.c)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io/rtl87x3d/rtl876x_gpio.c)
target_include_directories(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io/rtl87x3d)
target_include_directories(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io_hal/rtl87x3d)
endif()
target_include_directories(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/io_hal/common)

#console srcs and headers
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/module/console/transport/console_uart.c)
target_include_directories(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/module/console/transport)

#preinclude kconfig.h
target_precompile_headers(audioDemo PRIVATE  ${kconfig_header_dir_path}/config.h)
target_precompile_headers(audioDemo PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/sample/audio_demo/app_flags.h)
target_include_directories(audioDemo PRIVATE ${kconfig_header_dir_path})

#include directory for board.h config.h
target_include_directories(audioDemo PUBLIC ../)
target_include_directories(audioDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#buck srcs and headers
if(CONFIG_RTL87X3D)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/module/buck/app_buck_apw7564.c)
target_sources(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/module/buck/app_buck_tps62860.c)
target_include_directories(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/sample/common)
target_include_directories(audioDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/mcu/module/buck)
endif()


if(CONFIG_RTL87X3E)
    target_include_directories(audioDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../inc/rtl87x3e)
elseif(CONFIG_RTL87X3EP)
    target_include_directories(audioDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../inc/rtl87x3ep)
elseif(CONFIG_RTL87X3D)
    target_include_directories(audioDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../inc/rtl87x3d)
endif()

#sdk/inc/app
target_include_directories(audioDemo PRIVATE ${sdk_app_dir_path})
target_compile_definitions(audioDemo PUBLIC RAM_TYPE_DATA_ON=0)

#boot file
target_sources(audioDemo
               PRIVATE ${realtek_start_up_boot_file}
               PRIVATE ${arm_start_up_boot_file})
set_property(SOURCE ${arm_start_up_boot_file} PROPERTY LANGUAGE C)

#generate version.h for app
set(version_path ${CMAKE_CURRENT_SOURCE_DIR}/../version.h)
if(NOT compile_lib)
    generate_version("audio_demo_app" ${version_path})
endif()

#set link script app.ld
if(CONFIG_RTL87X3E)
    generate_ldscript("${CMAKE_CURRENT_SOURCE_DIR}/app_rtl87x3e.ld" audioDemo)
elseif(CONFIG_RTL87X3EP)
    generate_ldscript("${CMAKE_CURRENT_SOURCE_DIR}/app_rtl87x3ep.ld" audioDemo)
elseif(CONFIG_RTL87X3D)
    generate_ldscript("${CMAKE_CURRENT_SOURCE_DIR}/app_rtl87x3d.ld" audioDemo)
endif()

#app link option
set(LINK_FLAGS "-fdata-sections -ffunction-sections -Wl,--gc-sections")
set(EXTRA_LINK_FLAGS "-Wl,-Map=${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/audioDemo_${build_bank}.map,--cref,--no-warn-mismatch -specs=nosys.specs")
#app compile option
target_compile_options(audioDemo PRIVATE
                    ${mcpu_arch}
                    ${mfpu_arch}
                    ${mfloat_arch}
                    ${march_arch}
                    -mthumb -Wall
                    -Wno-parentheses
                    -Wno-unused-variable
                    -fdata-sections
                    -ffunction-sections
                    -std=gnu99)

target_link_options(audioDemo
                    PRIVATE
					${mcpu_arch}
					${mfpu_arch}
					${mfloat_arch}
					${march_arch}
                    -mthumb -Wall
                    -Wl,-Map=${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/audioDemo_${build_bank}.map
                    -fdata-sections
                    -ffunction-sections
                    -Wl,--gc-sections
                    -Wl,--cref
                    -specs=nosys.specs
                    -Wl,-wrap=malloc
                    -Wl,-wrap=calloc
                    -Wl,-wrap=free
                    -Wl,-wrap=memset_s
                    -Wl,-wrap=memcpy_s)

 
# post build
set(gcc_tool_path ${REALTEK_SDK_BASE_PATH}/tool/Gadgets/gcc_tool)
set(debug_table_tool_path ${REALTEK_SDK_BASE_PATH}/tool/Gadgets/build_debug_table_tool)
add_custom_command(TARGET  audioDemo POST_BUILD
            WORKING_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}
            COMMAND ${CMAKE_OBJCOPY} -O ihex   ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/audioDemo_${build_bank}.elf  ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/audioDemo_${build_bank}.hex
            COMMAND ${gcc_tool_path}/Hex2Bin.exe  ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/audioDemo_${build_bank}.hex ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/audioDemo_${build_bank}.bin
            COMMAND ${gcc_tool_path}/prepend_header.exe /app_code ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/audioDemo_${build_bank}.bin   /${encryption_type} ${gcc_tool_path}/../${encryption_key}
            COMMAND ${gcc_tool_path}/prepend_header.exe /app_code ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/audioDemo_${build_bank}.bin   /mp_ini ${CMAKE_CURRENT_SOURCE_DIR}/../mdk/mp.ini
            COMMAND bash ${gcc_tool_path}/md5_generate.sh ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/audioDemo_${build_bank}_MP.bin ${version_path}
            COMMAND ${debug_table_tool_path}/build_debug_table.exe --debug_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path} --su_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../../../build --out_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}
            COMMAND ${CMAKE_OBJDUMP} -D -S ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/audioDemo_${build_bank}.elf > ${CMAKE_CURRENT_SOURCE_DIR}/bin/${bin_output_path}/audioDemo_${build_bank}.disasm
            )
