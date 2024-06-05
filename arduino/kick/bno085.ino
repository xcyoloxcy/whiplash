#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO08x.h>

// For I2C mode
Adafruit_BNO08x bno08x(-1);
sh2_SensorValue_t sensorValue;

const int motor_pin_1 = 23;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("WebSerial 3D Firmware");

  // Try to initialize!
  if (!bno08x.begin_I2C()) {
    Serial.println("Failed to find BNO08x chip");
    while (1) { delay(10); }
  }
  Serial.println("BNO08x Found!");

  for (int n = 0; n < bno08x.prodIds.numEntries; n++) {
    Serial.print("Part ");
    Serial.print(bno08x.prodIds.entry[n].swPartNumber);
    Serial.print(": Version :");
    Serial.print(bno08x.prodIds.entry[n].swVersionMajor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[n].swVersionMinor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[n].swVersionPatch);
    Serial.print(" Build ");
    Serial.println(bno08x.prodIds.entry[n].swBuildNumber);
  }

  setReports();
  Serial.println("Reading events");
  delay(100);
}

void setReports(void) {
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_ROTATION_VECTOR)) {
    Serial.println("Could not enable rotation vector");
  }
}

void loop(void) {
  // delay(10);

  if (bno08x.wasReset()) {
    Serial.print("Sensor was reset ");
    setReports();
  }

  if (!bno08x.getSensorEvent(&sensorValue)) {
    return;
  }

  if (sensorValue.sensorId == SH2_ROTATION_VECTOR) {
    float qw = sensorValue.un.rotationVector.real;
    float qx = sensorValue.un.rotationVector.i;
    float qy = sensorValue.un.rotationVector.j;
    float qz = sensorValue.un.rotationVector.k;

    // Convert quaternion to Euler angles
    float yaw   = atan2(2.0 * (qx * qy + qw * qz), qw * qw + qx * qx - qy * qy - qz * qz);
    float pitch = -asin(2.0 * (qx * qz - qw * qy));
    float roll  = atan2(2.0 * (qy * qz + qw * qx), qw * qw - qx * qx - qy * qy + qz * qz);

    // Convert radians to degrees
    yaw = yaw * 180.0 / PI;
    pitch = pitch * 180.0 / PI;
    roll = roll * 180.0 / PI;

    // Serial.print("Yaw: ");
    // Serial.print(yaw);
    // Serial.print(" Pitch: ");
    // Serial.print(pitch);
    // Serial.print(" Roll: ");
    // Serial.println(roll);

    float y_angle = pitch;
    // Serial.print("-1000, ");
    Serial.println(int(y_angle));

    float k_d = 10;
    const float y_threshold = -20;
    if (y_angle < y_threshold) {
      float pwm = min(abs(y_angle - y_threshold) * k_d, 254);
      analogWrite(motor_pin_1, pwm);
    }
  }
}