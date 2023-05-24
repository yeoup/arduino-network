#include <ArduinoHttpClient.h>

#include "SendDataModule.h"

void SendDataModule::send(HttpClient *client, int gas)
{
    Serial.println("Send data to server");
    Serial.print("Gas sensor value: "); Serial.print(gas);
    Serial.println();
    Serial.print("Photo");

    // Data 정의
    String contentType = "application/x-www-form-urlencoded";
    String postData = "name=Alice&age=12";
    String gasValue = "&gas=" + String(gas);
    postData += gas;

    client->post("/", contentType, postData);
}