#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/select.h>
#include <pthread.h>

#define PORT 8001


/*
g_server_fd для слушания входящих соединений на сервере
g_readfds хранения множества файловых дескрипторов, которые нужно мониторить на наличие данных для чтения.
g_mutex мьютекс, используемый для обеспечения безопасности при доступе к общим данным из нескольких потоков
*/
int g_server_fd;
fd_set g_readfds;
pthread_mutex_t g_mutex;

int g_active_connections = 0;  // Счетчик активных соединений
int g_max_active_connections = 1;  // Максимальное количество соединений, которое вы хотите поддерживать


void handle_sighup(int signum) {
    printf("Received SIGHUP signal\n");
    // Обработка сигнала SIGHUP
}


void* handle_connection(void* socket_desc) {
    int client_socket = *(int*)socket_desc;
    char buffer[1024] = {0};
    int valread;

    pthread_mutex_lock(&g_mutex);
    g_active_connections++;

    if (g_max_active_connections < g_active_connections) {
        // Закрываем все сверхлимитные соединения
        close(client_socket);
        printf("Close connection accepted\n");
        g_active_connections--;
        
        pthread_mutex_unlock(&g_mutex);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&g_mutex);

    while (1) 
    {
        fd_set tmpfds;
        FD_ZERO(&tmpfds);
        FD_SET(client_socket, &tmpfds);
        int max_fd = client_socket + 1;

        
        int activity = select(max_fd, &tmpfds, NULL, NULL, NULL);

        if (activity < 0) 
        {
            perror("Select error");
            close(client_socket);
            break;
        }

        if (FD_ISSET(client_socket, &tmpfds)) 
        {
            pthread_mutex_lock(&g_mutex);
            if ((valread = read(client_socket, buffer, 1024)) <= 0) 
            {
                if (valread == 0) 
                {
                    printf("Connection closed by client from socket %d\n", client_socket);
                } 
                else {
                    perror("Read error");
                }
                
                close(client_socket);
                break;
            } 
            else {
                printf("Data received from socket %d: %d bytes\n", client_socket, valread);
            }
            pthread_mutex_unlock(&g_mutex);

        }
    }
    pthread_exit(NULL);
}

int main() {
    /*
    sockaddr_in для хранения информации о сетевом адресе.
    addrlen равна размеру структуры address, для передачи размера структуры
    pthread_t для хранения идентификатора потока
    */
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t thread_id;
    pthread_mutex_init(&g_mutex, NULL);

    // Создаем сокет
    if ((g_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес сервера
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Привязываем сокет к адресу и порту
    if (bind(g_server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Начинаем слушать порт
    if (listen(g_server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);
    signal(SIGHUP, handle_sighup);
    while (1) {
        int new_socket;
        if ((new_socket = accept(g_server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }
        printf("New connection accepted\n");

        pthread_create(&thread_id, NULL, handle_connection, (void*)&new_socket);
    }

    return 0;
}
