// 빵판에 전기들어오는게 없어서 아두이노 5v랑 빵판의 +연결, 아두이노 gnd와 빵판의 -연결 했음
// 선 왓다갓다 바꿔끼웠음. 동작 동일함

#include <SoftwareSerial.h>  //아두이노 2개 연결하기 위한 라이브러리
#include <Servo.h>           //서보모터 라이브라리
#include <Adafruit_VC0706.h> //카메라 라이브러리 1
#include <Adafruit_OV7670.h> //카메라 라이브러리 2

// 카메라 객체 생성
Adafruit_OV7670 camera;

// 아두이노 2개 연결 핀
const int RxPin = 5;
const int TxPin = 4;

// RGB led 연결 핀
int ledRed = 9;   // red
int ledGreen = 8; // green

// 가스 센서 3개 각각의 연결 핀
int gas1 = A0; // 가스센서 1번 연결 핀
int gas2 = A1; // 가스센서 2번 연결 핀
int gas3 = A2; // 가스센서 3번 연결 핀

// 피에조 부저 연결 핀
int buzzer = 7;

// ??? 모터 연결 핀
int motor = 2;

// 서보 모터 변수 및 상태 변수 svState;
Servo sv;
int svState = 0;

// AC모터 변수 및 상태 변수 acState
int acState = 0;

// 안전상태에서 부저 1회만 울리게 하기 위해 상태변수사용
int isSafe = 1;

// delay()는 코드 전체를 멈추는 거라 delay(지정시간) 동안 다른 코드가 동작을 못함.
// 이를 해결하기 위해 (동시동작을 가능하게 하기 위해) millis()함수 사용
// millis()는 함수 실행때의 시간을 기록하는 함수임.  현재시간-과거시간=지정시간 으로 사용할 수 있음
//=> 과거시간을 업데이트하면서 delay()처럼 지정시간 이후 동작을 지정할 수 있음.
unsigned long time_previous = 0, time_current = 0;

void setup()
{
  Serial.begin(9600);      // 시리얼 통신 초기화.
  pinMode(buzzer, OUTPUT); // 피에조는 출력센서.
  pinMode(gas1, INPUT);    // 가스 1,2,3은 입력센서.
  pinMode(gas2, INPUT);
  pinMode(gas3, INPUT);
  pinMode(ledRed, OUTPUT); // led는 출력센서.
  pinMode(ledGreen, OUTPUT);
  time_previous = millis(); // 현재시간을 이전 시간으로 등록.

  // 카메라 모듈 초기화
  if (!camera.begin(OV7670_CAM))
  {
    Serial.println(F("카메라 모듈을 찾을 수 없습니다. 연결을 확인하세요."));
    while (1)
      ;
  }
  // 카메라 모듈 해상도 설정 (QQVGA, QVGA, VGA 등)
  camera.setResolution(OV7670_320x240); // 원하는 해상도로 변경 가능
}

void loop()
{
  // 버튼 또는 다른 트리거 이벤트를 기반으로 사진을 찍을 수 있습니다.
  // 사진 촬영
  camera.takePicture();

  // 사진 데이터 가져오기
  uint8_t *buffer;
  uint8_t length = camera.readPicture(buffer);

  // 사진 데이터를 원하는 형식으로 사용하거나 저장할 수 있습니다.
  // 사진 데이터를 시리얼 모니터에 출력
  for (uint8_t i = 0; i < length; i++)
  {
    Serial.write(buffer[i]);
  }
  // 메모리 해제
  camera.freePicture();

  delay(1000); // 사진 찍기 간격을 조절할 수 있습니다.

  // 가스센서 항시 감지 (시리얼 모니터 에서 확인)
  Serial.print("gas1 : ");
  Serial.print(analogRead(gas1)); // 디폴트 : 약 50~60, 입김불면 90~95
  Serial.print(", gas2 : ");      // 디폴트 : 약 40~50, 입김불면 80~85
  Serial.print(analogRead(gas2)); // 디폴트 : 약 55~65, 입김불면 90~95
  Serial.print(", gas3 : ");
  Serial.println(analogRead(gas3));

  // 루프 돌 떄마다 현재시간 갱신
  time_current = millis();

  // 센서값 탐지 -> 센서 3개중 한개라도 100이상이면 if문 내부 동작
  if (analogRead(gas1) > 120 || analogRead(gas2) > 120 || analogRead(gas3) > 120)
  {                               // 기준치 100으로 설정(적당히 바꾸세요)
    time_previous = time_current; // 현재시간 갱신
    if (time_current - time_previous >= 100)
    { // 1000=1초임.
      svState++;
    }
  }
  else
  {
    time_previous = time_current;
    if (time_current - time_previous >= 100)
    {
      svState--;
    }
    if (svState < 0)
    {
      svState = 0;
    }
  }

  // svState에 따라 작동 제어
  if (svState == 1)
  {
    Serial.println("현재 안전합니다.");

    tone(buzzer, 30, 10000);
    // //피에조 작동 1회
    // if(isSafe==1){
    //   tone(buzzer,32.7082,10000);
    //   time_previous = time_current;
    //   if(time_current - time_previous >= 1000){
    //     isSafe=0;
    //     noTone(buzzer);
    //   }
    // }
  }

  // 카메라촬영
  // 사진 이미지 넘기기
  // 센서값 넘기기

  // 모터 작동
  digitalWrite(motor, 255);

  // 녹색 점등, 적색 소등
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledRed, LOW);

  sv.write(180); // 서보모터
  if (time_current - time_previous >= 1000)
  {
    sv.detach(); // 서보모터 1초 동작 후 멈춤
    time_previous = time_current;
  }

  else if (svState >= 3)
  {
    Serial.println("경고!!! 실내 흡연자 발생!!");
    // 피에조 작동
    // tone(buzzer,7000,1500);
    noTone(buzzer);
    // 카메라촬영
    // 사진 이미지 넘기기
    // 센서값 넘기기

    // 적색 점등, 녹색 소등
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);

    // 서보모터
    sv.write(0);
    if (time_current - time_previous >= 1000)
    {
      sv.detach(); // 서보모터 1초 동작 후 멈춤
      time_previous = time_current;
    }
  }
}
