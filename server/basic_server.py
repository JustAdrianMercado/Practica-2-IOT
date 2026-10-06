"""Servidor TCP básico para probar la comunicación con un ESP32.

Escucha en todas las interfaces, imprime cada línea recibida de un cliente
y responde con ``ACK <mensaje>``. Atiende varios clientes a la vez (un hilo
por cliente).

Uso:
    python server/basic_server.py [puerto]
"""

import socket
import sys
import threading

HOST = "0.0.0.0"
DEFAULT_PORT = 5000


class BasicTcpServer:
    """Servidor TCP que hace eco de las líneas recibidas con un ACK."""

    def __init__(self, host: str, port: int) -> None:
        self.host = host
        self.port = port

    def serve_forever(self) -> None:
        """Acepta clientes indefinidamente y los atiende en hilos separados."""
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
            server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            server.bind((self.host, self.port))
            server.listen()
            print(f"[SERVER] Escuchando en {self.host}:{self.port}")
            print(f"[SERVER] IP local de esta PC: {self._local_ip()}")
            while True:
                conn, addr = server.accept()
                threading.Thread(
                    target=self._handle_client, args=(conn, addr), daemon=True
                ).start()

    def _handle_client(self, conn: socket.socket, addr: tuple) -> None:
        """Lee líneas del cliente y responde a cada una con ``ACK``."""
        client = f"{addr[0]}:{addr[1]}"
        print(f"[SERVER] Cliente conectado: {client}")
        with conn, conn.makefile("r", encoding="utf-8", newline="\n") as reader:
            for line in reader:
                message = line.strip()
                if not message:
                    continue
                print(f"[{client}] -> {message}")
                conn.sendall(f"ACK {message}\n".encode("utf-8"))
        print(f"[SERVER] Cliente desconectado: {client}")

    @staticmethod
    def _local_ip() -> str:
        """Devuelve la IP de la interfaz usada para salir a la red."""
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as probe:
            try:
                probe.connect(("8.8.8.8", 80))
                return probe.getsockname()[0]
            except OSError:
                return "desconocida"


if __name__ == "__main__":
    port = int(sys.argv[1]) if len(sys.argv) > 1 else DEFAULT_PORT
    try:
        BasicTcpServer(HOST, port).serve_forever()
    except KeyboardInterrupt:
        print("\n[SERVER] Detenido.")
