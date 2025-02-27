# Desafio-WiFi

### Mudanças feitas no código:
Primeiro eu criei uma função connect_wifi_public(), que tem o código que irá escanear e entrar na rede pública com o melhor sinal, fiz com que essa função rodasse um while que só iria parar caso conseguisse conectar com sucesso a uma rede.

Declarei String wifiConnected no topo do arquivo para segurar o nome da rede.

No void setup() coloquei WiFi.mode(WIFI_STA), um if que irá checar se a rede está conectada, caso não esteja, ele irá dar um WiFi.disconnect() e em seguida irá chamar a função connect_wifi_public(), assim conseguindo conectar e fazer o ping de uma rede pública.

