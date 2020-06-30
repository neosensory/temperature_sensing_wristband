/*
 * Copyright 2020 Neosensory, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Please note that while this Neosensory SDK has an Apache 2.0 license, 
 * usage of the Neosensory API to interface with Neosensory products is 
 * still  subject to the Neosensory developer terms of service located at:
 * https://neosensory.com/legal/dev-terms-service/
 * 
 * temperature_sensing_wristband.ino - Collects data from
 * multiple contactless temperature sensors and sends
 * that data to Buzz as vibrations.
 * 
 */

#include <neosensory_bluefruit.h>
#include <Wire.h>
#include <SparkFunMLX90614.h>
#include "sensors.h"

NeosensoryBluefruit NeoBluefruit;

#define TEMP_MOTOR_0 0
#define TEMP_MOTOR_1 3

IRTherm thermometer_0;
IRTherm thermometer_1;
long last_read_time;

SensorData thermometer_0_data(false, 6, 2, 10, RECORDING_SAMPLE_RATE, "Therm 0");
SensorData thermometer_1_data(false, 6, 2, 10, RECORDING_SAMPLE_RATE, "Therm 1");

bool do_print_buzz_messages = false;
bool do_print_sensor_data = false;
bool do_plot_data = true;

int num_frames_to_send;
float **motor_frames;

void setup() {
  if (!do_plot_data) Serial.begin(9600);
  NeoBluefruit.begin();
  NeoBluefruit.setConnectedCallback(onConnected);
  NeoBluefruit.setDisconnectedCallback(onDisconnected);
  NeoBluefruit.setReadNotifyCallback(onReadNotify);
  NeoBluefruit.startScan();
  setup_sensors();
  if (do_plot_data) setup_plot();
  initialize_motor_frames();
}

void loop () {
  bool has_new_data = read_sensors();
  if (!has_new_data) return;
  
  if (do_print_sensor_data) {
    thermometer_0_data.print_info();
    thermometer_1_data.print_info();
  }
  if (do_plot_data) plot_data();
  
  if (!(NeoBluefruit.isConnected() && NeoBluefruit.isAuthorized())) return;
  
  float** motor_frames = get_motor_frames(
    thermometer_0_data.get_activation(), thermometer_1_data.get_activation());
  NeoBluefruit.vibrateMotors(motor_frames, num_frames_to_send);
}

// Get the frames that we should send to the wristband, based on our data
float** get_motor_frames(
  float thermometer_0_activation, float thermometer_1_activation) {
  for (int i = 0; i < num_frames_to_send; i++) {
    for (int j = 0; j < NeoBluefruit.num_motors(); j++) {
      motor_frames[i][j] = 0;
    }
    motor_frames[i][TEMP_MOTOR_0] = 
      (i % 2 & thermometer_0_activation < 0) ? 0 : abs(thermometer_0_activation);
    motor_frames[i][TEMP_MOTOR_1] = 
      (i % 2 & thermometer_1_activation < 0) ? 0 : abs(thermometer_1_activation);
  }
  return motor_frames;
}

// Setup sensors so that data can be collected
void setup_sensors() {
  thermometer_0.begin();
  thermometer_0.setUnit(TEMP_F);
  thermometer_1.begin(0x5B);
  thermometer_1.setUnit(TEMP_F);
}

// Update the data in our sensor objects. Will do nothing if called before 
// RECORDING_DELAY_MS since the last update.
// Returns true if we received new data, false if we did nothing.
bool read_sensors() {
  if (millis() - last_read_time < RECORDING_DELAY_MS) {
    return false;
  }
  last_read_time = millis();

  if (!thermometer_0.read()) return false;
  float thermometer_0_reading = thermometer_0.object();
  thermometer_0_data.add_new_sensor_data(thermometer_0_reading);
  
  if (!thermometer_1.read()) return false;
  float thermometer_1_reading = thermometer_1.object();
  thermometer_1_data.add_new_sensor_data(thermometer_1_reading);

  return true;
}

// Create the pointers in memory necessary for holding the motor 
// frames we'll send to Buzz.
void initialize_motor_frames() {
  num_frames_to_send = RECORDING_DELAY_MS / NeoBluefruit.firmware_frame_duration();
  motor_frames = new float*[num_frames_to_send];
  for (int i = 0; i < num_frames_to_send; i++) {
    motor_frames[i] = new float[NeoBluefruit.num_motors()];
  }
}

/* Buzz Callbacks */

void onConnected(bool success) {
  if (!success) {
    if (do_print_buzz_messages) Serial.println("Attempted connection but failed.");
    return;
  }
  NeoBluefruit.authorizeDeveloper();
  NeoBluefruit.acceptTermsAndConditions();
  NeoBluefruit.stopAlgorithm();
  if (do_print_buzz_messages) Serial.println("Connected to Buzz!");
}

void onDisconnected(uint16_t conn_handle, uint8_t reason) {
  if (do_print_buzz_messages) Serial.println("\nDisconnected from Buzz.");
}

void onReadNotify(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  if (!do_print_buzz_messages) return;
  for (int i = 0; i < len; i++) {
    Serial.write(data[i]);
  }
}
