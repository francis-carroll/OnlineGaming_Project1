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

#include "Client.h"

int main()
{
    srand((unsigned)time(nullptr));

    Client client("127.0.0.1", 1111);

    if (!client.connectSocket())
    {
        cout << "Failed to connect to server" << endl;
        system("pause");
        return 1;
    }

    /*Game m_game;

    m_game.run();*/

    string buffer;
    while (true)
    {
        getline(cin, buffer);

        if (!client.SendString(Packet::P_ChatMessage, buffer)) break;

        Sleep(10);
    }

    system("pause");
    return 0;
}