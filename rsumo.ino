/*Pinagem do arduino*/

//Sensor Linha
int linhaDireita = 9; //Sensor linha direita  
int linhaEsquerda = 10; //Sensor linha esquerda

//Sensor Distancia
const int trigPin = 11; //Trigger pino 5
const int echoPin = 12; //Echo pino 4
const int delayLeitura = 50;
float distancia;
float duracao;

//motor_Esquerda
int IN1 = 2 ;
int IN2 = 4 ;
int velocidadeEsq = 3;

//motor_Direita
int IN3 = 6 ;
int IN4 = 7 ;
int velocidadeDir = 5;

//variavel auxiliar
int minVel = 180;
int velocidade = minVel;
int velocidadeGiro = 255;

//Inicializa Pinos
void setup(){
  //Sensor Linha
  pinMode(linhaDireita, INPUT);
  pinMode(linhaEsquerda, INPUT);

  //Sensor Distancia
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  

  //motor_Esquerda
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(velocidadeEsq,OUTPUT);

  //motor_Direita
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(velocidadeDir,OUTPUT);


  Serial.begin(9600);
}

void frenteEsq() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
}

void reEsq() {
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
}

void frenteDir() {
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void reDir() {
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void girarEsq() {
  moverPassoRe(200);
  reEsq();
  frenteDir();
  moverPasso(500);
}

void girarDir() {
  moverPassoRe(200);
  reDir();
  frenteEsq();
  moverPasso(500);
}

void moverPasso(int tempo) {
  analogWrite(velocidadeEsq,velocidadeGiro);
  analogWrite(velocidadeDir,velocidadeGiro);
  delay(tempo);
  analogWrite(velocidadeEsq,0);
  analogWrite(velocidadeDir,0);
  delay(tempo);
}

void moverPassoRe(int tempo) {
  reDir();
  reEsq();
  analogWrite(velocidadeEsq,velocidadeGiro);
  analogWrite(velocidadeDir,velocidadeGiro);
  delay(tempo);
  analogWrite(velocidadeEsq,0);
  analogWrite(velocidadeDir,0);
  delay(tempo);
}

void mover() {
  analogWrite(velocidadeEsq,velocidade);
  analogWrite(velocidadeDir,velocidade);
}

double getDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duracao = pulseIn(echoPin, HIGH);
  //A função pulseIn espera o pino ir para HIGH,
  //inicia um temporizador, e para o temporizador
  //quando o pino voltar para LOW, retornando o
  //tempo em microsegundos.
  double distancia = duracao * 0.034 / 2;
  Serial.println(distancia);

  return distancia;
}

void corrigeTragetoriaLinha() {
  int ld = digitalRead(linhaDireita);
  int le = digitalRead(linhaEsquerda);
  
  if (ld && le) {
    return;
  }

  if (!ld) {
    girarEsq();
    return corrigeTragetoriaLinha();
  } else if (!le) {
    girarDir();
    return corrigeTragetoriaLinha();
  }
  
  // Serial.println(ld);
  // Serial.println(le);
}


void loop(){
  double d = getDistancia();
  int acabouDeVirar = 0;

  corrigeTragetoriaLinha();

  if (d <= 20) {
    girarDir();
    acabouDeVirar = 1;
  }
  else if (d <= 40) {
    velocidade = minVel;
  } else if (d <= 60) {
    velocidade = 200;
  } else {
    velocidade = 255;
  }
  if (acabouDeVirar) {
    acabouDeVirar = 0;
  } else {
    frenteDir();
    frenteEsq();
    mover();
  }
  delay(10);
  
}