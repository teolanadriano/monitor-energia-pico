#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#define WIFI_SSID "REDE WIFI"
#define WIFI_PASS "senha12345"
#define PORT 80
#define MATRIX_SIZE 5

// Estados dos aparelhos
char *ventilador = "desligado";
char *ar_condicionado = "desligado";
char *televisao = "desligado";
char *geladeira = "desligado";
char *computador = "desligado";
char *micro_ondas = "desligado";

// Matriz de LEDs 5x5
uint8_t led_matrix[MATRIX_SIZE][MATRIX_SIZE] = {0};

void atualizar_matriz_leds(char *estado) {
    int cor = (strcmp(estado, "ligado") == 0) ? 1 : 2; // 1 = Verde, 2 = Vermelho
    for (int i = 1; i < 4; i++) {
        for (int j = 1; j < 4; j++) {
            led_matrix[i][j] = cor;
        }
    }
    printf("Matriz LED Atualizada:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", led_matrix[i][j]);
        }
        printf("\n");
    }
}

err_t handle_command(char *command, char *response) {
    if (strcmp(command, "status") == 0) {
        sprintf(response, "Ventilador:%s, Ar-condicionado:%s, Televisao:%s, Geladeira:%s, Computador:%s, Micro-ondas:%s",
                ventilador, ar_condicionado, televisao, geladeira, computador, micro_ondas);
    } else if (strncmp(command, "Ventilador:", 11) == 0) {
        ventilador = (command[11] == 'o' ? "ligado" : "desligado");
        atualizar_matriz_leds(ventilador);
        strcpy(response, "Ventilador atualizado");
    } else if (strncmp(command, "Ar-condicionado:", 16) == 0) {
        ar_condicionado = (command[16] == 'o' ? "ligado" : "desligado");
        atualizar_matriz_leds(ar_condicionado);
        strcpy(response, "Ar-condicionado atualizado");
    } else if (strncmp(command, "Televisao:", 10) == 0) {
        televisao = (command[10] == 'o' ? "ligado" : "desligado");
        atualizar_matriz_leds(televisao);
        strcpy(response, "Televisao atualizada");
    } else if (strncmp(command, "Geladeira:", 10) == 0) {
        geladeira = (command[10] == 'o' ? "ligado" : "desligado");
        atualizar_matriz_leds(geladeira);
        strcpy(response, "Geladeira atualizada");
    } else if (strncmp(command, "Computador:", 11) == 0) {
        computador = (command[11] == 'o' ? "ligado" : "desligado");
        atualizar_matriz_leds(computador);
        strcpy(response, "Computador atualizado");
    } else if (strncmp(command, "Micro-ondas:", 12) == 0) {
        micro_ondas = (command[12] == 'o' ? "ligado" : "desligado");
        atualizar_matriz_leds(micro_ondas);
        strcpy(response, "Micro-ondas atualizado");
    } else {
        strcpy(response, "Comando inválido");
    }
    return ERR_OK;
}

err_t server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        return ERR_OK;
    }
    tcp_recved(tpcb, p->len);
    char response[256];
    handle_command((char *)p->payload, response);
    tcp_write(tpcb, response, strlen(response), TCP_WRITE_FLAG_COPY);
    pbuf_free(p);
    return ERR_OK;
}

err_t server_accept(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, server_recv);
    return ERR_OK;
}

void start_server() {
    struct tcp_pcb *pcb = tcp_new();
    tcp_bind(pcb, IP_ADDR_ANY, PORT);
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, server_accept);
    printf("Servidor iniciado na porta %d\n", PORT);
}

int main() {
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar Wi-Fi\n");
        return -1;
    }
    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi %s...\n", WIFI_SSID);
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Falha ao conectar no Wi-Fi\n");
        return -1;
    }
    printf("Conectado ao Wi-Fi\n");
    start_server();
    while (1) {
        sleep_ms(1000);
    }
}
