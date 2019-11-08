#!/usr/bin/env bash

######################### Function definitions ########################

SUDO=${SUDO:-}

function ensure_xcode_installed {
    #Ensure XCode full version is installed and configured,
    #as xcodebuild gets invoked later in the build, and it will fail otherwise
    if ! command -v xcodebuild >/dev/null 2>&1; then
        cat 1>&2 <<EOF
Please install XCode from the App Store.
You will need the full version, not just the command line tools.
If you already have XCode installed, you may need to issue this command
to let the tools find the right developer directory:
    "$SUDO" xcode-select -r
See https://github.com/nodejs/node-gyp/issues/569
EOF
        exit 1
    fi
}

function ensure_homebrew_installed {
    #Ensure homebrew is installed
    if ! command -v brew >/dev/null 2>&1; then
      echo "No homebrew found - installing Homebrew from https://brew.sh"
      /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
    fi
}

function ensure_cmake {
    # Adapted from https://askubuntu.com/questions/355565/how-do-i-install-the-latest-version-of-cmake-from-the-command-line
    if [[ -x /usr/local/bin/cmake ]]; then
        return
    fi

    local version
    local -i build
    local tmpdir
    local cmake
    version=3.12
    build=3
    arch=$(arch)
    uname=$(uname -s)
    tmpdir=$(mktemp -d)
    cmake="cmake-$version.$build-$uname-$arch"
    cd "$tmpdir" || exit 1
    if curl -fsSO "https://cmake.org/files/v$version/$cmake.sh"; then
        # Install binary package if we could retrieve it
        echo "Installing binary: https://cmake.org/files/v$version/$cmake.sh"
        $SUDO mkdir -p /opt/cmake
        $SUDO sh "$cmake.sh" --skip-license --prefix=/opt/cmake || true # exits 141 on success for some reason
        $SUDO rm -f /usr/local/bin/cmake
        $SUDO ln -s "/opt/cmake/bin/cmake" /usr/local/bin/cmake
    else
        # Install from source (on Raspberry Pi with Rasbian 9.6 (stretch) for example.
        cmake="cmake-$version.$build"
        echo "Installing from source: https://cmake.org/files/v$version/$cmake.tar.gz"
        curl -fsSO "https://cmake.org/files/v$version/$cmake.tar.gz"
        tar xfz "$cmake.tar.gz"
        cd "$cmake" || exit 1
        echo "Before configure."
        ./configure
        echo "Before make."
        make
        $SUDO make install
    fi
    cd - || exit 1
    rm -rf "$tmpdir"
}

function ensure_debian_devtools_installed {
    $SUDO apt-get -qq update
    $SUDO apt-get -qq install --no-install-recommends build-essential git jq libsdl2-dev libsdl2-image-dev curl doxygen imagemagick ca-certificates openssl
    # Ubuntu 18.04 has an old cmake (3.9) so install a newer one from binaries from cmake
    ensure_cmake
}

function ensure_arch_devtools_installed {
    $SUDO pacman -Sqyyu --noconfirm base-devel git libglvnd sdl2 sdl2_image curl doxygen imagemagick
    # Use same version of cmake as for ubuntu
    ensure_cmake
}

function ensure_creative_engine {
    if [[ ! -d "$CREATIVE_ENGINE_DIR" ]]; then
        git clone https://github.com/ModusCreateOrg/creative-engine.git "$CREATIVE_ENGINE_DIR"
    fi
}

function ensure_resources {
    if [[ ! -d "$RESOURCES_DIR" ]]; then
        git clone https://github.com/ModusCreateOrg/modite-adventure-resources.git "$RESOURCES_DIR"
    fi
}

function build {
    cd "$BASE_DIR" || exit 1
    if [[ ! -d creative-engine ]]; then
        ln -sf "$CREATIVE_ENGINE_DIR" .
    fi
    if [[ ! -d ./src/resources ]]; then
        ln -sf "$RESOURCES_DIR/resources" ./src
    fi
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR" || exit 1
    cmake ..
    make -j 8
}

function ensure_esp_idf {
    if [[ -n "${IDF_PATH:-}" ]]; then
        echo "XTENSA is already installed, nothing to do."
        return
    fi

    echo "Attempting to install XTENSA on: $OS"

    cd "$BASE_DIR" || exit 1
    OS="$(uname)"
    if [[ "$OS" == "Darwin" ]]; then
        cp sdkconfig.osx sdkconfig
        mkdir esp
        cd esp || exit 1
        git clone --recursive https://github.com/espressif/esp-idf.git
        cd esp-idf || exit 1
        git reset --hard SUPPORTED_ESP_IDF_VERSION
        git submodule update --init --recursive
        export IDF_PATH="$BASE_DIR/esp/esp-idf"
        python -m pip install --user -r "$IDF_PATH/requirements.txt"
    else
        echo "Can't install XTENSA on: $OS"
    fi
}

