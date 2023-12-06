
# ![Static Badge](https://img.shields.io/badge/Project_Code-red)

/*
Name: Shubham Anil Khangar
STD : M.Sc. (Electronics Science)
University: Savitribai Phule Pune University
*/


// Arduino ide dependancies required for these code
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include<DHT.h>


DHT dht(D5,DHT11); //connect the data pin of dht11 to D5


// Provide your wifi credentials here 
const char* ssid = "Shubham";
const char* password = "876543210";


// Provide your web page login credentials
const char* http_username = "shubham";
const char* http_password = "khangar";

// parameters for processing the incoming request from web browser 
const char* PARAM_INPUT_1 = "state";
const char* PARAM_INPUT_2 = "start";


// Create AsyncWebServer object on port 80 where the server is listen's to the incoming data 
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// html webpage file stored in the program memory using the PROGMEM
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.6rem; color: rgb(238, 18, 18);}
    body {max-width: 600px; margin:0px auto; padding-bottom: 10px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; tr ansition: .4s; border-radius: 68px}
    .butt{color: red; background-color: black;}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>

<!--body scope-------------------------------------------------------->
<body>

  <h2>ESP Web Server System</h2>

  <p>LedBuiltIn-> <span id="state">%STATE%</span></p>
  BuiltInLed: <input type="checkbox" id="myCheck">
  <button onclick="check()">ON</button>
  <button onclick="uncheck()">OFF</button>
  <hr>

  <p>LedGpio1-> <span id="state1">%STATE1%</span></p>
  LedGpio1: <input type="checkbox" id="myCheck1">
  <button onclick="check1()">ON</button>
  <button onclick="uncheck1()">OFF</button>
  <hr>

  <p>sensor data-><span id="state2">%STATE2%</span></p>
  <button onclick="refresh()">refresh</button>
  <hr>
  <br>
  <button onclick="logoutButton()" class="butt">Logout</button>

<!--JS--------------------------------------------------------------------------------->
<script>



function refresh() {
    var xhr = new XMLHttpRequest();
            
            // Configure it to make a GET request to a server endpoint
            xhr.open("GET", "/sensor", true);

            // Set up a callback function to handle the response
            xhr.onreadystatechange = function() {
                if (xhr.readyState === 4 && xhr.status === 200) {
                    // Handle the response from the server
                    var responseData = xhr.responseText;
                    console.log(responseData);
                    document.getElementById("state2").innerHTML = "sensor"; 
                }
            };

            // Send the request
            xhr.send();
}




function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}

function check() {
  document.getElementById("myCheck").checked = true;
   var xhr = new XMLHttpRequest();
    xhr.open("GET", "/update?state=1", true); 
    document.getElementById("state").innerHTML = "ON";  
    xhr.send();
}

function uncheck() {
  document.getElementById("myCheck").checked = false;
   var xhr = new XMLHttpRequest();
    xhr.open("GET", "/update?state=0", true); 
    document.getElementById("state").innerHTML = "OFF";  
    xhr.send();
}

function check1() {
  document.getElementById("myCheck1").checked = true;
   var xhr = new XMLHttpRequest();
    xhr.open("GET", "/update?start=1", true); 
    document.getElementById("state1").innerHTML = "ON";  
    xhr.send();
}

function uncheck1() {
  document.getElementById("myCheck1").checked = false;
   var xhr = new XMLHttpRequest();
    xhr.open("GET", "/update?start=0", true); 
    document.getElementById("state1").innerHTML = "OFF";  
    xhr.send();
}
</script>
</body>
</html>
)rawliteral";

const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <p>Logged out or <a href="/">return to homepage</a>.</p>
  <p><strong>Note:</strong> close all web browser tabs to complete the logout process.</p>
</body>
</html>
)rawliteral";



void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
      
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

// Replaces placeholder with button section in your web page
String processor(const String& var){

  if (var == "STATE"){
    if(digitalRead(D0)){
      return "ON";
    }
    else {
      return "OFF";
    }
  }
    if (var == "STATE1"){
    if(digitalRead(D1)){
      return "ON";
    }
    else {
      return "OFF";
    }
    }
     if (var == "STATE2"){
    float t=dht.readTemperature();
    delay(1000);
     t=dht.readTemperature();
    delay(100);
    char buff[10];
    sprintf(buff, "%f", t);
    return buff;
  }
  return String();
}


void setup(){
  Serial.begin(115200);    //initializing the console window by 115200 baud rate
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);  // led at 0
  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);  // led at 1
  
  // Connect to Wi-Fi network 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address assinged by the hotspot
  Serial.println(WiFi.localIP());
  delay(100);
  dht.begin(); //initialized the dht11 sensor module with pin D5
  
  initWebSocket();
  // server request handler for the root static html webpage 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){

    if(!request->authenticate(http_username, http_password))   //these function is handeling the login process in the webpage 
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);  //these function is rendering the out root html webpage to the client logged in using the ip address of the node device
  });
    
  // server request handler for the logout html webpage 
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(401);
  });

  // server request handler for the logged-out  html webpage to send the login page to client 
  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", logout_html, processor);
  });

   // server request handler for the logged-out  html webpage to send the login page to client 
  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
    Serial.println("recieved sensor refresh request");
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if(!request->authenticate(http_username, http_password))     // if the client is not authorized then authenticate the client if yes then continues 
      return request->requestAuthentication();
    
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      digitalWrite(D0, inputMessage.toInt());
      Serial.println("node_state_OUT@D0-->"+inputMessage);
    }
     if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      digitalWrite(D1, inputMessage.toInt());
      Serial.println("node_state_OUT@D1-->"+inputMessage);
    }

    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }

    request->send(200, "text/plain", "OK");
  });
  
  // Start ESP Server
  server.begin();
}

//_____loop scope starts from here__________________________________________________________________________
void loop() 
{
  
}
