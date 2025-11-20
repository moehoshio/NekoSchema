vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO moehoshio/NekoSchema
    REF v1.1.2
    SHA512 44c3ddb583a8a2b269907ccc74e80ef2177b3dec62218d3118be8a6b6c0afabab4f981f638d5cc059181d248fe02f6ee3733fc3e94504dde6860e831ae8f8a08
    HEAD_REF main
)

set(VCPKG_BUILD_TYPE release) # header-only port

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DNEKO_SCHEMA_BUILD_TESTS=OFF
        -DNEKO_SCHEMA_AUTO_FETCH_DEPS=OFF
        -DNEKO_SCHEMA_ENABLE_MODULE=OFF
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/NekoSchema PACKAGE_NAME nekoschema)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/lib")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")

file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")


