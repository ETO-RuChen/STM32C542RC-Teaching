# file-format: 1.0.0
set(CMSIS_COMPONENTS_LIST ) # Start with empty component list. To be populated below by each config.
add_subdirectory(misc/STMicroelectronics.button_hw-part.2.2.0)
add_subdirectory(misc/STMicroelectronics.crystal_hw-part.2.3.1)
add_subdirectory(misc/STMicroelectronics.esd_hw-part.2.2.0)
add_subdirectory(misc/STMicroelectronics.led_hw-part.2.5.0)
add_subdirectory(misc/STMicroelectronics.mcp2562fd_hw-part.2.1.0)
add_subdirectory(misc/STMicroelectronics.nucleo-c542rc_hw-board.2.1.0)
add_subdirectory(misc/STMicroelectronics.st-link_hw-part.2.1.0)
add_subdirectory(stm32c5xx_dfp)
target_link_libraries(${CMAKE_PROJECT_NAME} STMicroelectronics_button_hw_part_2_2_0 STMicroelectronics_crystal_hw_part_2_3_1 STMicroelectronics_esd_hw_part_2_2_0 STMicroelectronics_led_hw_part_2_5_0 STMicroelectronics_mcp2562fd_hw_part_2_1_0 STMicroelectronics_nucleo_c542rc_hw_board_2_1_0 STMicroelectronics_st_link_hw_part_2_1_0 STMicroelectronics_stm32c5xx_dfp_2_1_0)
