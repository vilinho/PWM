# Controle de Servo Motor e LED com Raspberry Pi Pico

## Descrição
Este projeto demonstra o controle de um servo motor e um LED usando **PWM (Modulação por Largura de Pulso)** em um Raspberry Pi Pico.

## Funcionamento
1. O programa realiza a seguinte sequência inicial de movimentos do servo: <br>
- Mover para 180°
- Mover para 90°
- Mover para 0°
- Aguardar 5 segundos entre cada movimento ⏰

2. Iniciar o loop principal onde:
- O LED fará um efeito fade (acende e apaga suavemente)
- O servo motor se moverá continuamente entre 0° e 180°

--> Vídeo demonstrando o funcionamento do efeito fade na placa física: https://drive.google.com/file/d/1WoNXvtLAKVECvAQ86igmws_YM57H_YGf/view?usp=sharing
