//Componentes: Ponte H L298D e sensor InfraRed
//Programador: Evandro Cantu
 
//Veriáveis para sensores de linha e velocidade
int     sensorEsq, sensorLinha, sensorDir;
int     velMin = 120, velMed = 140, velMax = 230; //Velocidades dos motores
int     limiar = 800; //limiar do sensor de luz

//Variáveis para sensores de início e fim de pista
int     sensorFim;
int     contFim = 4; // 4; //Conta marcas de início, fim e cruzamentos
boolean flagFim = false; 
long    tempoTotal; //Tempo total da volta
long    tempoExtra = 1000; //Tempo extra para seguir linha


//Definicoes pinos Arduino ligados a entrada da Ponte H
int IN1 = 2;
int IN2 = 4;
int IN3 = 7;
int IN4 = 8;
int  MA = 5; //Motor direito
int  MB = 6; //Motor esquerdo

void setup(){
  
  //Define os pinos como saida  
  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);  
  pinMode(IN3, OUTPUT);  
  pinMode(IN4, OUTPUT);
  pinMode(MA,  OUTPUT);  
  pinMode(MB,  OUTPUT);  
  
  //delay(2000);
}

void para_frente(int vel) {
  //Configura velocidade dos motores
  analogWrite(MA, vel);   
  analogWrite(MB, vel);   
  //Aciona o motores 
  digitalWrite(IN1, LOW);  //A 
  digitalWrite(IN2, HIGH); //A
  digitalWrite(IN3, LOW);  //B
  digitalWrite(IN4, HIGH); //B   
}

void vira_esquerda(int vel) {
  //MotorA_frente
  analogWrite(MA, vel);
  digitalWrite(IN1, LOW);  //A 
  digitalWrite(IN2, HIGH); //A  
  //MotorB_tras 
  analogWrite(MB, vel);   
  digitalWrite(IN3, HIGH); //B
  digitalWrite(IN4, LOW);  //B   
}

void vira_direita(int vel) {
  //MotorA_tras
  analogWrite(MA, vel);
  digitalWrite(IN1, HIGH);  //A 
  digitalWrite(IN2, LOW); //A  
  //MotorB_frente 
  analogWrite(MB, vel);   
  digitalWrite(IN3, LOW); //B
  digitalWrite(IN4, HIGH);  //B   
}

void para_motores() {
  //MotorA_para
  digitalWrite(IN1, HIGH);  //A 
  digitalWrite(IN2, HIGH);  //A  
  //MotorB_para 
  digitalWrite(IN3, HIGH);  //B
  digitalWrite(IN4, HIGH);  //B   
}

void segueLinha(){      

  //le os sensores
  sensorDir   = analogRead(2); 
  sensorLinha = analogRead(0);
  sensorEsq   = analogRead(1);
  sensorFim   = analogRead(3);

  //Se estiver na linha segue em frente 
  if (sensorLinha < limiar){
    para_frente(velMax);    
  }
 
  //Se sensorDir acha linha vire para a direita
  if (sensorDir < limiar && sensorLinha > limiar){
    vira_direita(velMed);
  }
 
  //Se sensorEsq acha linha vire para a esquerda
  if (sensorEsq < limiar && sensorLinha > limiar){
    vira_esquerda(velMed);
  }
  
}
 
void loop(){

  
  //Segue linha até encontrar marca de fim de pista
  while(contFim > 0){
    segueLinha();
    if (sensorFim < limiar)
       flagFim = true; 

    if (sensorFim > limiar && flagFim == true){
       flagFim=false;
       contFim--;
    }
  }

  //Segue linha por mais 2 segundos antes de parar
  tempoTotal = millis();
  while((millis() - tempoTotal) < tempoExtra)
    segueLinha();

  //Pára motores
  para_motores();

  //Espera 10s
  delay(30000);
 }