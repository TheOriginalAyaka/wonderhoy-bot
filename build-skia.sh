#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

FORCE_SYNC=false
if [ "$1" = "--force-sync" ]; then
    FORCE_SYNC=true
fi

echo -e "${GREEN}Building Skia...${NC}"

if [ ! -d "skia" ]; then
    echo -e "${RED}Error: skia directory not found. Please run this script from the project root.${NC}"
    exit 1
fi

cd skia

if [[ "$OSTYPE" == "darwin"* ]]; then
    echo -e "${YELLOW}Detected macOS${NC}"
    if [[ $(uname -m) == "arm64" ]]; then
        INCLUDE_PATHS='"-I/opt/homebrew/include", "-I/opt/homebrew/include/freetype2"'
        LIB_PATHS='"-L/opt/homebrew/lib"'
    else
        INCLUDE_PATHS='"-I/usr/local/include", "-I/usr/local/include/freetype2"'
        LIB_PATHS='"-L/usr/local/lib"'
    fi
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo -e "${YELLOW}Detected Linux${NC}"
    INCLUDE_PATHS='"-I/usr/include", "-I/usr/include/freetype2", "-I/usr/include/libpng16"'
    LIB_PATHS='"-L/usr/lib", "-L/usr/lib/x86_64-linux-gnu"'
else
    echo -e "${YELLOW}Unknown platform ($OSTYPE), using default paths${NC}"
    INCLUDE_PATHS='""'
    LIB_PATHS='""'
fi

echo -e "${YELLOW}Syncing dependencies...${NC}"
if [ "$FORCE_SYNC" = true ] || [ ! -d "third_party/externals" ] || [ -z "$(ls -A third_party/externals 2>/dev/null)" ]; then
    for attempt in 1 2 3; do
        echo -e "${YELLOW}Attempting git-sync-deps (attempt $attempt/3)...${NC}"
        if python3 tools/git-sync-deps; then
            echo -e "${GREEN}Dependencies synced successfully!${NC}"
            break
        else
            if [ $attempt -eq 3 ]; then
                echo -e "${RED}Failed to sync dependencies after 3 attempts${NC}"
                exit 1
            else
                echo -e "${YELLOW}Attempt $attempt failed, retrying in 5 seconds...${NC}"
                sleep 5
            fi
        fi
    done
else
    echo -e "${GREEN}Dependencies already synced, skipping... (use --force-sync to force)${NC}"
fi

echo -e "${YELLOW}Generating build files for $OSTYPE...${NC}"
bin/gn gen out/Static --args="
    is_official_build = true
    is_debug = false
    is_component_build = false

    skia_use_angle = false
    skia_use_dng_sdk = false
    skia_use_egl = false
    skia_use_expat = false
    skia_use_fontconfig = false
    skia_use_freetype = true
    skia_use_harfbuzz = false
    skia_use_icu = false
    skia_use_libheif = false
    skia_use_libjpeg_turbo_decode = false
    skia_use_libjpeg_turbo_encode = false
    skia_use_libwebp_decode = false
    skia_use_libwebp_encode = false
    skia_use_lua = false
    skia_use_metal = false
    skia_use_piex = false
    skia_use_vulkan = false
    skia_use_wuffs = false
    skia_use_xps = false
    skia_use_zlib = false
    skia_use_gl = false

    skia_enable_ganesh = false
    skia_enable_graphite = false
    skia_enable_skottie = false
    skia_enable_skshaper = false
    skia_enable_skparagraph = false
    skia_enable_svg = false
    skia_enable_pdf = false
    skia_enable_tools = false
    skia_enable_skunicode = false
    skia_enable_vello_shaders = false
    skia_enable_fontmgr_empty = true
    skia_enable_fontmgr_android = false
    skia_enable_fontmgr_custom_directory = false
    skia_enable_fontmgr_custom_embedded = false

    extra_cflags = [ $INCLUDE_PATHS ]
    extra_ldflags = [ $LIB_PATHS ]
"

echo -e "${YELLOW}Building Skia static library...${NC}"
ninja -C out/Static

echo -e "${GREEN}Skia build complete!${NC}"
echo -e "Static library location: ${YELLOW}skia/out/Static/libskia.a${NC}"