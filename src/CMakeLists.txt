
set(SOURCES
    main.cpp
    processor.cpp
    recorder.cpp
    communicator.cpp
)

add_executable(raspiance ${SOURCES})

target_link_libraries(raspiance PRIVATE hueplusplusstatic)

target_include_directories(raspiance PRIVATE .)

