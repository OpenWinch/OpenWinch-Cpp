
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

## LIBRARY ###
add_library(${OW_TARGET}_lib STATIC ${OPENWINCH_SRC})

set_target_properties( ${OW_TARGET}_lib PROPERTIES ${OW_PROPERTIES} )
target_link_libraries( ${OW_TARGET}_lib PRIVATE ${_OW_LINK} )
target_link_directories( ${OW_TARGET}_lib PRIVATE ${_OW_LINK_DIR} )
target_link_options( ${OW_TARGET}_lib PRIVATE  ${_OW_LINK_OPT} )
target_include_directories( ${OW_TARGET}_lib PRIVATE ${_OW_INCLUDE} )
target_compile_options( ${OW_TARGET}_lib PRIVATE ${_OW_COMPILE_OPT} )
target_compile_definitions( ${OW_TARGET}_lib PRIVATE ${_OW_COMPILE_DEF} )

install(TARGETS ${OW_TARGET}_lib
        DESTINATION /opt/openwinch/bin
        COMPONENT bin
)

### RUNTIME ###
add_executable(${OW_TARGET} ${OW_MAIN})

set_target_properties( ${OW_TARGET} PROPERTIES ${OW_PROPERTIES} )
target_link_libraries( ${OW_TARGET} PRIVATE ${OW_TARGET}_lib )
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