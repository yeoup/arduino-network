#include <SPI.h>
#include <Ethernet.h>

// MAC address setting
// Ethernet shield에 명시된 MAC address를 입력
byte mac[] = {
    0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 // MAC address (example)
};

void setup()
{
  Serial.begin(9600); // 시리얼 통신 초기화

  while (!Serial) // 시리얼 포트 연결확인
  {
    ;
  }

  // 이더넷 연결 시작
  Serial.println("Initialize Ethernet with DHCP:");

  if (Ethernet.begin(mac) == 0) // 연결 실패
  {
    Serial.println("Failed to configure Ethernet using DHCP");

    // 예외처리
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
      Serial.println("Ethernet shield was not found. Sorry, can't run without hardware. :(");
    }
    else if (Ethernet.linkStatus() == LinkOFF)
    {
      Serial.println("Ethernet cable is not connected.");
    }

    // 아두이노 정지
    while (true)
    {
      delay(1);
    }
  }

  // 할당받은 IP 주소 출력
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop()
{
  // IP 주소 변동 체크
  switch (Ethernet.maintain())
  {
  case 1: // renewed fail
    Serial.println("Error: renewed fail");
    break;
  case 2: // renewed success
    Serial.println("Renewed success");
    // IP 주소 출력
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
    break;
  case 3: // rebind fail
    Serial.println("Error: rebind fail");
    break;
  case 4: // rebind success
    Serial.println("Rebind success");
    // IP 주소 출력
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
    break;
  default: // nothing happened
    break;
  }
}
