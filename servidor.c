#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORTA 5000
#define TAM_BUFFER 1024

int main() {

    WSADATA wsaData;

    SOCKET servidor;
    SOCKET cliente;

    struct sockaddr_in endereco_servidor;
    struct sockaddr_in endereco_cliente;

    char buffer[TAM_BUFFER];

    // Inicializar o Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

        printf("Erro ao inicializar o Winsock.\n");
        return 1;
    }

    // Criar o socket TCP
    servidor = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (servidor == INVALID_SOCKET) {

        printf("Erro ao criar o socket.\n");

        WSACleanup();

        return 1;
    }

   // Configurar o endereco do servidor
    memset(&endereco_servidor, 0, sizeof(endereco_servidor));

    endereco_servidor.sin_family = AF_INET;

    endereco_servidor.sin_addr.s_addr = INADDR_ANY;

    endereco_servidor.sin_port = htons(PORTA);

    // Associar o socket a porta e endereco
    if (bind(
        servidor,
        (struct sockaddr*)&endereco_servidor,
        sizeof(endereco_servidor)
    ) == SOCKET_ERROR) {

        printf("Erro no bind.\n");

        closesocket(servidor);
        WSACleanup();

        return 1;
    }

    // Colocar o socket em modo de escuta
    if (listen(servidor, 5) == SOCKET_ERROR) {

        printf("Erro no listen.\n");

        closesocket(servidor);
        WSACleanup();

        return 1;
    }

    printf("Servidor iniciado na porta %d\n", PORTA);

    // Loop principal para aceitar conexoes de clientes
    while (1) {

        int tamanho_cliente = sizeof(endereco_cliente);

        cliente = accept(
            servidor,
            (struct sockaddr*)&endereco_cliente,
            &tamanho_cliente
        );

        if (cliente == INVALID_SOCKET) {

            printf("Erro ao aceitar conexao.\n");
            continue;
        }

        printf(
            "Cliente conectado (%s)\n",
            inet_ntoa(endereco_cliente.sin_addr)
        );

        // Loop principal para receber comandos do cliente
        while (1) {

            memset(buffer, 0, TAM_BUFFER);

            int bytes_recebidos = recv(
                cliente,
                buffer,
                TAM_BUFFER - 1,
                0
            );

            if (bytes_recebidos <= 0) {

                printf("Cliente desconectado.\n");
                break;
            }

            buffer[bytes_recebidos] = '\0';

            // Remover o caractere de nova linha do comando
            buffer[strcspn(buffer, "\r\n")] = '\0';

            printf(
                "Comando recebido: %s\n",
                buffer
            );

            // Processar o comando recebido
            if (strcmp(buffer, "TIME") == 0) {

                time_t agora;
                struct tm tempo_local;

                char resposta[TAM_BUFFER];

                agora = time(NULL);

                localtime_s(
                    &tempo_local,
                    &agora
                );

                strftime(
                    resposta,
                    TAM_BUFFER,
                    "Hora atual: %H:%M:%S",
                    &tempo_local
                );

                send(
                    cliente,
                    resposta,
                    (int)strlen(resposta),
                    0
                );

            }
            else if (strcmp(buffer, "STATUS") == 0) {

                char resposta[] =
                    "Servidor ativo e aguardando conexoes";

                send(
                    cliente,
                    resposta,
                    (int)strlen(resposta),
                    0
                );

            }
            else if (strncmp(buffer, "ECHO ", 5) == 0) {

                char* mensagem = buffer + 5;

                send(
                    cliente,
                    mensagem,
                    (int)strlen(mensagem),
                    0
                );

            }
            else if (strcmp(buffer, "EXIT") == 0) {

                char resposta[] =
                    "Conexao encerrada";

                send(
                    cliente,
                    resposta,
                    (int)strlen(resposta),
                    0
                );

                break;

            }
            else {

                char resposta[] =
                    "Comando desconhecido";

                send(
                    cliente,
                    resposta,
                    (int)strlen(resposta),
                    0
                );
            }
        }

        // Fechar o socket do cliente
        closesocket(cliente);

        printf("Conexao encerrada.\n");
    }

    // Fechar o socket do servidor
    closesocket(servidor);

    WSACleanup();

    return 0;
}