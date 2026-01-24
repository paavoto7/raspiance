
set(SOURCES
    main.cpp
    processor.cpp
    recorder.cpp
)

add_executable(raspiance ${SOURCES})

target_include_directories(raspiance PRIVATE .)

