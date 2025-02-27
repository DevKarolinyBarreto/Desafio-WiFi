#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>

// Ao invés da gente definir as funções aqui em cima, a gente define elas lá em baixo
// e faz as declarações aqui, assim elas estarão no escopo quando forem chamadas
// no setup
void conectaWiFi();
void verificaSite();
void connect_wifi_public();

const char *ssid = "IoT-Senac";
const char *password = "Senac@2025tr";
String wifiConnected = "";

const char *site[] = {"www.senactaboao.com.br", "www.google.com.br", "www.marcoscosta.eti.br"};

void setup()
{
  Serial.begin(115200);
  conectaWiFi();

  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.disconnect();
    connect_wifi_public();
  }
  Serial.println("====================================");
  verificaSite();
}

void loop()
{
}

void conectaWiFi()
{
  Serial.print("Conectando em ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (tentativas < 10)
  {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("WiFi conectado.");
    Serial.println("Endereço de IP: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.print("\nNão foi possível conectar a rede ");
    Serial.println(ssid);
  }
}

void verificaSite()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi não está conectado. Não será possível realizar o ping.");
    return;
  }

  for (int x = 0; x < sizeof(site) / sizeof(site[0]); x++)
  {
    Serial.println("Pingando o site: ");
    Serial.print(String(site[x]));

    bool success = Ping.ping(site[x], 5);

    if (!success)
    {
      Serial.println("O ping falhou!");
    }
    else
    {
      Serial.println("\nPing no site foi realizado com sucesso!");
    }
    Serial.println("==================================");
  }
}

void connect_wifi_public()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("\nProcurando...");

    int qtdeRedes = WiFi.scanNetworks();
    Serial.println("Verificação concluída.");

    if (qtdeRedes == 0)
    {
      Serial.println("Nenhuma rede encontrada.");
    }
    else
    {
      Serial.print("Total de rede(s) encontrada(s): ");
      Serial.println(qtdeRedes);

      for (int i = 0; i < qtdeRedes; i++)
      {
        Serial.print(i + 1);
        Serial.print(": ");

        Serial.print(WiFi.SSID(i)); // nome da rede encontrada
        Serial.print(" - Sinal (");

        Serial.print(WiFi.RSSI(i)); // intensidade do sinal
        Serial.print(") - Possui senha? ");

        Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Não" : "Sim");
        delay(10);
      }

      Serial.println("Verificando a rede aberta com melhor sinal");
      for (int i = 0; i < qtdeRedes; ++i)
      {
        if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)
        {
          Serial.println("Rede aberta com melhor sinal: ");
          Serial.print("Nome da Rede: ");
          Serial.print(WiFi.SSID(i));
          Serial.print(" - Sinal: ");
          Serial.print(WiFi.RSSI(i));
          Serial.println("------------------------------");

          Serial.println();

          if (wifiConnected == "")
          {
            Serial.print("Conectando a melhor rede aberta encontrada: ");
            Serial.println(WiFi.SSID(i));

            WiFi.begin(WiFi.SSID(i));

            int tentativas = 0;

            while (WiFi.status() != WL_CONNECTED)
            {
              Serial.print(".");
              tentativas++;
              if (tentativas == 100)
                break;
              delay(100);
            }

            Serial.println("");

            if (WiFi.status() == WL_CONNECTED)
            {
              wifiConnected = WiFi.SSID(i);
              Serial.println("WiFi conectado! " + wifiConnected);
            }
            else
            {
              wifiConnected = "";
              Serial.println("Não foi possível conectar.");
            }
            break;
          }
          else
          {
            Serial.println("ESP32 já conectado a rede: " + wifiConnected);
          }
          Serial.println("--------------------------------------");
        }
      }
    }
    delay(5000);
  }
}