# Practica-2-IOT
Protocolos de la capa de aplicación para el IoT y plataformas en la nube

## Estructura

| Ruta | Contenido |
|---|---|
| `platformio.ini` | Proyecto PlatformIO (placa `esp32doit-devkit-v1`), un entorno por firmware |
| `esp32/src/wifi_test/` | Prueba de comunicación ESP32 → servidor TCP por WiFi |
| `esp32/include/secrets.example.h` | Plantilla de credenciales WiFi e IP del servidor |
| `server/basic_server.py` | Servidor TCP básico en Python |
| `docs/` | Enunciado e indicaciones del docente |

## Prueba de comunicación (ESP32 ↔ servidor TCP)

1. Instalar VS Code y la extensión **PlatformIO IDE**; abrir la carpeta raíz del repositorio.
2. Conectar la PC a la misma red WiFi (2.4 GHz) que usará la ESP32 y ejecutar:
   ```
   python server/basic_server.py
   ```
   Anotar la IP que muestra el servidor.
3. Copiar `esp32/include/secrets.example.h` como `esp32/include/secrets.h` y completar
   `WIFI_SSID`, `WIFI_PASSWORD` y `SERVER_IP`. Este archivo no se sube al repositorio.
4. En la barra de PlatformIO: **Upload** (→) y luego **Serial Monitor** (enchufe), entorno `wifi_test`.
5. La ESP32 envía `HELLO <n>` cada segundo y el servidor responde `ACK HELLO <n>`.
