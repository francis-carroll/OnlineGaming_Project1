///@author Francis Carroll
///@date 08/12/20

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib")
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib")
#endif

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Game.h"

int main()
{
    srand((unsigned)time(nullptr));
    Game m_game;

    m_game.run();

    return 0;
}