function build_xtensa {
    if [[ ! "$OS" == "Darwin" ]]; then
        echo "Can't build XTENSA target on: $OS"
        return
    fi
    ensure_esp_idf
    if [[ -z "$IDF_PATH" ]]; then
        Echo "ESP_IDF is not installed!"
        return
    fi

    cd "$BASE_DIR" || exit 1

    if [[ ! -d $CREATIVE_ENGINE_DIR ]]; then
        rm -f creative-engine
        ln -s "$CREATIVE_ENGINE_DIR" .
    fi
    if [[ ! -d $RESOURCES_DIR ]]; then
        rm -f ./src/resources
        ln -s "$CREATIVE_ENGINE_DIR" ./src
    fi
    mkdir -p "$BUILD_DIR"
    make -j 10
}

function clean {
    if [ -d "$CREATIVE_ENGINE_DIR" ];then
        cd "$CREATIVE_ENGINE_DIR" || exit 1
        git clean -fdx
        rm -rf "$BASE_DIR/build"
    fi
}

# TODO: Use otool -L and some foo to find the dependencies
#        The sentinel is "/usr/local/opt"
function patch_mac_build {
    if [[ "$OS" == "Darwin" ]]; then
        export APP_DIR="$BASE_DIR/build/Modite.app"
        export APP_CNT_DIR="$APP_DIR/Contents"
        export APP_RES_DIR="$APP_CNT_DIR/Resources"
        export APP_MACOSX_DIR="$APP_CNT_DIR/MacOS"
        if [[ -d "$APP_DIR" ]]; then
            mkdir -p "$APP_MACOSX_DIR"
            rm -rf "$APP_MACOSX_DIR/libs"
            mkdir -p "$APP_MACOSX_DIR/libs"

            cp /usr/local/opt/sdl2/lib/libSDL2.dylib "$APP_MACOSX_DIR/libs/"
            cp /usr/local/opt/sdl2_image/lib/libSDL2_image.dylib "$APP_MACOSX_DIR/libs/"
            cp /usr/local/opt/libpng/lib/libpng.dylib "$APP_MACOSX_DIR/libs/"
            cp /usr/local/opt/jpeg/lib/libjpeg.dylib "$APP_MACOSX_DIR/libs/"
            cp /usr/local/opt/libtiff/lib/libtiff.dylib "$APP_MACOSX_DIR/libs/"
            cp /usr/local/opt/webp/lib/libwebp.dylib "$APP_MACOSX_DIR/libs/"
            chmod 0755 "$APP_MACOSX_DIR"/libs/*

            # FIX 
            install_name_tool -change \
                "/usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib" \
                 "./libs/libSDL2.dylib" \
                 "$APP_MACOSX_DIR/Modite"
            install_name_tool -change \
                "/usr/local/opt/sdl2_image/lib/libSDL2_image-2.0.0.dylib" \
                "./libs/libSDL2_image.dylib" \
                "$APP_MACOSX_DIR/Modite"

            # FIX SDL2_image
            install_name_tool -change \
                "/usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib" \
                "./libs/libSDL2.dylib" \
                "$APP_MACOSX_DIR/libs/libSDL2_image.dylib"
            
            install_name_tool -change \
                "/usr/local/opt/libpng/lib/libpng16.16.dylib" \
                "./libs/libpng.dylib" \
                "$APP_MACOSX_DIR/libs/libSDL2_image.dylib"
            
            install_name_tool -change \
                "/usr/local/opt/jpeg/lib/libjpeg.9.dylib" \
                "./libs/libjpeg.dylib" \
                "$APP_MACOSX_DIR/libs/libSDL2_image.dylib"
            
            install_name_tool -change \
                "/usr/local/opt/libtiff/lib/libtiff.5.dylib" \
                "./libs/libtiff.dylib" \
                "$APP_MACOSX_DIR/libs/libSDL2_image.dylib"
            
            install_name_tool -change \
                "/usr/local/opt/webp/lib/libwebp.7.dylib" \
                "./libs/libwebp.dylib" \
                "$APP_MACOSX_DIR/libs/libSDL2_image.dylib"

            # FIX TIFF
            install_name_tool -change \
                "/usr/local/opt/jpeg/lib/libjpeg.9.dylib" \
                "./libs/libjpeg.dylib" \
                "$APP_MACOSX_DIR/libs/libtiff.dylib"

            # CREATE WRAPPER
            mv "$APP_MACOSX_DIR/Modite" "$APP_MACOSX_DIR/Modite.bin"

            tee "$APP_MACOSX_DIR/Modite" <<"EOF"
#!/usr/bin/env bash
MY_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}")" && pwd )"
(cd "$MY_DIR" && ./Modite.bin)
EOF
            chmod 0755 "${APP_DIR}/Contents/MacOS/Modite"
         
            # INSTALL APP.PLIST & ETC
            cp "$BASE_DIR/resources/info.plist" "$APP_CNT_DIR"
            mkdir -p "$APP_RES_DIR"
            cp "$BASE_DIR/resources/desktop-icon/Modite.icns" "$APP_RES_DIR"

            codesign --force --sign "Developer ID Application: Modus Create, Inc." "$BASE_DIR/build/Modite.app" \
             || echo "Codesign has keychain dependencies, run this on your workstation!"

        fi
    fi
}

function patch_linux_build {
    echo "No patch needed for Linux."
}

function checkout_creative_engine_branch {
    DEFAULT_BRANCH="master"
    MODITE_BRANCH="develop"
    #MODITE_BRANCH=$(git rev-parse --abbrev-ref HEAD)
    cd "$BASE_DIR" || exit 1
    echo "The current modite branch is: $MODITE_BRANCH"
    if (cd "$CREATIVE_ENGINE_DIR" && git checkout "$MODITE_BRANCH"); then
        echo "Checked out creatine-engine branch: $MODITE_BRANCH"
    elif (cd "$CREATIVE_ENGINE_DIR" && git checkout "$DEFAULT_BRANCH"); then
        echo "Checked out creatine-engine branch: $DEFAULT_BRANCH"
    else
        echo "Faied to checkout a branch for creatine-engine!"
        exit 1
    fi
    cd - || exit 1
}

function checkout_resources_branch {
    DEFAULT_BRANCH="master"
    MODITE_BRANCH="develop"
    #MODITE_BRANCH=$(git rev-parse --abbrev-ref HEAD)
    cd "$BASE_DIR" || exit 1
    echo "The current modite branch is: $MODITE_BRANCH"
    if (cd "$RESOURCES_DIR" && git checkout "$MODITE_BRANCH"); then
        echo "Checked out ma-resources branch: $MODITE_BRANCH"
    elif (cd "$RESOURCES_DIR" && git checkout "$DEFAULT_BRANCH"); then
        echo "Checked out ma-resources branch: $DEFAULT_BRANCH"
    else
        echo "Faied to checkout a branch for ma-resources!"
        exit 1
    fi
    cd - || exit 1
}

function archive_app {
    ARCHIVE_NAME=undefined
    if [[ "$OS" == "Darwin" ]]; then
        echo "Archiving Darwin"
        cd "$BUILD_DIR" || exit 1
        ARCHIVE_NAME="modite.osx-${ARTIFACT_VERSION:-devel}.tgz"
        echo "ARCHIVE_NAME = $ARCHIVE_NAME"
        tar czvfp "./$ARCHIVE_NAME" Modite.app
        cd - || exit 1
    elif [[ "$OS" == "Linux" ]]; then
        echo "Archiving Linux"
        cd "$BUILD_DIR" || exit 1
        ARCHIVE_NAME="modite.linux-${ARTIFACT_VERSION:-devel}.tgz"
        echo "ARCHIVE_NAME = $ARCHIVE_NAME"
        tar czvfp "./$ARCHIVE_NAME" Modite
        cd - || exit 1
    fi
}

function add_release {

    # Create release from tag
    curl POST \
        -H "Authorization: token $GITHUB_TOKEN" \
        --data-binary "{\"tag_name\": \"$GITHUB_RELEASE_NAME\"}" \
        "https://api.github.com/repos/$TRAVIS_REPO_SLUG/releases"
}

function get_release_id {

    # Query for release ID
    JSON="$(curl GET \
        -H "Authorization: token $GITHUB_TOKEN" \
        "https://api.github.com/repos/$TRAVIS_REPO_SLUG/releases/tags/$GITHUB_RELEASE_NAME" \
        )"

    MESSAGE="$(cat <<< "$JSON" | jq .message)"
    if [ -n "$MESSAGE" ]; then
        echo "Not found"
        add_release
        JSON="$(curl GET \
        -H "Authorization: token $GITHUB_TOKEN" \
        "https://api.github.com/repos/$TRAVIS_REPO_SLUG/releases/tags/$GITHUB_RELEASE_NAME" \
        )"
    fi

    RELEASE_ID="$(cat <<< "$JSON" | jq .id)"
}

function upload_artifacts {
    echo "Uploading artifacts."
    GITHUB_TOKEN=${GITHUB_TOKEN:-''}
    if [ -z $GITHUB_TOKEN ]; then
        echo "Didn't find GITHUB_TOKEN."
    else
        if [ "${TRAVIS_TAG:-undefined}" = undefined ]; then
            ARTIFACT_VERSION="nightly-$(date +%y-%m-%d-%H%M%S)"
            GITHUB_RELEASE_NAME="nightly"
        else
            ARTIFACT_VERSION="$TRAVIS_TAG"
            GITHUB_RELEASE_NAME="$TRAVIS_TAG"
        fi

        echo "Artifact version: $ARTIFACT_VERSION"
        echo "GitHub Release Name: $GITHUB_RELEASE_NAME"

        archive_app

        get_release_id

        if [ "$RELEASE_ID" = "Not Found" ]; then
            echo "Not able to get release for: $TRAVIS_TAG"
            exit 6
        else
            echo "Found release: $RELEASE_ID"
        fi

        echo "RELEASE_ID: $RELEASE_ID"

        # Save artifact under the release
        curl -H "Authorization: token $GITHUB_TOKEN" \
             -H "Content-Type: $(file -b --mime-type "$ARCHIVE_NAME")" \
             --data-binary "@$ARCHIVE_NAME" \
             "https://uploads.github.com/repos/$TRAVIS_REPO_SLUG/releases/$RELEASE_ID/assets?name=$ARCHIVE_NAME"
    fi
}

