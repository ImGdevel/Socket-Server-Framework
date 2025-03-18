![소캣 서버 아키택쳐-페이지-3 drawio (1)](https://github.com/user-attachments/assets/d7dcb3db-2a14-46c4-9a90-3bcb58166379)

🇺🇸 [English](https://github.com/ImGdevel/Custom-Socket-Server) /
🇰🇷 [Korean](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_kr.md) /
🇨🇳 [中文](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_cn.md) /
🇪🇸 Español

### Este proyecto es un marco de servidor de sockets basado en el patrón Reactor-Worker, utilizando E/S asíncrona basada en epoll y una arquitectura de pool de hilos. Con una configuración simple, se puede utilizar para servidores de chat, servidores de juegos, sistemas distribuidos y más.

> ### Objetivos principales
> - **Alto rendimiento y escalabilidad** – Utiliza E/S asíncrona basada en epoll y un pool de hilos para manejar de manera eficiente decenas de miles de conexiones simultáneas.
> - **Diseño modular** – Compatible con integración de bases de datos, patrón Multi-Reactor y varios formatos de API y mensajes (JSON, Protobuf, etc.).
> - **Optimizado para servidores de juegos** – Soporta TCP y UDP simultáneamente, procesamiento de paquetes de baja latencia y extensión de protocolos personalizados.
> - **Apto para entornos de producción** – Diseñado para implementaciones reales, no solo como un proyecto de demostración.

Este proyecto no es solo un simple servidor de sockets, sino un marco de servidor de red universal que puede usarse en juegos y otros servicios en tiempo real.

<br>

## 🛠 Tecnologías utilizadas
- **Patrón Reactor (multiplexión de E/S, E/S no bloqueante)**
- **Procesamiento asíncrono basado en colas de mensajes**
- **Manejadores de eventos expandibles (soporte para eventos personalizados)**
- **Preprocesamiento de mensajes basado en Filter Chain (soporte para filtros personalizados)**
- **Integración con Redis, bases de datos y Message Broker**

## 📂 Arquitectura del servidor

![소캣 서버 아키택쳐-페이지-2 drawio](https://github.com/user-attachments/assets/bc8727bb-dbf8-4433-9099-5108b600afbf)

1. **Cliente → Balanceador de carga**: El balanceador de carga distribuye las solicitudes de los clientes al `Reactor`.
2. **Reactor (Productor) → Cola de mensajes**: `Reactor` procesa las solicitudes del cliente de forma asíncrona y las almacena en la cola de mensajes.
3. **Cola de mensajes → Worker (Consumidor)**: `Worker` recupera los mensajes de la cola y los procesa con `FilterChain`.
4. **Despachador de mensajes → Manejador de eventos**: Los mensajes se envían al manejador correspondiente para su procesamiento final, interactuando con `Redis`, bases de datos y `MessageBroker`.

<br>

---

## 🏗️ Inicio rápido

### 1️⃣ Instalar paquetes necesarios
```sh
sudo apt update
sudo apt install -y g++ cmake make
```

### 2️⃣ Clonar y compilar el proyecto
```sh
git clone https://github.com/your-repo/socket-server-framework.git
cd socket-server-framework
make
```

### 3️⃣ Ejecutar el servidor
```sh
./build/server
```

## 🛠️ Configuración personalizada del servidor

### 1️⃣ Definir un manejador de eventos
```cpp
// CustomEventHandler.h
#include "IEventHandler.h"
#include "ClientRequest.h"
#include "Logger.h"
#include <unordered_map>

class CustomEventHandler : public IEventHandler {
public:
    std::unordered_map<std::string, HandlerFunc> createHandlers() const override {
        return {
            {"LOGIN", [this](const ClientRequest& request) { this->onLogin(request); }}
        };
    }

private:
    void onLogin(const ClientRequest& request) {
        Logger::info("Login: " + request.toString());
    }
};
```

### 2️⃣ Configurar y ejecutar el servidor
```cpp
// main.cpp
#include "Server.h"
#include "CustomEventHandler.h"

int main() {
   CustomEventHandler handler;
   auto server = Server::Builder()
       .setPort(9090)
       .setWorkerCount(8)
       .setEventHandler(handler)
       .setMessageType("json")
       .build();
    server->run();
}
```

### 3️⃣ Compilar y ejecutar
```sh
make
./build/server
```

<br>

## 🐳 Ejecutar con Docker

### 1️⃣ Construir la imagen de Docker
```sh
docker build -t socket-server .
```

### 2️⃣ Ejecutar el contenedor
```sh
docker run --rm -p 8080:8080 socket-server
```
Por defecto, el servidor se ejecuta en el puerto **8080**. Puedes cambiarlo con `-p <puerto_host>:<puerto_contenedor>`.

## 🔧 Comandos adicionales
```sh
make download  # Instalar dependencias
make test      # Ejecutar pruebas
make clean     # Eliminar archivos de compilación
make clean-all # Eliminar archivos de compilación y dependencias
```

<br>

---

## 📜 Licencia

Este proyecto sigue una licencia de código abierto.

- **Uso educativo**: completamente gratuito
- **Uso personal y para startups pequeñas**: bajo licencia MIT
- **Uso empresarial y comercial**: requiere autorización previa

Consulta el archivo LICENSE para más detalles.

## 📜 Contribuciones

¡Las contribuciones para mejorar el proyecto son bienvenidas!
Si tienes dudas o sugerencias, abre un [GitHub Issue](https://github.com/your-repo/socket-server-framework/issues).
'''

