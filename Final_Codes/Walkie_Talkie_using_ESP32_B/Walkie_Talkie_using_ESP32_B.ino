#include <WiFi.h>
#include <WiFiUdp.h>
#include <driver/i2s.h>

const char* ssid = "USERNAME"; // Your WiFi Username
const char* password = "Password"; // Yuor WiFi Password

// ===== CHANGE THIS PER DVICE =======
IPAddress local_IP(192,168,29,201);   // Set unique IP for this device
IPAddress peerIP(192,168,29,200);    //  Set IP for other device 
IPAddress gateway(192,168,29,1);
IPAddress subnet(255,255,255,0);
const int port = 1234;
WiFiUDP udp;

// I2S Settings 
#define I2S_NUM I2S_NUM_0
#define I2S_SAMPLE_RATE 32000
#define I2S_BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_32BIT
#define I2S_READ_LEN 2048
#define I2S_DMA_BUF_LEN 1024

#define I2S_BCLK_PIN 26
#define I2S_WS_PIN 25
#define I2S_DIN_PIN 22 // To DAC
#define I2S_DOUT_PIN 34 // From Mic

#define LED_PIN 2
#define PTT_PIN 15

#define MIC_POWER_PIN 18  // Controls INMP441 VDD
#define DAC_POWER_PIN 19 //  Controls MAX98357A VDD

const float volumeBoost = 5.0;
int32_t i2sBuffer[I2S_READ_LEN / 4];
int32_t amplifiedBuffer[I2S_READ_LEN / 4];

bool i2sInstalled = false;
bool isTransmitMode = false;

void setupI2S_RX(){
  if(i2sInstalled){
    i2s_driver_uninstall(I2S_NUM);
    i2sInstalled = false;
  }

 i2s_config_t config = {
    .mode =  (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = I2S_DMA_BUF_LEN,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pins = {
    .bck_io_num = I2S_BCLK_PIN,
    .ws_io_num = I2S_WS_PIN,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_DOUT_PIN
  };

  i2s_driver_install(I2S_NUM, &config, 0, NULL);
  i2s_set_pin(I2S_NUM, &pins);
  i2s_zero_dma_buffer(I2S_NUM);
  i2sInstalled = true;
}

void setupI2S_TX(){
  if(i2sInstalled){
    i2s_driver_uninstall(I2S_NUM);
    i2sInstalled = false;
  }

 i2s_config_t config = {
    .mode =  (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = I2S_DMA_BUF_LEN,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pins = {
    .bck_io_num = I2S_BCLK_PIN,
    .ws_io_num = I2S_WS_PIN,
    .data_out_num = I2S_DIN_PIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_NUM, &config, 0, NULL);
  i2s_set_pin(I2S_NUM, &pins);
  i2s_zero_dma_buffer(I2S_NUM);
  i2sInstalled = true;
}

void connectWiFi(){
  WiFi.config(local_IP,gateway,subnet);
  WiFi.begin(ssid,password);
  Serial.print("Connecting to Wi-Fi");
   while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: ");
  Serial.println(WiFi.localIP());
}


void setup(){
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PTT_PIN, INPUT_PULLUP);  //Active LOW
  pinMode(MIC_POWER_PIN, OUTPUT);  // Controls INMP441 VDD
  pinMode(DAC_POWER_PIN, OUTPUT);  // Controls MAX98357A VDD
  connectWiFi();
  udp.begin(port);
}

void loop(){
  bool pttPressed = digitalRead(PTT_PIN) == LOW;

  if(pttPressed){
    // ==== Transmit Mode ====
    if(!isTransmitMode){
      setupI2S_RX();
      isTransmitMode = true;
      digitalWrite(MIC_POWER_PIN, HIGH);   // Enable MIC
      digitalWrite(DAC_POWER_PIN, LOW);    // Disable DAC
    }

     size_t bytesRead = 0;
   esp_err_t result = i2s_read(I2S_NUM, (void*)i2sBuffer, sizeof(i2sBuffer), &bytesRead, 10 / portTICK_PERIOD_MS);

   if(result == ESP_OK && bytesRead > 0){
    udp.beginPacket(peerIP,port);
    udp.write((uint8_t*)i2sBuffer,bytesRead);
    udp.endPacket();
    digitalWrite(LED_PIN, HIGH);
   }
  } else {
   // ==== Receive Mode ====
   if(isTransmitMode){
    setupI2S_TX();
    isTransmitMode = false;
    digitalWrite(MIC_POWER_PIN, LOW);   // Disable MIC
    digitalWrite(DAC_POWER_PIN, HIGH);    // Eable DAC
   }
    
   int packetSize = udp.parsePacket();
   if(packetSize > 0 && packetSize <= sizeof(i2sBuffer)){
    udp.read((uint8_t*)i2sBuffer, packetSize);
    int sampleCount = packetSize / sizeof(int32_t);

    for(int i = 0; i < sampleCount; i++){
      int32_t amplified = i2sBuffer[i] * volumeBoost;
      if(amplified > INT32_MAX) amplified = INT32_MAX;
      else if(amplified < INT32_MIN) amplified = INT32_MIN;
      amplifiedBuffer[i] = (int32_t)amplified;
    }

    size_t written;
    i2s_write(I2S_NUM, amplifiedBuffer, sampleCount * sizeof(int32_t), &written, 10 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, HIGH);
   }else{
    digitalWrite(LED_PIN, LOW);
  }
  }
}