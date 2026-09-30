#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORTA 5000
#define TAM_BUFFER 1024

int main() {

    WSADATA wsaData;

    SOCKET cliente;

    struct sockaddr_in endereco_servidor;

    char comando[TAM_BUFFER];
    char resposta[TAM_BUFFER];

// Incializar o Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

        printf("Erro ao inicializar o Winsock.\n");

        return 1;
    }

// Criar o socket TCP
    cliente = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (cliente == INVALID_SOCKET) {

        printf("Erro ao criar o socket.\n");

        WSACleanup();

        return 1;
    }

// Configurar o endereco do servidor
    memset(
        &endereco_servidor,
        0,
        sizeof(endereco_servidor)
    );

    endereco_servidor.sin_family = AF_INET;

    endereco_servidor.sin_port = htons(PORTA);

// Definir o endereço IP do servidor (localhost)
    inet_pton(
        AF_INET,
        "127.0.0.1",
        &endereco_servidor.sin_addr
    );

// Conectar ao servidor
    if (connect(
        cliente,
        (struct sockaddr*)&endereco_servidor,
        sizeof(endereco_servidor)
    ) == SOCKET_ERROR) {

        printf("Erro ao conectar ao servidor.\n");

        closesocket(cliente);
        WSACleanup();

        return 1;
    }

    printf("Conectado ao servidor.\n");

// Loop principal para enviar comandos e receber respostas
    while (1) {

        printf("\nDigite um comando: ");

        if (fgets(
            comando,
            TAM_BUFFER,
            stdin
        ) == NULL) {

            break;
        }

// Enviar comando ao servidor
        if (send(
            cliente,
            comando,
            (int)strlen(comando),
            0
        ) == SOCKET_ERROR) {

            printf("Erro ao enviar comando.\n");

            break;
        }

// Receber resposta do servidor
        memset(
            resposta,
            0,
            TAM_BUFFER
        );

        int bytes_recebidos = recv(
            cliente,
            resposta,
            TAM_BUFFER - 1,
            0
        );

        if (bytes_recebidos <= 0) {

            printf("Servidor desconectado.\n");

            break;
        }

        resposta[bytes_recebidos] = '\0';

        printf(
            "Resposta do servidor: %s\n",
            resposta
        );

        // Remover o caractere de nova linha do comando
        comando[strcspn(
            comando,
            "\r\n"
        )] = '\0';

        // Verificar se o comando é "EXIT"
        if (strcmp(
            comando,
            "EXIT"
        ) == 0) {

            break;
        }
    }

    // Fechar o socket do cliente
    closesocket(cliente);

   // Limpar o Winsock
    WSACleanup();

    printf("Cliente encerrado.\n");

    return 0;
}