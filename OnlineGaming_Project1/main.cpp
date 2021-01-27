///@author Francis Carroll
///@date 08/12/20

#define _WINSOCK_DEPRECATED_NO_WARNINGS

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

#include <Game.h>

int main()
{
    srand((unsigned)time(nullptr));

    shared_ptr<Game> m_game = make_shared<Game>();
    m_game->run();

    return 0;
}