# Comunicação Cliente-Servidor com Sockets

Projeto desenvolvido em **C** para demonstrar a comunicação entre cliente e servidor utilizando **sockets TCP (Winsock)**.

## Estrutura

* **`servidor.c`** — aguarda conexões, recebe comandos, processa as solicitações e envia as respostas.
* **`cliente.c`** — conecta-se ao servidor, envia comandos e exibe as respostas recebidas.

## Protocolo

A comunicação utiliza os seguintes comandos:

* **`TIME`** — retorna a hora atual do servidor.
* **`STATUS`** — informa o estado do servidor.
* **`ECHO <mensagem>`** — retorna a mensagem enviada.
* **`EXIT`** — encerra a conexão.

Comandos não reconhecidos recebem a resposta `Comando desconhecido`.

## Exemplo

```text
Digite um comando: TIME
Resposta do servidor: Hora atual: 19:22:51

Digite um comando: STATUS
Resposta do servidor: Servidor ativo e aguardando conexoes

Digite um comando: ECHO teste
Resposta do servidor: teste

Digite um comando: EXIT
Resposta do servidor: Conexao encerrada
```

## Autor

**Lucas Alberti**
Sistemas de Informação
