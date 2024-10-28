#include <WiFi.h>

// Configuración de la red WiFi
const char *ssid = "BUAP_Estudiantes";
const char *password = "f85ac21de4*";

WiFiServer server(80);

// Pines para los LEDs y PWM
const int ledPin1 = 5;
const int ledPin2 = 18;
const int pwmPin = 19;
int pwmValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);  // LED 1
  pinMode(ledPin2, OUTPUT);  // LED 2
  pinMode(pwmPin, OUTPUT);   // Pin PWM

  // Conexión a la red WiFi
  Serial.print("Conectando a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  // Esperar conexión
  int timeoutCounter = 0;  // Para evitar bucles infinitos en caso de fallo
  while (WiFi.status() != WL_CONNECTED && timeoutCounter < 30) {
    delay(500);
    Serial.print(".");
    timeoutCounter++;  // Incrementar contador de tiempo de espera
  }

  if (WiFi.status() == WL_CONNECTED) {
    // Mostrar IP asignada
    Serial.println("");
    Serial.println("WiFi conectado.");
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());
    server.begin();
  } else {
    Serial.println("Error al conectar WiFi. Verifica el SSID y la contraseña.");
  }
}

void loop() {
  WiFiClient client = server.available();  // Escuchar para clientes entrantes

  if (client) {
    Serial.println("Nuevo Cliente.");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Encabezado de la respuesta HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // HTML de la página web con los botones
client.println("<div><h1>Control de LEDs y PWM</h1></div>");
client.println("<p><img class=\"imageLeft\" src=\"https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSMXzNanLrfIabgWvwqkhUHWgj0xA_q6HHXsQ&amp;s\" alt=\"FCE\" width=\"199\" height=\"253\" /></p>");
client.println("<p>Integrantes: Dares Cuautle Ortega, Lance Martinez Corto, Arturo Crisanto Cuatlayotl, Angel Uriel Cruz Gonzalez</p>");
client.println("<div><button onclick=\"controlLED1('H')\">Encender LED 1</button>");
client.println("<button onclick=\"controlLED1('L')\">Apagar LED 1</button></div>");
client.println("<div><button onclick=\"controlLED2('H')\">Encender LED 2</button>");
client.println("<button onclick=\"controlLED2('L')\">Apagar LED 2</button></div>");
client.println("<div><input type='number' id='pwmValue' min='0' max='255' value='0'>");
client.println("<button onclick=\"setPWM()\">Establecer PWM</button></div>");
client.println("<script>");
client.println("function controlLED1(action) { var xhr = new XMLHttpRequest(); xhr.open('GET', '/LED1_' + action, true); xhr.send(); }");
client.println("function controlLED2(action) { var xhr = new XMLHttpRequest(); xhr.open('GET', '/LED2_' + action, true); xhr.send(); }");
client.println("function setPWM() { var val = document.getElementById('pwmValue').value; var xhr = new XMLHttpRequest(); xhr.open('GET', '/PWM_' + val, true); xhr.send(); }");
client.println("</script>");
            
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // Control de LEDs y PWM basado en las solicitudes GET
        if (currentLine.endsWith("GET /LED1_H")) {
          digitalWrite(ledPin1, HIGH);
        }
        if (currentLine.endsWith("GET /LED1_L")) {
          digitalWrite(ledPin1, LOW);
        }
        if (currentLine.endsWith("GET /LED2_H")) {
          digitalWrite(ledPin2, HIGH);
        }
        if (currentLine.endsWith("GET /LED2_L")) {
          digitalWrite(ledPin2, LOW);
        }
        if (currentLine.startsWith("GET /PWM_")) {
          String pwmString = currentLine.substring(9);  // Obtener el valor de PWM desde la URL
          pwmValue = pwmString.toInt();                 // Convertir a número
          analogWrite(pwmPin, pwmValue);                // Establecer el valor de PWM
        }
      }
    }
    // Cerrar la conexión del cliente
    client.stop();
    Serial.println("Cliente desconectado.");
  }
}
