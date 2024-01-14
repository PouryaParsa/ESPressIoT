//
// ESPressIoT Controller for Espresso Machines
//
// Helper Functions

String macToID(const uint8_t* mac) {
  String result;
  for (int i = 3; i < 6; ++i) {
    result += String(mac[i], HEX);
  }
  result.toUpperCase();
  return result;
}

String macToString(const uint8_t* mac) {
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], HEX);
    if (i < 5) result += ":";
  }
  result.toUpperCase();
  return result;
}

String statusAsJson() {
  StaticJsonDocument<256> statusObject;
  String outputString;

  statusObject["time"] = time_now;
  statusObject["measuredTemperature"] = gInputTemp;
  statusObject["targetTemperature"] = gTargetTemp;
  statusObject["heaterPower"] = gOutputPwr;
  statusObject["externalControlMode"] = externalControlMode;
  statusObject["externalButtonState"] = gButtonState;

  serializeJson(statusObject, outputString);
  return outputString;
}
