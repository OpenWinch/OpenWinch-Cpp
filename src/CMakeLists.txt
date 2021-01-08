
### Internal libs ###
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
#find_package(stdc++ REQUIRED)
#find_package(RT)

### LIST files ###
set(OW_MAIN "openwinch.cpp")

file(GLOB_RECURSE OPENWINCH_SRC "*.cpp")
list(REMOVE_ITEM OPENWINCH_SRC ${OW_MAIN})
set(OPENWINCH_SRC ${OPENWINCH_SRC} PARENT_SCOPE)

### LIBRARY ###
add_library(${OW_TARGET}_lib OBJECT ${OPENWINCH_SRC})
add_library(${OW_TARGET}_lib_shared SHARED $<TARGET_OBJECTS:${OW_TARGET}_lib>)
add_library(${OW_TARGET}_lib_static STATIC $<TARGET_OBJECTS:${OW_TARGET}_lib>)

foreach(OW_TARGET_EXT lib;lib_shared;lib_static)
    set_target_properties(      ${OW_TARGET}_${OW_TARGET_EXT} PROPERTIES ${OW_PROPERTIES} )
    target_link_libraries(      ${OW_TARGET}_${OW_TARGET_EXT} PRIVATE ${_OW_LINK} )
    target_link_directories(    ${OW_TARGET}_${OW_TARGET_EXT} PRIVATE ${_OW_LINK_DIR} )
    target_link_options(        ${OW_TARGET}_${OW_TARGET_EXT} PRIVATE ${_OW_LINK_OPT} )
    target_include_directories( ${OW_TARGET}_${OW_TARGET_EXT} PRIVATE ${_OW_INCLUDE} )
    target_compile_options(     ${OW_TARGET}_${OW_TARGET_EXT} PRIVATE ${_OW_COMPILE_OPT} )
    target_compile_definitions( ${OW_TARGET}_${OW_TARGET_EXT} PRIVATE ${_OW_COMPILE_DEF} )

    set_target_properties(${OW_TARGET}_${OW_TARGET_EXT} PROPERTIES OUTPUT_NAME "${OW_TARGET}")
    install(TARGETS ${OW_TARGET}_${OW_TARGET_EXT}
            DESTINATION /opt/openwinch/bin
            COMPONENT bin
    )

endforeach()

### RUNTIME ###
add_executable(${OW_TARGET} ${OW_MAIN})

set_target_properties( ${OW_TARGET} PROPERTIES ${OW_PROPERTIES} )
target_link_libraries( ${OW_TARGET} PRIVATE ${OW_TARGET}_lib_static )
target_link_directories( ${OW_TARGET} PRIVATE ${_OW_LINK_DIR} )
target_link_options( ${OW_TARGET} PRIVATE  ${_OW_LINK_OPT} )
target_include_directories( ${OW_TARGET} PRIVATE ${_OW_INCLUDE} )
target_compile_options( ${OW_TARGET} PRIVATE ${_OW_COMPILE_OPT} )
target_compile_definitions( ${OW_TARGET} PRIVATE ${_OW_COMPILE_DEF} )

file(COPY ../public DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) 

install(TARGETS ${OW_TARGET}
        DESTINATION /opt/openwinch/bin
        COMPONENT bin
)

### SHARE ###

install(DIRECTORY ../public 
        DESTINATION /opt/openwinch/bin
        COMPONENT bin)

install(FILES raspberry/slog.cfg
        DESTINATION /opt/openwinch/bin
        COMPONENT bin)