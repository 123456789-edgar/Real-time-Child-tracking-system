//Importing Libraries.
 
#include <SPI.h>
#include <WiFi.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

//Internet connectivity to the WiFi shield

char ssid[] = "...";        // Network SSID (name) 
char pass[] = "...";    // Network Password (use for WPA, 
                                  // or use as the key for WEP)

int keyIndex = 0;                 // Network Key Index Number 
                                  // (needed only for WEP)
 
int status = WL_IDLE_STATUS;

WiFiClient client;

void connectToInternet()
{
  status = WiFi.status();
  
  // Check for the presence of the shield
  if (status == WL_NO_SHIELD) 
  {
    Serial.println("[ERROR] WiFi Shield Not Present");
    // Do nothing
    while (true);
  }

  // Attempt to connect to WPA/WPA2 Wifi network
  while ( status != WL_CONNECTED) 
  {
    Serial.print("[INFO] Attempting Connection - WPA SSID: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);
  }

  // Connection successful
  Serial.print("[INFO] Connection Successful");
  Serial.print("");  
  printConnectionInformation();
  Serial.println("-----------------------------------------------");
  Serial.println("");  
}

void printConnectionInformation() 
{
  // Print Network SSID
  Serial.print("[INFO] SSID: ");
  Serial.println(WiFi.SSID());

  // Print Router's MAC address
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("[INFO] BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // Print received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("[INFO] Signal Strength (RSSI): ");
  Serial.println(rssi);

  // Print encryption type
  byte encryption = WiFi.encryptionType();
  Serial.print("[INFO] Encryption Type: ");
  Serial.println(encryption, HEX);

  // Print WiFi Shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("[INFO] IP Address: ");
  Serial.println(ip);

  // Print MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("[INFO] MAC Address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

// GPS Setup - Variables & Functions
 
TinyGPS gps;
SoftwareSerial ss(2, 3); // GPS TX = Arduino D2, GPS RX = Arduino D3

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

void getGPSCoordinates()
{
  float latitude;
  float longitude;
  unsigned long age = 0;
   
  gps.f_get_position(&latitude, &longitude, &age);

  smartdelay(10000);
  
  // Transmit sensor data  
  if(latitude != TinyGPS::GPS_INVALID_F_ANGLE && 
                               longitude != TinyGPS::GPS_INVALID_F_ANGLE)
  {
    transmitSensorData(latitude, longitude); 
  }
  else
  {
    Serial.println("[INFO] Searching for Satellite");
  }  
}
 
// Data Publish - Variables & Functions

//IP address of the server
char server[] = {"http://localhost/gpstracker/"};
int port = 3306; //the operation port for MySQL on the xampp server

unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10L * 1000L;

void transmitSensorData(float latitude, float longitude)
{
  // Read all incoming data (if any)
  while (client.available()) 
  {
    char c = client.read();
  }

  if (millis() - lastConnectionTime > postingInterval) 
  {
    client.stop();
  
    Serial.println("[INFO] Connecting to Server");

    String requestData = "clientID=Edgar&latitude=" + String(latitude) + 
                                       "&longitude=" + String(longitude);
    Serial.println("[INFO] Query String: " + requestData);
    
    // Prepare data or parameters that need to be posted to server
    if (client.connect(server, port)) 
    {
      Serial.println("[INFO] Server Connected - HTTP GET Started");
       
      // Make a HTTP request:
      client.println("GET /path location here" + requestData + 
                                                              " HTTP/1.1");
      client.println("Host: " + String(server));
      client.println("Connection: close");
      client.println();    

      lastConnectionTime = millis();
    
      Serial.println("[INFO] HTTP GET Completed");
    } 
    else 
    {
      // Connection to server:port failed
      Serial.println("[ERROR] Connection Failed");
    }
  }

  Serial.println("-----------------------------------------------");  
}

 // Standard Arduino Functions - setup(), loop()

void setup()
{  
  // Initialize serial port  
  Serial.begin(115200);
  
  // Initialize serial port for GPS data
  ss.begin(9600);

  //Connect Arduino to internet
  connectToInternet();
}

void loop()
{
  // Get GPS Coordinates
  getGPSCoordinates();
}
