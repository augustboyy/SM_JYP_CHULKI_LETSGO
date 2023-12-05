/* 이 소스는 에듀이노(Eduino)에 의해서 번역, 수정, 작성되었고 소유권 또한 에듀이노의 것입니다.
    소유권자의 허락을 받지 않고 무단으로 수정, 삭제하여 배포할 시 법적인 처벌을 받을 수도 있습니다.

    에듀이노 교육용 2휠 아두이노 스마트카 구동예제
    - 초음파센서를 활용한 스마트카 자율주행-

    초음파 센서로 거리를 감지하여 전방에 장애물이 인식되면, 좌우로 회피하여
    자율주행이 가능한 소스입니다.
    방향은 Random() 함수를 이용해 무작위로 선정됩니다.

*/

#include <SoftwareSerial.h>
#include <AFMotor.h>
AF_DCMotor motor_L(1);            
AF_DCMotor motor_R(4); 

#define BT_RXD A5
#define BT_TXD A4
SoftwareSerial bluetooth(BT_RXD, BT_TXD);      

char rec_data;
bool rec_chk = false;

int i;
int j;

int Lspeed=200;
int Rspeed=170;

//초음파센서 출력핀(trig)과 입력핀(echo) 설정
int trigPin = A0;
int echoPin = A1;
long duration, distance;

void setup(){
  Serial.begin(9600);              // PC와의 시리얼 통신속도
  bluetooth.begin(9600);            // 스마트폰 블루투스 통신속도
  Serial.println("Eduino Smart Car Start!");

  pinMode(echoPin, INPUT);   // echoPin 입력
  pinMode(trigPin, OUTPUT);  // trigPin 출력

  // turn on motor
    Serial.begin(9600);                         // PC와의 시리얼 통신속도
  Serial.println("Eduino Smart Car Start!");

  motor_L.setSpeed(Lspeed);                   // 왼쪽 모터의 속도
  motor_L.run(RELEASE);
  motor_R.setSpeed(Rspeed);                   // 오른쪽 모터의 속도
  motor_R.run(RELEASE);
}

void Obstacle_Check();
void Distance_Measurement();
void Forward();
void Backward();
void Right();
void Left();
void Stop();

///////////모터 제어 함수////////////
void Forward() {
  motor_L.run(FORWARD);  motor_R.run(FORWARD);
  motor_L.setSpeed(Lspeed);  motor_R.setSpeed(Rspeed);
}
void Backward() {
  motor_L.run(BACKWARD);  motor_R.run(BACKWARD);
  motor_L.setSpeed(Lspeed);  motor_R.setSpeed(Rspeed);
}
void Right() {
  motor_L.run(FORWARD);  motor_R.run(BACKWARD);
  motor_L.setSpeed(Lspeed);  motor_R.setSpeed(Rspeed*0.5);
}
void Left() {
  motor_L.run(BACKWARD);  motor_R.run(FORWARD);
  motor_L.setSpeed(Lspeed*0.5);  motor_R.setSpeed(Rspeed);

}
void Stop() {
  motor_L.run(RELEASE);       motor_R.run(RELEASE);
  motor_L.setSpeed(0);  motor_R.setSpeed(0);
}

///////////장애물 확인 및 회피 방향 결정///////////
void Obstacle_Check() {
  Distance_Measurement();
  delay(50);
  
  Serial.println(distance);

  if (distance < 100) {
      Stop();
      delay(50);
    
  }
}

////////거리감지///////////
void Distance_Measurement() {
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);       // trigPin에서 초음파 발생(echoPin도 HIGH)
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  distance = ((float)(340 * duration) / 1000) / 2;
  delay(50);

}


void loop(){



  if(bluetooth.available()){         // 블루투스 명령 수신
     rec_data = bluetooth.read();
     Serial.write(rec_data);
     rec_chk = true;
  }  

  if(rec_data == 'g'){  // 전진, go
     motor_L.run(FORWARD);  motor_R.run(FORWARD);        
  } 
  else if(rec_data == 'b'){ // 후진, back
     motor_L.run(BACKWARD);  motor_R.run(BACKWARD);    
  }
  else if(rec_data == 'l'){ // 좌회전, Go Left
   motor_L.run(RELEASE);  motor_R.run(FORWARD);     
  }
  else if(rec_data == 'r'){ // 우회전, Go Right
    motor_L.run(FORWARD);  motor_R.run(RELEASE);                
  }
  else if(rec_data == 'q'){ // 제자리 회전, Left Rotation
     motor_L.run(BACKWARD);   motor_R.run(FORWARD);      
  }
  else if(rec_data == 'w'){ // 제자리 회전, Right Rotation
      motor_L.run(FORWARD);   motor_R.run(BACKWARD);    
  }
  else if(rec_data == 's'){ // Stop 
    motor_L.run(RELEASE);       motor_R.run(RELEASE);
    } 

    
  if(rec_data == 'f' ){       // 정지
    if(rec_chk == true){
       for (i=250; i>=0; i=i-20) {
          motor_L.setSpeed(i);  motor_R.setSpeed(i);  
          delay(10);
       }  
       
    }
  }

   Obstacle_Check();
     
}



