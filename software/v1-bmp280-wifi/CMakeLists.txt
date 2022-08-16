cmake_minimum_required(VERSION 3.13)
set(PICO_SDK_FETCH_FROM_GIT on)

include(pico_sdk_import.cmake)

project(my_project)
set(PICO_BOARD pico_w)

pico_sdk_init()

add_executable(bmp280_i2c
        bmp280_i2c.c
        )

target_link_libraries(bmp280_i2c pico_stdlib hardware_i2c pico_cyw43_arch_lwip_threadsafe_background)

target_include_directories(bmp280_i2c PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	)

# enable usb output, disable uart output
pico_enable_stdio_usb(bmp280_i2c 1)
pico_enable_stdio_uart(bmp280_i2c 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(bmp280_i2c)

