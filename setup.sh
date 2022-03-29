#/usr/bin/env bash

function extract_dependency() {
    # Arguments:
    #   > URL: str value for website
    #   > OUT: str value for output from tarball
    dep_url="$1"
    dep_dir="$2"
    dep_tar="$dep_dir.tar.gz"
    echo "$dep_dir"
    echo "$dep_tar"

    mkdir -vp "$dep_dir"
    wget --show-progress -O "$dep_tar"  "$dep_url"   
    tar -zxf "$dep_tar" -C "$BUILD_DIR"
    rm "$dep_tar"
    return 0
}

BUILD_DIR="./build"
CURRENT_DIR=$(pwd)

lua_ver="5.3.5"
lua_url="http://www.lua.org/ftp/lua-$lua_ver.tar.gz"
lua_dir="$BUILD_DIR/lua-$lua_ver"
extract_dependency "$lua_url" "$lua_dir"

cd "$lua_dir"
make -j8 linux test
make -j8 local
cd "$CURRENT_DIR"

luarocks_ver="3.8.0"
luarocks_url="https://luarocks.org/releases/luarocks-$luarocks_ver.tar.gz"
luarocks_dir="$BUILD_DIR/luarocks-$luarocks_ver"
extract_dependency "$luarocks_url" "$luarocks_dir"

cd "$luarocks_dir"
./configure --prefix=. --with_lua="$lua_dir"
make -j8
make -j8 install
cd "$CURRENT_DIR"

ln -s ".$luarocks_dir/luarocks" .
ln -s ".$lua_dir/install/bin/lua" .
exit 0
