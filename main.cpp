#define NOMINMAX

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <random>
#include <thread>
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Headers/Exential.h"

using json = nlohmann::json;

struct Login {
    std::string nome;
    std::string senha;
    std::string key;
};

std::wstring RandomID()
{
    const wchar_t chars[] =
        L"abcdefghijklmnopqrstuvwxyz"
        L"0123456789";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 35);

    std::wstring id;

    for (int i = 0; i < 18; ++i)
        id += chars[dist(gen)];

    id += L"-";

    for (int i = 0; i < 4; ++i)
        id += chars[dist(gen)];

    return id;
}

int main()
{
    Login user;
    std::string inputUser;
    std::string inputSenha;

    bool loginSucesso = false;
    int opt;

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo{};
    fontInfo.cbSize = sizeof(fontInfo);

    GetCurrentConsoleFontEx(
        hConsole,
        FALSE,
        &fontInfo
    );

    wcscpy_s(
        fontInfo.FaceName,
        L"Comic Sans MS"
    );

    fontInfo.dwFontSize.X = 40;
    fontInfo.dwFontSize.Y = 100;

    SetCurrentConsoleFontEx(
        hConsole,
        FALSE,
        &fontInfo
    );

    std::thread titleThread([&]()
        {
            while (true)
            {
                std::wstring id = RandomID();
                SetConsoleTitleW(
                    (L"[—] " + id).c_str()
                );
                Sleep(300);
                SetConsoleTitleW(
                    (L"[|] " + id).c_str()
                );
                Sleep(300);

                id = RandomID();

                SetConsoleTitleW(
                    (L"[—] " + id).c_str()
                );

                Sleep(300);
            }
        });

    titleThread.detach();

    while (true)
    {
        SetConsoleTextAttribute(
            hConsole,
            FOREGROUND_RED |
            FOREGROUND_BLUE |
            FOREGROUND_INTENSITY
        );

        system("cls");
        std::wcout << L"\n";
        std::wcout << L"         ========= Tela de login =========\n\n";

        std::wcout << L"                 [1] CreateUser\n";
        std::wcout << L"                 [2] Login\n";
        std::wcout << L"                 [3] Quit\n\n";

        std::wcout << L"        > ";

        SetConsoleTextAttribute(
            hConsole,
            FOREGROUND_RED |
            FOREGROUND_GREEN |
            FOREGROUND_BLUE
        );

        std::cin >> opt;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }

        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        switch (opt)
        {
        case 1:
        {
            std::cout << "Enter Your Username: ";
            std::getline(std::cin, user.nome);

            std::cout << "Agora digite sua senha: ";
            std::getline(std::cin, user.senha);

            std::cout << "Digite Sua Key: ";
            std::getline(std::cin, user.key);

            json dados;
            std::ifstream entrada("dados.json");

            if (entrada.is_open())
            {
                try {
                    entrada >> dados;
                }
                catch (...) {
                    dados = json::object();
                }
                entrada.close();
            }

            if (!dados.contains("usuarios") || !dados["usuarios"].is_array())
            {
                dados["usuarios"] = json::array();
            }

            dados["usuarios"].push_back({
                {"key", user.key},
                {"nome", user.nome},
                {"senha", user.senha}
                });

            std::ofstream saida("dados.json");

            if (!saida.is_open())
            {
                std::cout << "Erro ao abrir dados.json!\n";
                break;
            }

            saida << dados.dump(4);
            saida.close();

            std::cout << "\nUsuario criado com sucesso!\n";
            std::cout << "\nPressione Enter para continuar...";
            std::cin.get();
            break;
        }
        case 2:
        {
            json dados;
            std::ifstream arquivo("dados.json");

            if (!arquivo.is_open())
            {
                std::cout << "Nao foi possivel abrir o arquivo! Crie um usuario primeiro.\n";
                std::cout << "\nPressione Enter para continuar...";
                std::cin.get();
                break;
            }

            try {
                arquivo >> dados;
            }
            catch (...) {
                std::cout << "Erro ao ler o arquivo JSON!\n";
                arquivo.close();
                break;
            }
            arquivo.close();

            std::cout << "Digite seu UserName: ";
            std::getline(std::cin, inputUser);

            std::cout << "Digite sua senha: ";
            std::getline(std::cin, inputSenha);

            if (dados.contains("usuarios") && dados["usuarios"].is_array())
            {
                for (const auto& u : dados["usuarios"])
                {
                    if (u["nome"] == inputUser && u["senha"] == inputSenha)
                    {
                        loginSucesso = true;
                        break;
                    }
                }
            }

            if (loginSucesso)
            {
                std::cout << "Login realizado com sucesso!\n";
            }
            else
            {
                std::cout << "Usuario ou senha incorretos!\n";
                std::cout << "\nPressione Enter para continuar...";
                std::cin.get();
            }

            break;
        }
        case 3:
            std::cout << "Encerrando aplicação..." << std::endl;
            Sleep(100);
            return 0;
        default:
            std::cout << "Opcao invalida!" << std::endl;
            Sleep(500);
            break;
        }

        if (loginSucesso)
        {
            loginSucesso = false;
            while (true) {


                Sleep(300);
                MenuStart();

            }
        }
    }

    return 0;
}
