// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "game.hpp"
#include "atlas.hpp"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <filesystem>

using namespace lluna;

using namespace std;

Game* Game::_instance = nullptr;

Game* Game::create()
{
    //ToDo: watch for created instances
    Game::_instance = new Game();
    return Game::_instance;
}

Game* Game::get()
{
    return Game::_instance;
}

void Game::destroy()
{
    delete Game::_instance;
    Game::_instance = nullptr;
}

Game::Game()
{

}

Game::~Game()
{
}

void Game::print(string text,int x,int y)
{
    SDL_Rect pos = {x,y,32,32};

    for (size_t n=0;n<text.size();n++) {
        uint32_t code = text[n];
        int i = code % 16;
        int j = code / 16;
        _font->draw(i,j,pos);
        pos.x+=32;
    }
}

void Game::init(int argc,char* argv[])
{
    int garbage;
    int seed = garbage;
    bool value = false;
    string cmd;

    for (int n=1;n<argc;n++) {

        if (value) {
            if (cmd == "--seed") {
                string data = argv[n];
                seed = 0;
                for (char c:data) {
                    seed = seed + c;
                }
            }

            value = false;
            continue;
        }
        cmd = argv[n];

        if (cmd == "--seed") {
            value = true;
        }

    }

    _seed = seed;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cerr<<"Failed to init SDL"<<endl;
    }

    // yeah, logic is inverted for IMG_Init
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        cerr<<"Failed to init SDL_Image"<<endl;
    }

    _window = SDL_CreateWindow("Lluna",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        0,0, SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_ShowCursor(SDL_DISABLE);

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_BLEND);

    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    // theme
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer);
    ImGui_ImplSDLRenderer2_Init(_renderer);

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    _tiles[0] = new Atlas("tileset.png",64,64);
    _font = new Atlas("font.png",32,32);
    _ui = new Atlas("ui.png",64,64);

    //_tiles[1] = new Atlas("L1.png");

    //_level[0]= new Level("test.csv");
    stringstream ss;
    ss<<std::hex<<_seed<<".csv";

    if (std::filesystem::exists(ss.str())) {
        clog<<"There is a saved file"<<endl;
        _level[0] = new Level(ss.str().c_str());
    }
    else {
        _level[0] = new Level(256,256,seed);
    }

    //_level[1]= new Level("level0_L1.csv");

    camx = 0;
    camy = 0;

    _cam_pos = {0,0};

    _item_selected = 0;

    _items[0].what = lluna::Rock;
    _items[1].what = lluna::Dirt;
    _items[2].what = lluna::Grass;
    _items[3].what = lluna::Log;
    _items[4].what = lluna::Wall;
    _items[5].what = lluna::Watermelon;
    _items[6].what = lluna::Leafs;
    _items[7].what = lluna::Water;
    _items[8].what = lluna::Sand;
}

