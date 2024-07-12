project('lluna',['cpp'])

sdl=dependency('sdl2')
sdl_image=dependency('SDL2_image')

executable('lluna', ['main.cpp','game.cpp','level.cpp','tile.cpp','atlas.cpp','noise.cpp'],
    dependencies:[sdl,sdl_image],
    cpp_args:'-std=c++17'
    )
