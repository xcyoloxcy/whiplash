#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* Returns the IMU data as both a euler angles and quaternions as the WebSerial
   3D Model viewer at https://adafruit-3dmodel-viewer.glitch.me/ expects.
 
   This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3-5V DC
   Connect GROUND to common ground

   History
   =======
   2020/JUN/01  - First release (Melissa LeBlanc-Williams)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

const int motor_pin_1=23;
const int motor_pin_2=21;
// const int motor_pin_3=21;

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(9600);
  Serial.println("WebSerial 3D Firmware"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
   
  delay(1000);

  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);
   
  /* Display some basic information on this sensor */
  displaySensorDetails();

  // analogWrite(motor_pin_3, 0);
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  /* Board layout:
         +----------+
         |         *| RST   PITCH  ROLL  HEADING
     ADR |*        *| SCL
     INT |*        *| SDA     ^            /->
     PS1 |*        *| GND     |            |
     PS0 |*        *| 3VO     Y    Z-->    \-X
         |         *| VIN
         +----------+
  */

  /* The WebSerial 3D Model Viewer expects data as heading, pitch, roll */
  // Serial.print(F("Orientation: "));
  float x_angle = 360 - (float)event.orientation.x;
  // Serial.println(F(", "));
  float y_angle = (float)event.orientation.y; 
  if(x_angle == 360 || y_angle == 0)return;
  
  // Serial.print("-1000, ");
  Serial.print(int(y_angle));
  // Serial.print(", -1000, ");
  Serial.print(",");
  Serial.println(int(x_angle));
  float k_d = 40;
  float k_d_2 = 20;
  const float y_threshold = -40;
  // const float x_threshold = 360;
  if (y_angle > y_threshold)
  {
    // if (360> x_angle && x_angle > 180)
    // {
    // float pwm = min(abs(y_angle - y_threshold) * k_d, 254);
    // analogWrite(motor_pin_1, pwm);
    // analogWrite(motor_pin_2, pwm);
    // }
    // else
    // {
      float pwm = min(abs(y_angle - y_threshold) * k_d, 200);
    analogWrite(motor_pin_1, pwm);
    analogWrite(motor_pin_2, pwm);
    // }

  }
  // else
  // {
  //   analogWrite(motor_pin_1, 0);
  // }
  // Serial.println(y_angle);

  // Serial.print(F(", "));
  // Serial.print((float)event.orientation.z);
  // Serial.println(F(""));

  /* The WebSerial 3D Model Viewer also expects data as roll, pitch, heading */
  // imu::Quaternion quat = bno.getQuat();
  
  // Serial.print(F("Quaternion: "));
  // Serial.print((float)quat.w(), 4);
  // Serial.print(F(", "));
  // Serial.print((float)quat.x(), 4);
  // Serial.print(F(", "));
  // Serial.print((float)quat.y(), 4);
  // Serial.print(F(", "));
  // Serial.print((float)quat.z(), 4);
  // Serial.println(F(""));

  // /* Also send calibration data for each sensor. */
  // uint8_t sys, gyro, accel, mag = 0;
  // bno.getCalibration(&sys, &gyro, &accel, &mag);
  // Serial.print(F("Calibration: "));
  // Serial.print(sys, DEC);
  // Serial.print(F(", "));
  // Serial.print(gyro, DEC);
  // Serial.print(F(", "));
  // Serial.print(accel, DEC);
  // Serial.print(F(", "));
  // Serial.print(mag, DEC);
  // Serial.println(F(""));



  // delay(BNO055_SAMPLERATE_DELAY_MS);
}
