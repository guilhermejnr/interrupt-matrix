
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define LED_PIN_R 13
#define LED_PIN_G 11
#define LED_PIN_B 12
#define BTN_A_PIN 5
#define BTN_B_PIN 6
#define WS2812_PIN 7

#define NUM_PIXELS 25
#define DEBOUNCE_DELAY 200 // ms

// Estado global
static int current_number = 0;
static uint32_t last_btn_a_time = 0;
static uint32_t last_btn_b_time = 0;

// Arrays para definir os números de 0-9 na matriz 5x5
static const bool number_patterns[10][25] = {
    //Número 0
    {
    0, 1, 1, 1, 0,      
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0,   
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0   
    },

    //Número 1
    {0, 1, 1, 1, 0,      
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0,    
    0, 1, 1, 0, 0,  
    0, 0, 1, 0, 0   
    },

    //Número 2
    {0, 1, 1, 1, 0,      
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0,    
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0   
    },

    //Número 3
    {0, 1, 1, 1, 0,      
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,    
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0   
    },

    //Número 4
    {0, 1, 0, 0, 0,      
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,    
    0, 1, 0, 1, 0,     
    0, 1, 0, 1, 0   
    },

    //Número 5
    {0, 1, 1, 1, 0,      
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,   
    0, 1, 0, 0, 0,  
    0, 1, 1, 1, 0   
    },

    //Número 6
    {0, 1, 1, 1, 0,      
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0,    
    0, 1, 0, 0, 0,  
    0, 1, 1, 1, 0   
    },

    //Número 7
    {0, 1, 0, 0, 0,      
    0, 0, 0, 1, 0,   
    0, 1, 0, 0, 0,    
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0  
    },

    //Número 8
    {0, 1, 1, 1, 0,      
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0,    
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0   
    },

    //Número 9
    {0, 1, 1, 1, 0,      
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,    
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0   
    }
};

static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) |
           ((uint32_t)(g) << 16) |
           (uint32_t)(b);
}

void display_number(PIO pio, uint sm) {
    // Limpar todos os LEDs primeiro
    for (int i = 0; i < 25; i++) {
        put_pixel(pio, sm, 0);
    }
    
    // Pequeno delay para garantir que todos os LEDs foram limpos
    sleep_ms(1);
    
    // Mostrar o número atual
    for (int i = 0; i < 25; i++) {
        if (number_patterns[current_number][i]) {
            // Usando azul mais brilhante para melhor visibilidade
            put_pixel(pio, sm, urgb_u32(0, 0, 255));
        } else {
            put_pixel(pio, sm, 0);
        }
    }
}

void gpio_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    
    if (gpio == BTN_A_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        if (current_time - last_btn_a_time >= DEBOUNCE_DELAY) {
            if (current_number < 9) current_number++;
            last_btn_a_time = current_time;
        }
    } else if (gpio == BTN_B_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        if (current_time - last_btn_b_time >= DEBOUNCE_DELAY) {
            if (current_number > 0) current_number--;
            last_btn_b_time = current_time;
        }
    }
}

int main() {
    stdio_init_all();

    // Inicializar LED RGB
    gpio_init(LED_PIN_R);
    gpio_init(LED_PIN_G);
    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);

    // Desligar todos os LEDs RGB no início
    gpio_put(LED_PIN_R, 0);
    gpio_put(LED_PIN_G, 0);
    gpio_put(LED_PIN_B, 0);

    // Inicializar botões com pull-up
    gpio_init(BTN_A_PIN);
    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);
    gpio_pull_up(BTN_B_PIN);

    // Configurar interrupções para os botões
    gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BTN_B_PIN, GPIO_IRQ_EDGE_FALL, true);

    // Inicializar WS2812
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);

    const uint32_t led_interval = 100; // 100ms para 5Hz (5 vezes por segundo)
    uint32_t next_led_time = 0;

    while (1) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        
        // Piscar LED vermelho 5 vezes por segundo
        if (current_time >= next_led_time) {
            gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R));
            next_led_time = current_time + led_interval;
        }

        // Atualizar display de números
        display_number(pio, sm);
        
        // Pequeno delay para estabilidade
        sleep_ms(10);
    }

    return 0;
}