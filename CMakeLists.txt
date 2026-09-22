cmake_minimum_required(VERSION 3.24)
set(CMAKE_POLICY_VERSION_MINIMUM 3.10)
project(astranav_2d VERSION 1.0 LANGUAGES CXX)


set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Integração nativa do Raylib 5.0 direto do repositório oficial
include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
)
FetchContent_MakeAvailable(raylib)

# Configuração de Include
include_directories(include)

# Busca automática dos arquivos fonte (.cpp)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Criação do executável
add_executable(${PROJECT_NAME} ${SOURCES})

if (EMSCRIPTEN)
    set(PLATFORM "Web" CACHE STRING "" FORCE)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

    target_link_options(${PROJECT_NAME} PRIVATE
        "-sASYNCIFY" 
        "-sUSE_GLFW=3"
        "--preload-file assets"
        "--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/minshell.html"
    )
endif()

# Link das bibliotecas necessárias (Raylib e dependências do sistema)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
