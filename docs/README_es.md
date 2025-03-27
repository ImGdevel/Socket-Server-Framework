![Socket Server Architecture](https://github.com/user-attachments/assets/d7dcb3db-2a14-46c4-9a90-3bcb58166379)

🇺🇸 [English](https://github.com/ImGdevel/Socket-Server-Framework) /
🇰🇷 [한국어](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_kr.md) /
🇨🇳 [中文](https://github.com/ImGdevel/Socket-Server-Framework/blob/main/docs/README_cn.md) /
🇪🇸 Español

# Socket Server Framework

Este proyecto es un **framework de servidor de sockets de alto rendimiento** que implementa el **patrón Reactor-Worker** con **E/S asíncrona basada en epoll** y **arquitectura de pool de hilos**. El framework proporciona una base modular para construir aplicaciones de red escalables como servidores de chat, servidores de juegos y sistemas distribuidos.

## Principios de Diseño Fundamentales

- **Arquitectura Basada en Eventos** – Construida sobre el patrón Reactor para multiplexación eficiente de E/S
- **Procesamiento Asíncrono** – Operaciones de E/S no bloqueantes con procesamiento de mensajes basado en pool de hilos
- **Flexibilidad de Protocolo** – Soporte para protocolos de transporte TCP y UDP
- **Agnóstico de Formato de Mensaje** – Analizadores conectables para JSON, Protobuf y formatos personalizados
- **Listo para Producción** – Manejo integral de errores, logging y gestión de recursos

## Arquitectura Técnica

![Server Architecture Diagram](https://github.com/user-attachments/assets/bc8727bb-dbf8-4433-9099-5108b600afbf)

### Componentes Clave

**Capa Reactor**
- Implementaciones `ReactorTCP` y `ReactorUDP` para manejo de eventos específicos del protocolo
- Multiplexación de E/S basada en epoll para gestión eficiente de conexiones
- Operaciones de socket no bloqueantes con notificación de eventos

**Gestión de Sesiones**
- `ClientSessionTCP` y `ClientSessionUDP` para gestión del estado de conexión
- Manejo del ciclo de vida de sesión específico del protocolo
- Limpieza de recursos y seguimiento de conexiones

**Pipeline de Procesamiento de Mensajes**
- `FilterChain` para preprocesamiento y validación de solicitudes
- `MessageDispatcher` para enrutar mensajes a manejadores apropiados
- `EventRegistry` para registro dinámico de manejadores

**Modelo de Hilos**
- Implementación `ThreadPool` para gestión escalable de workers
- Patrón productor-consumidor con colas de mensajes
- Cantidad configurable de hilos worker

## Stack Tecnológico

- **Lenguaje**: C++17
- **Modelo de E/S**: epoll (Linux)
- **Concurrencia**: Pool de hilos con colas de mensajes
- **Formatos de Mensaje**: JSON (RapidJSON), Protocol Buffers
- **Dependencias Externas**: Redis (hiredis), MySQL, Google Test
- **Sistema de Construcción**: GNU Make con gestión automatizada de dependencias

---

## Primeros Pasos

### Prerrequisitos

```bash
sudo apt update
sudo apt install -y g++ cmake make
```

### Construir y Ejecutar

```bash
git clone <repository-url>
cd socket-server-framework
make download  # Instalar dependencias
make           # Construir el proyecto
./build/server # Ejecutar el servidor
```

El servidor inicia con la configuración predeterminada de `ServerConfig` y utiliza el `TestJSONEventHandler`.

---

## Implementación Personalizada

### 1. Definir Manejador de Eventos

```cpp
#include "IEventHandler.h"
#include "ClientRequest.h"
#include "Logger.h"

class CustomEventHandler : public IEventHandler {
public:
    std::unordered_map<std::string, HandlerFunc> createHandlers() const override {
        return {
            {"LOGIN", [this](const ClientRequest& request) { handleLogin(request); }},
            {"MESSAGE", [this](const ClientRequest& request) { handleMessage(request); }}
        };
    }

private:
    void handleLogin(const ClientRequest& request) {
        Logger::info("Inicio de sesión de usuario: " + request.toString());
        // Lógica de implementación
    }
    
    void handleMessage(const ClientRequest& request) {
        Logger::info("Mensaje recibido: " + request.toString());
        // Lógica de implementación
    }
};
```

### 2. Configurar Servidor

```cpp
#include "Server.h"
#include "CustomEventHandler.h"

int main() {
    CustomEventHandler handler;
    
    auto server = Server::Builder()
        .setPort(9090)
        .setWorkerCount(4)
        .setTransportProtocol("TCP")  // o "UDP"
        .setMessageType("json-rapid") // o "json", "protobuf"
        .setEventHandler(handler)
        .build();
    
    server->run();
    return 0;
}
```

### 3. Filtros Personalizados

```cpp
#include "IFilter.h"

class AuthenticationFilter : public IFilter {
public:
    bool process(ClientRequest& request) override {
        // Lógica de autenticación
        return true; // Continuar procesamiento
    }
};

// Agregar al servidor
.addFilter(std::make_unique<AuthenticationFilter>())
```

---

## Despliegue con Docker

### Construir Contenedor

```bash
# Construir el proyecto primero
make

# Construir imagen Docker
docker build -t socket-server-framework .
```

### Ejecutar Contenedor

```bash
docker run --rm -p 8080:8080 socket-server-framework
```

---

## Comandos Disponibles

```bash
make download   # Descargar y construir dependencias externas
make            # Construir el ejecutable del servidor
make test       # Ejecutar pruebas unitarias
make clean      # Remover artefactos de construcción
make clean-all  # Remover artefactos de construcción y dependencias
```

---

## Estructura del Proyecto

```
src/
├── main.cpp                    # Punto de entrada de la aplicación
├── server/
│   ├── Server.h/.cpp          # Clase principal del servidor con patrón Builder
│   ├── ServerConfig.h/.cpp    # Gestión de configuración
│   ├── reactor/               # Implementaciones de loop de eventos
│   │   ├── Reactor.h          # Interfaz abstracta de reactor
│   │   ├── ReactorTCP.cpp     # Reactor específico para TCP
│   │   └── ReactorUDP.cpp     # Reactor específico para UDP
│   ├── session/               # Gestión de conexiones
│   │   ├── ClientSessionTCP.cpp
│   │   └── ClientSessionUDP.cpp
│   ├── threadpool/            # Gestión de hilos worker
│   ├── dispatcher/            # Enrutamiento de mensajes
│   ├── filter/                # Preprocesamiento de solicitudes
│   ├── handler/               # Manejadores de eventos
│   └── messages/              # Análisis de mensajes
└── utils/                     # Funciones de utilidad
```

---

## Pruebas

El proyecto incluye pruebas unitarias integrales utilizando el framework Google Test:

```bash
make test
```

La cobertura de pruebas incluye:
- Inicialización y configuración del servidor
- Operaciones de cola de workers
- Gestión de sesiones de cliente
- Procesamiento de cadena de filtros

---

## Licencia

Este proyecto está disponible bajo múltiples opciones de licencia:

- **Uso Educativo**: Gratuito para propósitos de aprendizaje y educación
- **Proyectos Personales y Pequeños**: Licencia MIT
- **Empresarial y Comercial**: Requiere acuerdo de licencia por separado

Consulte el archivo LICENSE para términos detallados.

---

## Contribuir

Damos la bienvenida a contribuciones para mejorar el framework:

1. Hacer fork del repositorio
2. Crear una rama de característica
3. Implementar cambios con pruebas apropiadas
4. Enviar un pull request

Para reportes de errores y solicitudes de características, por favor use [GitHub Issues](https://github.com/ImGdevel/Socket-Server-Framework/issues).