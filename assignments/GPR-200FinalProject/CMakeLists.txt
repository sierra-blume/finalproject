#Transformations example

file(
 GLOB_RECURSE ASSIGNMENT7_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE ASSIGNMENT7_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this GPR-200FinalProject's asset folder to bin when it is built
add_custom_target(copyAssetsA7 ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${ASSIGNMENT7_INC} DESTINATION include/GPR-200FinalProject)
add_executable(GPR-200FinalProject ${ASSIGNMENT7_SRC} ${ASSIGNMENT7_INC} ${ASSIGNMENT7_ASSETS} "terrainMain.cpp")
target_link_libraries(GPR-200FinalProject PUBLIC core IMGUI)
target_include_directories(GPR-200FinalProject PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when GPR-200FinalProject is built
add_dependencies(GPR-200FinalProject copyAssetsA7)