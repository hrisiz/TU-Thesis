add_library(web_plugin_configurator SHARED WebPluginConfigurator.cpp)
target_link_libraries(web_plugin_configurator PUBLIC plugin_configurator logger elapsed_time)
target_include_directories(web_plugin_configurator PUBLIC ../include/)
install(TARGETS web_plugin_configurator DESTINATION ${PROJECT_BINARY_DIR}/output/lib)

install(DIRECTORY UI DESTINATION ${PROJECT_BINARY_DIR}/output/config/)

add_subdirectory(test)