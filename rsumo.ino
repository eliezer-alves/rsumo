/*Pinagem do arduino*/

int iniciar = 1;
int DELAY_INICIO = 3000;

int explorar = 0;
int procurarInimigo = 1;
int procurou = 0;
int direcao = 0;

int SUAVIZAR_MUDANCA_MOVIMEMTO = 1;
int DELAY_SUAVIZACAO = 100;


//Sensor Linha
int TIPO_SENSOR_LINHA = 1; // 1 = analógico, 2 = digital
int SENSIBILIDADE_LINHA = 40; // valor abixo desse é consididerado limite do ringue -> geralmente 20 a 30 branco 100 a 900 preto;
int CORRECAO_TRAJETORIA = 700;
int mudar_correcao = 0;


int linhaDireitaAnlg = A0;
int linhaEsquerdaAnlg = A1;
int linhaDireita = 8; //Sensor linha direita  
int linhaEsquerda = 9; //Sensor linha esquerda



double distanciaMaximaInimigo = 70;
//Sensor Distancia
const int trigPin = 11; //Trigger pino 5
const int echoPin = 12; //Echo pino 4
const int delayLeitura = 50;
double duracao;

//Sensor Distancia2
const int trigPin2 = 10; //Trigger pino 5
const int echoPin2 = 13; //Echo pino 4
double duracao2;

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
int velocidade = 255;
int velocidadeGiro = 255;


//Inicializa Pinos
void setup(){
  //Sensor Linha
  pinMode(linhaEsquerdaAnlg, INPUT);
  pinMode(linhaDireitaAnlg, INPUT);
  pinMode(linhaDireita, INPUT);
  pinMode(linhaEsquerda, INPUT);

  //Sensor Distancia
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  

  //Sensor Distancia2
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);  

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

void girarEsq(int tempo = 500, int recuo = 1) {
  if (recuo) {  
    moverPassoRe(200);
  }
  reEsq();
  frenteDir();
  moverPasso(tempo);
}

void girarDir(int tempo = 500, int recuo = 1) {
  if (recuo) {
    moverPassoRe(200);
  }
  reDir();
  frenteEsq();
  moverPasso(tempo);
}

void moverPasso(int tempo) {
  analogWrite(velocidadeEsq,velocidadeGiro);
  analogWrite(velocidadeDir,velocidadeGiro);
  delay(tempo);
  if (SUAVIZAR_MUDANCA_MOVIMEMTO) {
    analogWrite(velocidadeEsq,0);
    analogWrite(velocidadeDir,0);
    delay(DELAY_SUAVIZACAO);
  }
}

void moverPassoRe(int tempo) {
  reDir();
  reEsq();
  analogWrite(velocidadeEsq,velocidadeGiro);
  analogWrite(velocidadeDir,velocidadeGiro);
  delay(tempo);
  if (SUAVIZAR_MUDANCA_MOVIMEMTO) {
    analogWrite(velocidadeEsq,0);
    analogWrite(velocidadeDir,0);
    delay(DELAY_SUAVIZACAO);
  }
}

void mover(int _delay=0) {
  analogWrite(velocidadeEsq,velocidade);
  analogWrite(velocidadeDir,velocidade);
  delay(_delay);
}

void moverFrente(int _delay=0) {
  frenteEsq();
  frenteDir();
  mover(_delay);
}



void corrigeTragetoriaDigital() {
  int ld = digitalRead(linhaDireita);
  int le = digitalRead(linhaEsquerda);
  
  if (ld && le) {
    return;
  }

  if (!ld) {
    girarEsq(CORRECAO_TRAJETORIA);
    return corrigeTragetoriaDigital();
  } else if (!le) {
    girarDir(CORRECAO_TRAJETORIA);
    return corrigeTragetoriaDigital();
  }  
}

void corrigeTragetoriaAnalogico(int teveErro=0) {
  int ld = analogRead(linhaDireitaAnlg);
  int le = analogRead(linhaEsquerdaAnlg);

  if (ld < SENSIBILIDADE_LINHA) { // geralmente 20 a 30 branco 100 a 900 preto;
    explorar = 0;
    girarEsq(CORRECAO_TRAJETORIA);
    return corrigeTragetoriaAnalogico(1);
  } else if (le < SENSIBILIDADE_LINHA) { // geralmente 20 a 30 branco 100 a 900 preto;
    explorar = 0;
    girarDir(CORRECAO_TRAJETORIA);
    return corrigeTragetoriaAnalogico(1);
  } else if (teveErro) {
    mover(2000);
  }
}

void corrigeTragetoria() {
  if (TIPO_SENSOR_LINHA) {
    corrigeTragetoriaAnalogico();
  } else {
    corrigeTragetoriaDigital();
  }
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
  //Serial.println(distancia);

  return distancia;
}
double getDistancia2() {
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duracao2 = pulseIn(echoPin2, HIGH);
  //A função pulseIn espera o pino ir para HIGH,
  //inicia um temporizador, e para o temporizador
  //quando o pino voltar para LOW, retornando o
  //tempo em microsegundos.
  double distancia = duracao2 * 0.034 / 2;
  //Serial.println(distancia);

  return distancia;
}

void mudarDirecao() {
  if (direcao) {
    direcao = 0;
  } else {
    direcao = 1;
  }
}

void _iniciar() {
  frenteDir();
  frenteEsq();
  delay(DELAY_INICIO);
  mover();
}

void procuraInimigo() {
    if(direcao) {
      girarDir(100, 0);      
    } else {
      girarEsq(100, 0);
    }
    if (procurou > 5) {
      procurou = 0;
      explorar = 1;
    }
    procurou+=1;
    delay(100);
}

void loop(){

  if (iniciar) {
    iniciar = 0;
    _iniciar();
  }

  double distancia = getDistancia();
  double distancia2 = getDistancia2();
  // Serial.println(distancia);
  // Serial.println(distancia2);
  // Serial.println("----------");
  // // corrigeTragetoriaLinha();
  corrigeTragetoria();

  // //Serial.println(distancia);

  if (distancia < distanciaMaximaInimigo || distancia2 < distanciaMaximaInimigo || explorar) {
    mudarDirecao();
    procurou = 0;
    velocidade = 255;
    moverFrente();
  } else {    
    procuraInimigo();
  }
  // delay(400);
  
}