void Game::loop()
{
    bool quit_request = false;
    bool show_items = false;
    
    auto last_clock = std::chrono::steady_clock::now();

    while (!quit_request) {

        //event handling
        SDL_Event event;

        int mouse_x,mouse_y;
        uint32_t mouse_buttons;

        //ImGui::SetMouseCursor(ImGuiMouseCursor_None);

        ImGui::NewFrame();

        // properties window
        ImGui::Begin("Toolbox");
        stringstream ss;
        ss<<"POSITION:"<<_cam_pos.x<<","<<_cam_pos.y<<" MOUSE:"<<mouse_x<<","<<mouse_y;
        ImGui::Text(ss.str().c_str());

        if(ImGui::Button("Home")) {
            move_camera(0,0);
        }
        ImGui::SameLine();

        if(ImGui::Button("Save")) {
            stringstream ss;
            ss<<std::hex<<_seed<<".csv";
            _level[0]->save(ss.str().c_str());
        }
        ImGui::SameLine();

        if(ImGui::Button("Exit")) {
            quit_request = true;
        }
        //ImGui::SameLine();

        int what = _items[_item_selected].what;
        int tx = what % 16;
        int ty = what / 16;
        float kt = 0.0625f;
        float u = tx * kt;
        float v = ty * kt;
        if (ImGui::ImageButton("Foo",_tiles[0]->texture(),{32,32},{u,v},{u+kt,v+kt})) {
            show_items = true;
        }

        ImGui::End();

        if (show_items) {
            ImGui::Begin("Items");
            for (int j=0;j<16;j++) {
                for (int i=0;i<16;i++) {
                    u = i * kt;
                    v = j * kt;
                    stringstream ss;
                    ss<<"item-"<<i<<"."<<j;
                    if (ImGui::ImageButton(ss.str().c_str(),_tiles[0]->texture(),{32,32},{u,v},{u+kt,v+kt})) {
                        _items[_item_selected].what = i+j*16;
                        show_items = false;
                    }
                    ImGui::SameLine();
                }
                ImGui::NewLine();

            }
            ImGui::End();
        }

        while(SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            auto& io = ImGui::GetIO();

            if (io.WantCaptureMouse or io.WantCaptureKeyboard) {
                continue;
            }

            switch (event.type) {
                case SDL_QUIT:
                    clog<<"quit requested"<<endl;
                    quit_request = true;
                break;

                case SDL_KEYDOWN:

                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit_request = true;
                        break;

                        case SDLK_UP:
                            _cam_target.y = _cam_pos.y - 64;
                        break;

                        case SDLK_DOWN:
                            _cam_target.y = _cam_pos.y + 64;
                        break;

                        case SDLK_RIGHT:
                            _cam_target.x = _cam_target.x + 64;
                        break;

                        case SDLK_LEFT:
                            _cam_target.x = _cam_pos.x - 64;
                        break;

                        case SDLK_h:
                            move_camera(0,0);
                        break;

                        case SDLK_p:
                            move_camera(640,640);
                        break;

                        case SDLK_SPACE:
                            _item_selected = _item_selected + 1;
                            _item_selected = _item_selected % 9;
                        break;

                    }
                break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int tx = (_cam_pos.x/64) + (event.button.x/64);
                        int ty = (_cam_pos.y/64) + (event.button.y/64);

                        _level[0]->put(tx,ty,Tiles::Empty);
                    }

                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        int tx = (_cam_pos.x/64) + (event.button.x/64);
                        int ty = (_cam_pos.y/64) + (event.button.y/64);

                        _level[0]->put(tx,ty,_items[_item_selected].what);
                    }

                break;

                case SDL_JOYDEVICEADDED:
                    SDL_JoystickOpen(event.jdevice.which);
                break;

                case SDL_JOYAXISMOTION:
                    clog<<(int)event.jaxis.value<<endl;
                    if (event.jaxis.axis==0) {
                        if (event.jaxis.value>1024) {
                            _cam_target.x = _cam_target.x + 64;
                        }
                        if (event.jaxis.value<-1024) {
                            _cam_target.x = _cam_target.x - 64;
                        }
                    }
                    if (event.jaxis.axis==1) {
                        if (event.jaxis.value>1024) {
                            _cam_target.y = _cam_target.y + 64;
                        }
                        if (event.jaxis.value<-1024) {
                            _cam_target.y = _cam_target.y - 64;
                        }
                    }

                break;

                case SDL_JOYBUTTONDOWN:
                    clog<<(int)event.jbutton.button<<endl;
                break;
            }
        }
        
        auto current_clock = std::chrono::steady_clock::now();

        int ms_delta = std::chrono::duration_cast<std::chrono::milliseconds>(current_clock - last_clock).count();
        
        bool tick = false;
        
        if (ms_delta > 256) {
            last_clock = current_clock;
            tick = true;
        }

        mouse_buttons = SDL_GetMouseState(&mouse_x,&mouse_y);

        //updating
        int dx =  _cam_target.x - _cam_pos.x;
        int dy =  _cam_target.y - _cam_pos.y;

        _cam_pos.x+=dx;
        _cam_pos.y+=dy;

        //rendering
        SDL_SetRenderDrawColor(_renderer, 0x00, 0x98, 0xdc, 0xff);
        SDL_RenderClear(_renderer);

        for (int i=0;i<40;i++) {
            for (int j=21;j>=0;j--) {
                int tx = _cam_pos.x/64;
                int ty = _cam_pos.y/64;
                int l0 = _level[0]->get(tx+i,ty+j);
                SDL_Rect dest;
                dest.x=i*64;
                dest.y=j*64;
                dest.w=64;
                dest.h=64;

                if (l0>-1) {
                    int ax = l0 % 16;
                    int ay = l0 / 16;

                    _tiles[0]->draw(ax,ay,dest);
                    uint8_t light = 0;
                    int tyl = ty + j - 1;
                    
                    while(tyl>0 and light<240) {
                        int p = _level[0]->get(tx+i,tyl);
                        
                        if (p>-1) {
                            light = light + 16;
                        } 
                        tyl--;
                    }
                    
                    
                    //SDL_SetRenderDrawColor(_renderer, 0, 0, 0, light);
                    //SDL_RenderFillRect(_renderer, &dest);
                    
                    if (tick) {
                        if (l0==Tiles::Sand or l0 == Tiles::Water) {
                            int lbottom = _level[0]->get(tx+i,ty+j+1);
                            
                            if (lbottom == Tiles::Empty) {
                                _level[0]->put(tx+i,ty+j+1,l0);
                                _level[0]->put(tx+i,ty+j,Tiles::Empty);
                            }
                        }
                    }
                }
            }
        }

        /*
        for (int n=0;n<9;n++) {
            int cx = 256 + (64*n);
            int cy = 580;
            int ax = _items[n].what % 16;
            int ay = _items[n].what / 16;

            _tiles[0]->draw(ax,ay,{cx,cy,64,64});

            if (n == _item_selected) {
                _ui->draw(1,3,{cx,cy,64,64});
            }
            else {
                _ui->draw(0,3,{cx,cy,64,64});
            }
        }
        */
        //print(ss.str(),0,0);

        // send ImGui context to SDL Renderer context
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(),_renderer);

        _ui->draw(0,0,{mouse_x-8,mouse_y-8,64,64});

        SDL_RenderPresent(_renderer);
    }
}

void Game::move_camera(int x,int y)
{
    _cam_target.x = x;
    _cam_target.y = y;
}
