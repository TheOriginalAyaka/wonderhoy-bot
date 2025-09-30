param(
    [switch]$ForceSync
)

$ErrorActionPreference = "Stop"

Write-Host "Building Skia..." -ForegroundColor Green

if (-not (Test-Path "skia")) {
    Write-Host "Error: skia directory not found. Please run this script from the project root." -ForegroundColor Red
    exit 1
}

Set-Location skia

Write-Host "Detected Windows" -ForegroundColor Yellow
$IncludePaths = '"-IC:/vcpkg/installed/x64-windows/include", "-IC:/vcpkg/installed/x64-windows/include/freetype2"'
$LibPaths = '"-LC:/vcpkg/installed/x64-windows/lib"'

Write-Host "Syncing dependencies..." -ForegroundColor Yellow
if ($ForceSync -or -not (Test-Path "third_party/externals") -or (Get-ChildItem "third_party/externals" -ErrorAction SilentlyContinue | Measure-Object).Count -eq 0) {
    for ($attempt = 1; $attempt -le 3; $attempt++) {
        Write-Host "Attempting git-sync-deps (attempt $attempt/3)..." -ForegroundColor Yellow
        try {
            python tools/git-sync-deps
            Write-Host "Dependencies synced successfully!" -ForegroundColor Green
            break
        }
        catch {
            if ($attempt -eq 3) {
                Write-Host "Failed to sync dependencies after 3 attempts" -ForegroundColor Red
                exit 1
            }
            else {
                Write-Host "Attempt $attempt failed, retrying in 5 seconds..." -ForegroundColor Yellow
                Start-Sleep 5
            }
        }
    }
}
else {
    Write-Host "Dependencies already synced, skipping... (use -ForceSync to force)" -ForegroundColor Green
}

Write-Host "Generating build files for Windows..." -ForegroundColor Yellow
$gnArgs = @"
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

extra_cflags = [ $IncludePaths ]
extra_ldflags = [ $LibPaths ]
"@

bin/gn gen out/Static --args="$gnArgs"

Write-Host "Building Skia static library..." -ForegroundColor Yellow
ninja -C out/Static

Write-Host "Skia build complete!" -ForegroundColor Green
Write-Host "Static library location: skia/out/Static/skia.lib" -ForegroundColor Yellow