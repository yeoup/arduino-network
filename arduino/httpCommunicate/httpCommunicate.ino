#include <SPI.h>
#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>

// This example downloads the URL "http://arduino.cc/"

// Name of the server we want to connect to
const char kHostname[] = "arduino.cc";
// Path to download (this is the bit after the hostname in the URL
// that you want to download
const char kPath[] = "/";

byte mac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

void setup()
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 

  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }  
}

void loop()
{
  int err =0;
  
  EthernetClient c;
  HttpClient http(c);
  
  err = http.get(kHostname, kPath);
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout) )
        {
            if (http.available())
            {
                c = http.read();
                // Print out this character
                Serial.print(c);
               
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to
                // arrive
                delay(kNetworkDelay);
            }
        }
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();

  // And just stop, now that we've tried a download
  while(1);
}

// #include <SPI.h>
// #include <HttpClient.h>
// #include <Ethernet.h>
// #include <EthernetClient.h>
//
// const char kHostname[] = "http://localhost:3000"; //"[서버 IP 주소]"; // 서버 IP 주소
// const char kPath[] = "/"; //"[API 경로]";         // API 경로
//
// const int kNetworkTimeout = 15 * 1000; // 데이터 수신 대기 임계 시간
// const int kNetworkDelay = 1000; // 사용 가능한 데이터가 없는 경우 다시 시도하기 전에 대기할 시간
//
//// HTTP 통신 함수
// void connection()
//{
//   int result = 0;
//
//   EthernetClient c;
//   HttpClient http(c);
//
//   result = http.get(kHostname, kPath); // HTTP 연결 시도
//   if (result == 0) // 서버 연결 성공
//   {
//     Serial.println("startedRequest ok");
//
//     result = http.responseStatusCode(); // HTTP 상태 코드
//     if (result >= 0) // 데이터 수신
//     {
//       Serial.print("Got status code: ");
//       Serial.println(result);
//
//       // Usually you'd check that the response code is 200 or a
//       // similar "success" code (200-299) before carrying on,
//       // but we'll print out whatever response we get
//
//       result = http.skipResponseHeaders();
//       if (result >= 0) //
//       {
//         int bodyLen = http.contentLength();
//         Serial.print("Content length is: ");
//         Serial.println(bodyLen);
//         Serial.println();
//         Serial.println("Body returned follows:");
//
//         // Now we've got to the body, so we can print it out
//         unsigned long timeoutStart = millis();
//         char c;
//         // Whilst we haven't timed out & haven't reached the end of the body
//         while ((http.connected() || http.available()) &&
//                ((millis() - timeoutStart) < kNetworkTimeout))
//         {
//           if (http.available())
//           {
//             c = http.read();
//             // Print out this character
//             Serial.print(c);
//
//             bodyLen--;
//             // We read something, reset the timeout counter
//             timeoutStart = millis();
//           }
//           else
//           {
//             // We haven't got any data, so let's pause to allow some to
//             // arrive
//             delay(kNetworkDelay);
//           }
//         }
//       }
//       else
//       {
//         Serial.print("Failed to skip response headers: ");
//         Serial.println(result);
//       }
//     }
//     else // 데이터 수신 실패
//     {
//       Serial.print("Getting response failed: ");
//       Serial.println(result);
//     }
//   }
//   else // 서버 연결 실패
//   {
//     Serial.print("Connect failed: ");
//     Serial.println(result);
//   }
//
//   http.stop(); // HTTP 연결 종료
// }
