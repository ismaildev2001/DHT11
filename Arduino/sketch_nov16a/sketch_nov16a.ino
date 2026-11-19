
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>

// ========== CONFIGURATION DHT11 ==========
#define DHTPIN D2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// ========== CONFIGURATION WIFI ==========
const char* WIFI_SSID = "HABIB";
const char* WIFI_PASSWORD = "12345678";

// ========== SERVEUR DJANGO ==========
const char* SERVER_URL = "http://10.217.231.51:8000/post/";
const char* SERVER_HOST = "10.217.231.51";
const int SERVER_PORT = 8000;

// ========== TEST CONNEXION TCP ==========
void testTCPConnection() {

    WiFiClient client;

    Serial.println("\n========== TEST TCP ==========");

    Serial.print("[ESP] IP : ");
    Serial.println(WiFi.localIP());

    Serial.print("[ESP] Gateway : ");
    Serial.println(WiFi.gatewayIP());

    Serial.print("[ESP] Subnet : ");
    Serial.println(WiFi.subnetMask());

    Serial.println("[TCP] Connexion vers Django...");

    if (client.connect(SERVER_HOST, SERVER_PORT)) {

        Serial.println("[TCP] CONNEXION REUSSIE !");

        client.stop();

    } else {

        Serial.println("[TCP] ECHEC DE CONNEXION !");
    }

    Serial.println("==============================");

}

// ========== TEST HTTP GET ==========
void testHTTPConnection() {

    WiFiClient client;
    HTTPClient http;

    Serial.println("\n========== TEST HTTP GET ==========");

    if (http.begin(client, "http://10.217.231.51:8000/list/")) {

        Serial.println("[HTTP] Envoi GET /list/");

        int httpCode = http.GET();

        Serial.print("[HTTP] Code retour : ");
        Serial.println(httpCode);

        if (httpCode > 0) {
            Serial.println("[HTTP] Reponse Django :");
            Serial.println(http.getString());
        } else {
            Serial.println("[HTTP] Erreur :");
            Serial.println(http.errorToString(httpCode));
        }

        http.end();

    } else {

        Serial.println("[HTTP] Impossible de demarrer HTTP !");
    }

    Serial.println("===================================");

}

// ========== CONNEXION WIFI ==========
void connectWiFi() {

    Serial.println("\n[WIFI] Connexion en cours...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts < 30) {

        delay(500);
        Serial.print(".");

        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {

        Serial.println("\n[WIFI] CONNECTE !");

        Serial.print("[WIFI] Adresse IP ESP8266 : ");
        Serial.println(WiFi.localIP());

    } else {

        Serial.println("\n[WIFI] ECHEC DE CONNEXION !");
    }

}

// ========== SETUP ==========
void setup() {

    Serial.begin(115200);
    delay(1000);

    Serial.println("\n========== DEMARRAGE ESP8266 ==========");

    // Initialisation DHT11
    Serial.println("[DHT] Initialisation...");

    dht.begin();

    delay(2000);

    Serial.println("[DHT] Initialisation terminee.");

    // Connexion WiFi
    connectWiFi();

    if (WiFi.status() == WL_CONNECTED) {

        // Informations réseau
        Serial.print("[ESP] Gateway : ");
        Serial.println(WiFi.gatewayIP());

        Serial.print("[ESP] Subnet : ");
        Serial.println(WiFi.subnetMask());

        // Test TCP
        testTCPConnection();

        // Test HTTP GET
        testHTTPConnection();

    }

    Serial.println("\n========== FIN INITIALISATION ==========");

}

// ========== LOOP ==========
void loop() {

    Serial.println("\n========== NOUVELLE MESURE ==========");

    // 1. Vérification WiFi
    if (WiFi.status() != WL_CONNECTED) {

        Serial.println("[WIFI] DECONNECTE !");
        WiFi.reconnect();

        delay(3000);
        return;
    }

    Serial.println("[WIFI] CONNECTE");

    // 2. Lecture DHT11
    Serial.println("[DHT] Lecture du capteur...");

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    Serial.print("[DHT] Temperature : ");
    Serial.println(temperature);

    Serial.print("[DHT] Humidite : ");
    Serial.println(humidity);

    // 3. Vérification données
    if (isnan(temperature) || isnan(humidity)) {

        Serial.println("[ERREUR] Lecture DHT11 impossible !");

        delay(5000);
        return;
    }

    Serial.println("[DHT] Lecture reussie !");

    // 4. Construction JSON
    String json = "{\"temperature\":" + String(temperature, 1)
                + ",\"humidity\":" + String(humidity, 1) + "}";

    Serial.println("[JSON] " + json);

    // 5. Envoi POST vers Django
    WiFiClient client;
    HTTPClient http;

    Serial.println("[HTTP] Connexion au serveur Django...");

    if (http.begin(client, SERVER_URL)) {

        http.addHeader("Content-Type", "application/json");

        int httpCode = http.POST(json);

        Serial.print("[HTTP] Code retour : ");
        Serial.println(httpCode);

        if (httpCode > 0) {

            Serial.println("[HTTP] Reponse Django :");
            Serial.println(http.getString());

            if (httpCode == 201) {
                Serial.println("[SUCCESS] Mesure enregistree !");
            }

        } else {

            Serial.print("[HTTP] Erreur : ");
            Serial.println(http.errorToString(httpCode));
        }

        http.end();

    } else {

        Serial.println("[HTTP] Impossible de demarrer HTTP !");
    }

    Serial.println("=====================================");

    delay(5000);

}
