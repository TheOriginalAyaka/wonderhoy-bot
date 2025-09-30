set(SKIA_DIR ${CMAKE_SOURCE_DIR}/skia)

set(SKIA_INCLUDE_DIRS
        ${SKIA_DIR}
        ${SKIA_DIR}/include
        ${SKIA_DIR}/include/ports
        ${SKIA_DIR}/include/core
        ${SKIA_DIR}/include/encode
        ${SKIA_DIR}/include/codec
)

add_library(Skia::Core STATIC IMPORTED)
set_target_properties(Skia::Core PROPERTIES
        IMPORTED_LOCATION ${SKIA_DIR}/out/Static/libskia.a
        INTERFACE_INCLUDE_DIRECTORIES "${SKIA_INCLUDE_DIRS}"
)

find_package(PNG REQUIRED libpng)
find_package(Freetype REQUIRED freetype2)

if(APPLE)
    find_library(COREFOUNDATION CoreFoundation REQUIRED)
    find_library(COREGRAPHICS CoreGraphics REQUIRED)
    find_library(CORETEXT CoreText REQUIRED)
    find_library(COREIMAGE CoreImage REQUIRED)

    set(APPLE_FRAMEWORKS
            ${COREFOUNDATION}
            ${COREGRAPHICS}
            ${CORETEXT}
            ${COREIMAGE}
    )
endif()

add_library(Skia::Skia INTERFACE IMPORTED)

target_link_libraries(Skia::Skia INTERFACE
        Skia::Core
        PNG::PNG
        Freetype::Freetype
        $<$<PLATFORM_ID:Darwin>:${APPLE_FRAMEWORKS}>
)
