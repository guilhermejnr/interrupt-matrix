# Projeto: Contador com Matriz de LEDs no RP2040

Este projeto foi desenvolvido como parte do programa **EmbarcaTech** e tem como objetivo consolidar o aprendizado sobre interrupções, debounce de botões e controle de LEDs (incluindo LEDs endereçáveis WS2812) utilizando a placa **BitDogLab** com o microcontrolador **RP2040**.

## 📌 Funcionalidades

- O LED vermelho do LED RGB pisca continuamente 5 vezes por segundo.
- O botão **A** incrementa o número exibido na matriz de LEDs.
- O botão **B** decrementa o número exibido na matriz de LEDs.
- A matriz de LEDs WS2812 exibe números de **0 a 9** em um formato fixo (estilo digital) ou criativo.

## 🔧 Requisitos do Projeto

1. **Uso de Interrupções (IRQ):** As ações dos botões utilizam interrupções para evitar polling.
2. **Debounce via Software:** Implementação para evitar leituras erradas nos botões.
3. **Controle de LEDs:** Manipulação do LED RGB e da matriz WS2812.
4. **Código organizado e comentado** para melhor entendimento.

## 🛠 Componentes Utilizados

- **Placa BitDogLab (RP2040)**
- **Matriz 5x5 de LEDs WS2812** (conectada à GPIO 7)
- **LED RGB** (conectado às GPIOs 11, 12 e 13)
- **Botões:**
  - Botão A (GPIO 5)
  - Botão B (GPIO 6)

## 🚀 Como Compilar e Executar

1. Instale a **Arm GNU Toolchain** e configure o ambiente de desenvolvimento no **Visual Studio Code**.
2. Clone este repositório:
   ```sh
   git clone https://github.com/guilhermejnr/interrupt-matrix.git
   cd seu-repositorio
   ```
3. Compile e carregue o firmware na placa RP2040.

## 🎥 Vídeo de demonstração:

👉 **https://1drv.ms/v/s!AkNGj8iHZYjMgRFhePM33qvK8QRu**

## 📜 Licença

Este projeto é de código aberto e pode ser utilizado para fins educacionais e acadêmicos.

---
Projeto desenvolvido no programa **EmbarcaTech** 🚀


