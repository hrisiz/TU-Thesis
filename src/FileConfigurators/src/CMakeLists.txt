add_library(ini_nm_configurator SHARED INIFileConfigurator.cpp FileConfigurator.cpp)
target_link_libraries(ini_nm_configurator PUBLIC ini_reader nm_plugin)
target_include_directories(ini_nm_configurator PUBLIC ../include/)

install(TARGETS ini_nm_configurator DESTINATION ${PROJECT_BINARY_DIR}/output/lib/)

install(FILES configuration.ini DESTINATION ${PROJECT_BINARY_DIR}/output/config/)