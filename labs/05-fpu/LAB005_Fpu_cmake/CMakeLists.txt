# file-format: 1.0.0
cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

include(cmake/target.cmake)
include(cmake/${TOOLCHAIN_FILE})

project(LAB005_Fpu)
add_executable(LAB005_Fpu)
enable_language(C CXX ASM)
include(cmake/files.cmake)
include(cmake/flags.cmake)
include(cmake/components.cmake)

# Stubs to be able to add compiler flag overrides, e.g. -Os
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  # Add additional source files here
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional include directories here
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional defined symbols here
)

target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional compiler flags here
)

target_link_options(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional linker flags here
)

target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional linker search directories here
)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional linker libraries here
)

# Project-owned application layer. Generated/vendor content stays below the
# generated component boundary and can be regenerated without losing this file.
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  app/app_main.c
  app/platform_uart.c
  stm32c5xx_dfp/Source/Templates/system_stm32c5xx.c
  stm32c5xx_dfp/Source/startup_stm32c542xx.c
)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  app
  arch/cmsis/CMSIS/Core/Include
)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -mfloat-abi=hard -mfpu=fpv5-sp-d16)
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
  -T${CMAKE_CURRENT_SOURCE_DIR}/stm32c5xx_dfp/Source/Templates/gcc/linker/stm32c542xc_flash.ld
)
