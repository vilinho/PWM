#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Configurações do Servo
const uint16_t pino_servo = 22;
const float divisor_pwm_servo = 125.0f;    // Divisor de clock para o PWM
const uint16_t periodo_pwm_servo = 20000; // Período de 20ms (50Hz)

// Definições dos pulsos (em µs)
const uint16_t pulso_minimo = 500; // para 0°
const uint16_t pulso_medio = 1470; // para 90°
const uint16_t pulso_maximo = 2400; // para 180°

const uint tempo_espera_servo = 5000; // Tempo de espera entre os movimentos
const uint atraso_loop_servo = 10;  // Atraso entre incrementos

// Configurações do LED RGB
const uint pino_led = 12;
const uint periodo_pwm_led = 255;   
const float divisor_pwm_led = 488.0f; // Aproximadamente 1kHz de frequência para PWM do LED

// Estrutura para armazenar as informações do servo
typedef struct {
    uint16_t pino;
    uint16_t slice_num; 
    uint16_t canal;
    uint16_t pulso_min;
    uint16_t pulso_max;
} Servo;

// Inicializa o servo motor
void inicializar_servo(Servo *servo, uint16_t pino, uint16_t pulso_min, uint16_t pulso_max) {
    servo->pino = pino;
    servo->pulso_min = pulso_min;
    servo->pulso_max = pulso_max;

    gpio_set_function(pino, GPIO_FUNC_PWM);
    servo->slice_num = pwm_gpio_to_slice_num(pino);
    servo->canal = pwm_gpio_to_channel(pino);
    pwm_set_wrap(servo->slice_num, periodo_pwm_servo);
    pwm_set_clkdiv(servo->slice_num, divisor_pwm_servo);
    pwm_set_chan_level(servo->slice_num, servo->canal, 0);
    pwm_set_enabled(servo->slice_num, true);
}

// Define a posição do servo em graus
void definir_posicao_servo(Servo *servo, int angulo) {
    uint16_t pulso;

    // Verifica se o ângulo é um dos valores padrão
    if (angulo == 0) {
        pulso = pulso_minimo;
    } else if (angulo == 90) {
        pulso = pulso_medio;
    } else if (angulo == 180) {
        pulso = pulso_maximo;
    } else {
        // Para outros ângulos, calcula o pulso
        angulo = (angulo < 0) ? 0 : (angulo > 180) ? 180 : angulo;
        pulso = servo->pulso_min + ((servo->pulso_max - servo->pulso_min) * angulo / 180);
    }

    pwm_set_chan_level(servo->slice_num, servo->canal, pulso);
    printf("Posição: %d° ---> Pulso: %d µs\n", angulo, pulso);
}

// Define o pulso do servo em µs
void definir_pulso_servo(Servo *servo, uint pulso) {
    pwm_set_chan_level(servo->slice_num, servo->canal, pulso);
    printf("Pulso alterado: %d µs\n", pulso);
}

int main() {
    stdio_init_all();

    // Inicialização do servo motor
    Servo servo;   
    inicializar_servo(&servo, pino_servo, pulso_minimo, pulso_maximo);

    // Inicialização do LED RGB
    gpio_set_function(pino_led, GPIO_FUNC_PWM);
    uint16_t slice_led = pwm_gpio_to_slice_num(pino_led);
    uint16_t canal_led = pwm_gpio_to_channel(pino_led);
    pwm_set_wrap(slice_led, periodo_pwm_led);
    pwm_set_clkdiv(slice_led, divisor_pwm_led);
    pwm_set_chan_level(slice_led, canal_led, 0); // Inicia com LED apagado
    pwm_set_enabled(slice_led, true);

    // Movimentos iniciais do servo
    definir_posicao_servo(&servo, 180);
    sleep_ms(tempo_espera_servo);
    definir_posicao_servo(&servo, 90);
    sleep_ms(tempo_espera_servo);
    definir_posicao_servo(&servo, 0);
    sleep_ms(tempo_espera_servo);

    int angulo = 500;
    int passo = 5;
    int brilho_led = 0;
    int fator_fade_led = 1; // Fator de incremento/decremento do brilho do LED
    
    
    while (true) {
        // Ajusta o brilho do LED para efeito fade (aumento e redução do brilho)
        
        brilho_led += fator_fade_led;
        // Inverte a direção do fade quando atinge os limites
        if (brilho_led >= periodo_pwm_led) {
            fator_fade_led = -1;
            brilho_led = periodo_pwm_led;
        } else if (brilho_led <= 0) {
            fator_fade_led = 1;
            brilho_led = 0;
        }
        
        // Aplica o novo nível de brilho
        pwm_set_chan_level(slice_led, canal_led, brilho_led);
        
        // Movimenta o servo de 0° a 180°
        definir_pulso_servo(&servo, angulo);
        angulo += passo;
        if (angulo >= 2400 || angulo <= 500)
            passo = -passo;
        
        sleep_ms(atraso_loop_servo);
    }
    return 0;
}
