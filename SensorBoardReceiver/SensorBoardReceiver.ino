/* SensorBoardSender.ino
 *  
 * PixelBlaze Sensor board -> Multicast datagram receiver
 * ESP8266 version
 *
 * Recives data from a sensor board multicast sender and
 * forwards it via serial port to a Pixelblaze (or other device.) One sender
 * can support any number of receivers on a network. 
 * 
 * To use: 
 * Connect the ESP82866 to the Pixelblaze's power (3v3) and ground pins, and connect GPIO15 (D8) on
 * on the ESP8266 to the RX pin on the Pixelblaze's expansion header.
 * Set the _SSID and _PASS variables for your wifi network.
 * Compile, upload and go!
 * 
 * Requires the Arduino IDE, with the community ESP8266 library installed.
 * 
 * 6/2022 ZRanger1
 * Distributed under the MIT license
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>

// Data transfer setup
#define BAUD            115200    // serial data rate

// Network setup - ssid and pass are hardwired.  You'll need to recompile with proper
// information for your setup.
#define _SSID "MySSID"            // Your WiFi SSID goes here.  
#define _PASS "MyPassword"     // Your WiFi password goes here.
#define LISTEN_PORT 6000           // UDP listen port

// what's in a Pixelblaze Sensor Board packet?
typedef struct {
    uint8_t  headerTag[6];  
    uint16_t frequency[32];
    uint16_t energyAverage;
    uint16_t maxFreqMagnitude;
    uint16_t maxFrequency;
    int16_t accelerometer[3];   // note: signed ints
    uint16_t light;   
    uint16_t analogInputs[5];   // note: actually 12-bit data
    uint8_t endTag[4];   
} __attribute__((packed)) SensorPacket;

WiFiUDP Udp;
Stream* logger;
IPAddress serverIP(224,0,0,12);
SensorPacket dataFrame;

void initDataFrame() {
  // zero entire structure
  uint8_t * buf = (uint8_t *) &dataFrame;
  for (int i = 0; i < sizeof(SensorPacket); i++) {
    buf[i] = 0;    
  }
  
  // set header string that identifies a sensor board packet
  dataFrame.headerTag[0] = 'S';
  dataFrame.headerTag[1] = 'B';
  dataFrame.headerTag[2] = '1';
  dataFrame.headerTag[3] = '.';
  dataFrame.headerTag[4] = '0';    
  dataFrame.headerTag[5] = 0;   

  // set end tag string
  dataFrame.endTag[0] = 'E';
  dataFrame.endTag[1] = 'N';
  dataFrame.endTag[2] = 'D';
  dataFrame.endTag[3] = 0;       
}

// setup()
//
// We'll use a UART-based serial port rather than software serial because it
// theoretically takes some of the load off of the ESP. 
// Anyway, we create a new software serial port for logging and swap jobs with the UART so it 
// can speak over GPIO. Data goes out on pin D8 (GPIO15).  If you need to hook up 
// a receiver, use on pin D7 (GPIO13).
void setup() {

  Serial.begin(BAUD);  
  Serial.swap(); 
  Serial.setRxBufferSize(1024);  
  
// use HardwareSerial0 pins so we can still log to the
// regular usbserial chips
  SoftwareSerial* ss = new SoftwareSerial(3, 1);
  ss->begin(BAUD);
  ss->enableIntTx(false);
  logger = ss;
  logger->println();

// Configure and connect WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(_SSID,_PASS);

  logger->print("\n\n\n");
  logger->println("Sensor Board Multicast Receiver v1.0");
  logger->println("Connecting to wifi...");
  while(WiFi.status() != WL_CONNECTED) {
    logger->print('.');
    delay(500);
  }

  logger->println("Connected!");
  logger->print("IP address: ");
  logger->println(WiFi.localIP());  

  initDataFrame();

// start listening for incoming packets  
  Udp.beginMulticast(WiFi.localIP(), serverIP, LISTEN_PORT);  
  logger->println("Setup: Success");
}

// main loop
void loop() {

  // process incoming UDP Packets
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read((uint8_t *) &dataFrame, sizeof(dataFrame));
  }

// send data to Pixelblaze
  Serial.write((uint8_t *) &dataFrame,sizeof(dataFrame)); 

}
