project('lluna',['cpp'])

sdl=dependency('sdl2')
sdl_image=dependency('SDL2_image')

executable('lluna', ['main.cpp','level.cpp'],
    dependencies:[sdl,sdl_image],
    cpp_args:'-std=c++20'
    )
