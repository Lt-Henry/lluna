project('lluna',['cpp'])

sdl=dependency('sdl2')
sdl_image=dependency('SDL2_image')

imgui_headers = include_directories('imgui')

imgui = library('imgui',['imgui/imgui.cpp','imgui/imgui_draw.cpp','imgui/imgui_widgets.cpp','imgui/imgui_tables.cpp','imgui/backends/imgui_impl_sdlrenderer2.cpp','imgui/backends/imgui_impl_sdl2.cpp'],
include_directories:imgui_headers,
dependencies:[sdl]
)

executable('lluna', ['main.cpp','game.cpp','level.cpp','tile.cpp','atlas.cpp','noise.cpp'],
    include_directories: imgui_headers,
    dependencies:[sdl,sdl_image],
    cpp_args:'-std=c++17',
    link_with:imgui
    )
