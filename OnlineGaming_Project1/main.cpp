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

    cout << "Please enter 'host' to host or 'join' to join" << endl;
    string state;
    getline(cin, state);
    cout << "Please enter the IP you would like to join/host on" << endl;
    string ip;
    getline(cin, ip);

    shared_ptr<Game> m_game;
    if (state == "join")
    {
        m_game = make_shared<Game>(GameState::Join, ip);
    }
    else if (state == "host")
    {
        m_game = make_shared<Game>(GameState::Host, ip);
    }
    else
    {
        return 0;
    }
    m_game->run();
    /*Client client("127.0.0.1", 1111);

    if (!client.connectSocket())
    {
        cout << "Failed to connect to server" << endl;
        system("pause");
        return 1;
    }*/

    system("pause");
    return 0;
}