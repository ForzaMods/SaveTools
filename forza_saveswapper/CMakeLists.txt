cmake_minimum_required(VERSION 3.28)
project(forza_saveswapper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/adobe/imgui
        GIT_TAG        origin/master
        GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(ImGui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_spectrum.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
)

target_include_directories(ImGui PUBLIC ${imgui_SOURCE_DIR})

add_executable(forza_saveswapper
        main.cpp
        application/Window.cpp
        application/Window.hpp
        application/Application.cpp
        application/Application.hpp
        swapper/CBinaryStream.hpp
        swapper/CHashBase.hpp
        swapper/CCompileTimeHash.hpp
        swapper/CRuntimeHash.hpp
        swapper/TSizeInHeader.hpp
        swapper/structures/CEntity.cpp
        swapper/structures/CEntity.hpp
        swapper/structures/CBinaryEntity.cpp
        swapper/structures/CBinaryEntity.hpp
        swapper/structures/CProfileEntity.cpp
        swapper/structures/CProfileEntity.hpp
        fileUtilities/FileDialogs.cpp
        fileUtilities/FileDialogs.hpp
        fileUtilities/TFileOpenResult.hpp
        encryption/tfit/CTFIT.cpp
        encryption/tfit/CTFIT.hpp
        encryption/tfit/CMac.cpp
        encryption/tfit/CMac.hpp
        encryption/tfit/CEncryption.cpp
        encryption/tfit/CEncryption.hpp
        encryption/tfit/CDecryption.cpp
        encryption/tfit/CDecryption.hpp
        encryption/streams/CFh3Stream.cpp
        encryption/streams/CFh3Stream.hpp
        encryption/streams/CEncryptionStream.cpp
        encryption/streams/CEncryptionStream.hpp
        encryption/TContext.hpp
        encryption/EGameType.hpp
        encryption/EKeyType.hpp
        encryption/streams/CDecryptionStream.cpp
        encryption/streams/CDecryptionStream.hpp
        encryption/keys/Fh4Keys.cpp
        encryption/keys/Fh4Keys.hpp
        encryption/keys/Fh5Keys.hpp
)

target_link_libraries(forza_saveswapper PRIVATE
        ImGui
        d3d11
        d3dcompiler
        dxguid
        dwmapi
        ${WIN32_LIBRARIES}
)

target_compile_options(ImGui PRIVATE -Wall -Wextra -Wpedantic -Werror)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -fPIC -static -static-libgcc -static-libstdc++ -Wl,-subsystem,windows")

install(TARGETS ImGui forza_saveswapper
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include
)

install(DIRECTORY ${imgui_SOURCE_DIR}
        DESTINATION include
)
