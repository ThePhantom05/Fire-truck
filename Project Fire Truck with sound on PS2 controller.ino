/*                                  CANAL ARDUINO PARA MODELISMO
                               ******************************************
                              https://www.youtube.com/ArduinoParaModelismo

 *****************************CODIGO FONTE DO RECEPTOR CONTROLE PS2 WIFI****************************


  Desenvolvido por Aldeir de Souza Moreira - aldeirsm@gmail.com / https://arduinoparamodelismo.com
  C 2022 - Todos os direitos reservados
  Código feito exclusivamente para o Canal de Arduino para Modelismo:   https://www.youtube.com/ArduinoParaModelismo
 *** Siga atentamente o esquema Elétrico montagem do hardware
*/

// https://arduinoparamodelismo.com
//🔵Pare de quebrar a cabeça e realize seu sonho de infância! Entenda de uma vez por todas,
//como dar vida a seus projetos e controlar tudo via controle remoto e com baixo custo, usando arduino!
//👉Acesse: https://arduinoparamodelismo.com


// Importando Bibliotecas
#include <PS2X_lib.h> // Biblioteca com as funções para controle de controle de PSII
#include <Servo.h>  // Incluindo biblioteca Servo

#include "DFRobotDFPlayerMini.h" // Biblioteca para o Módulo MP3

DFRobotDFPlayerMini myDFPlayer;

PS2X ps2x; //Instância um objeto PS2X para comunicação com o controle

Servo servoDirecao; // Criando Objeto controle do Servo Motor de direção
Servo servoGiraEscada; // Criando Objeto para controle do Servo Motor que gira a escada
Servo servoErgueEscada; // Criando Objeto para controle do Servo Motor que ergue a escada

int erro = 0; // Variável para reportar erro de leitura do controle de PSII

uint32_t Temp_PiscaAlerta, Temp_SetaDireita, Temp_SetaEsquerda, Millis_Giroflex; // Variáveis necessárias para contagem de tempo em milis segundos das setas e pisca-alerta


// Variáveis Globais
 
boolean Start = 0;
boolean Select = 0;
boolean SetaCima = 0;  
boolean SetaDireita = 0; 
boolean SetaEsquerda = 0; 
boolean SetaBaixo = 0;  
boolean BotaoR1 = 0;
boolean BotaoR2 = 0;
boolean BotaoR3 = 0;
boolean BotaoL1 = 0;
boolean BotaoL2 = 0;
boolean BotaoL3 = 0;
boolean BotaoTriangulo = 0;
boolean BotaoCirculo = 0;
boolean BotaoQuadrado = 0;
boolean BotaoX = 0;
byte JoyDireitaX = 127;
byte JoyDireitaY = 127;
byte JoyEsquerdaX = 127;
byte JoyEsquerdaY = 127;

int ServoGiraAux;
int ServoErgueAux;
int Giroflex = 0;
int Sirene = 0;
int LigaDesliga = 0;
int cont = 0;

void setup() {

// ATENÇÃO, TODOS OS PRINTS ESTÃO COMENTADOS PARA UM PERFEITO FUNCIONAMENTO DO MÓDULO MP3 NAS PORTAS DIGITAIS 0 e 1 (SERIAL VIA HARDWARE DO ARDUINO) 
  Serial.begin(9600);// Inicia comunicação serial

  erro = ps2x.config_gamepad(8, 11, 12, 13, true, true); //Configura o controle para os pinos informados(clock, command, attention, data, Pressures?, Rumble?) e verifica se há algum erro

  if (erro == 0) {
    //Serial.println("CONTROLE CONECTADO AO ARDUINO COM SUCESSO!");
  } else {
    //Serial.println("ERRO! CONTROLE NÃO CONECTADO! VERIFIQUE AS CONEXÕES");
  }

  //Serial.println("PROJETO IVECO TECTOR CORPO DE BOMBEIROS - CANAL ARDUINO PARA MODELISMO - PSII WIFI ***");


  servoDirecao.attach(A0);      // servo motor de direção conectado ao pino A0 do Arduino
  servoGiraEscada.attach(A1);   // servo motor que gira a base da escadas conectado ao pino A1 do Arduino
  servoErgueEscada.attach(A2);  // servo motor que ergue e abaixa a escada conectado ao pino A2 do Arduino

  pinMode(2, OUTPUT);  // Leds - Farol
  pinMode(3, OUTPUT);  // Leds - Lanterna/Luz Freio
  pinMode(4, OUTPUT);  // Luz de Ré
  pinMode(5, OUTPUT);  // Ponte H
  pinMode(6, OUTPUT);  // Ponte H
  pinMode(7, OUTPUT);  // Aciona Ponte H Bomba
  pinMode(9, OUTPUT);  // Seta Direita
  pinMode(10, OUTPUT); // Seta Esquerda
  pinMode(A3, OUTPUT); // GiroFlex
  pinMode(A4, OUTPUT); // GiroFlex
  pinMode(A5, OUTPUT); // Farol de Milha


  if (!myDFPlayer.begin(Serial)) {  //Use softwareSerial to communicate with mp3.
    //Serial.println(F("Unable to begin:"));
    //Serial.println(F("1.Please recheck the connection!"));
    //Serial.println(F("2.Please insert the SD card!"));
    //while (true);
  }
  //Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

  //----Set volume----
  myDFPlayer.volume(25);  //Set volume value (0~30).
  //myDFPlayer.volumeUp(); //Volume Up
  //myDFPlayer.volumeDown(); //Volume Down

  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //  myDFPlayer.EQ(DFPLAYER_EQ_POP);
  //  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
  //  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
  //  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
  //  myDFPlayer.EQ(DFPLAYER_EQ_BASS);


// Ajusta posição inicial da escada
  ServoErgueAux = 175;
  servoErgueEscada.write(ServoErgueAux);

  ServoGiraAux = 80;
  servoGiraEscada.write(ServoGiraAux);
}

void loop() {

  ps2x.read_gamepad(false, 0);    //lê as informações enviadas pelo Controle PSII

  // NÃO ESTA SENDO USADO
  if (ps2x.ButtonPressed(PSB_START)) {            // BOTÃO START
    Start = ! Start; // a variável vai receber o valor contrário ao que ela continha - As variávei boolean só tem dois valores, 0 ou 1, sendo assim, se a variável continha o valor 0 pasará a conter o valor 1 e vise-versa sempre que o botão for precionado

  LigaDesliga ++;
  if (LigaDesliga > 2) {
    LigaDesliga = 1;
  }
  //Serial.print("Start: ");
  //Serial.println( Start);

  if (LigaDesliga == 1) {
    myDFPlayer.play(6);  //Play the first mp3
    delay(100);
    //Serial.println( "Som 05");
  }
  if (LigaDesliga == 2) {
    myDFPlayer.play(5);  //Play the first mp3
    delay(100);
    //Serial.println( "Som 06");
  }
}


// NÃO ESTA SENDO USADO
if (ps2x.ButtonPressed(PSB_SELECT)) {         // BOTÃO SELECT
  Select = ! Select;
  //Serial.print("Select: ");
  //Serial.println( Select);
myDFPlayer.volumeUp(); //Volume Up
}

// USADO PARA PISCA-ALERTA
if (ps2x.ButtonPressed(PSB_PAD_UP)) {        // BOTÃO SETA PARA CIMA
  SetaCima = ! SetaCima;
  //Serial.print("Seta Cima: ");
  //Serial.println( SetaCima);

  digitalWrite(9, LOW);  // Apaga Seta Direita
  digitalWrite(10, LOW);  // Apaga Seta Esquerda
}

if (SetaCima == HIGH) {
  SetaDireita = LOW;  // Desabilita Seta Direita ao acionar Pisca-Alerta
  SetaEsquerda = LOW;  // Desabilita Seta Esquerda ao acionar Pisca-Alerta
  if (millis() -  Temp_PiscaAlerta > 500) { // função millis no if faz acontagem de tempo que a seta deve ficar acesa, nestecaso 500 milissegundos (Acende/Apaga led a cada meio segundo)
    digitalWrite(9, !digitalRead(9)); // Ativa e desativa a porta digital 9 de acordo com a leitura do estado da própria porta (se estiver acesa, apaga. Se estiver apagada acende)
    digitalWrite(10, !digitalRead(10)); // Ativa e desativa a porta digital 10 de acordo com a leitura do estado da própria porta (se estiver acesa, apaga. Se estiver apagada acende)
    Temp_PiscaAlerta = millis();
  }
}


// USADO PARA SETA PARA DIREITA
if (ps2x.ButtonPressed(PSB_PAD_RIGHT)) {     // BOTÃO SETA PARA DIREITA
  SetaDireita = ! SetaDireita;
  //Serial.print("Seta Direita: ");
  //Serial.println( SetaDireita);
  digitalWrite(9, LOW);  // Apaga Seta Direita
  digitalWrite(10, LOW);  // Apaga Seta Esquerda
}

if (SetaDireita == HIGH) {
  SetaEsquerda = LOW;  // Desabilita Seta Esquerda ao acionar a Seta Direita
  if (millis() -  Temp_SetaDireita > 500) {    // função millis no if faz acontagem de tempo que a seta deve ficar acesa (Acende/Apaga led a cada meio segundo)
    digitalWrite(9, !digitalRead(9));    // Ativa e desativa a porta digital 9 de acordo com a leitura do estado da própria porta (se estiver acesa, apaga. Se estiver apagada acende)
    Temp_SetaDireita = millis();
  }
}



// USADO PARA SETA ESQUERDA
if (ps2x.ButtonPressed(PSB_PAD_LEFT)) {     // BOTÃO SETA PARA ESQUERDA
  SetaEsquerda = ! SetaEsquerda;
  //Serial.print("Seta Esquerda: ");
  //Serial.println( SetaEsquerda);
  digitalWrite(9, LOW);  // Apaga Seta Direita
  digitalWrite(10, LOW);  // Apaga Seta Esquerda
}

if (SetaEsquerda == HIGH) {
  SetaDireita = LOW;                             // Desabilita Seta Direita ao acionar a Seta Esquerda
  if (millis() -  Temp_SetaEsquerda > 500) {   // função millis no if faz acontagem de tempo que a seta deve ficar acesa, nestecaso 500 milissegundos (Acende/Apaga led a cada meio segundo)
    digitalWrite(10, !digitalRead(10));    // Ativa e desativa a porta digital 10 de acordo com a leitura do estado da própria porta (se estiver acesa, apaga. Se estiver apagada acende)
    Temp_SetaEsquerda = millis();
  }
}



// USADO PARA O FAROL DE MILHA
if (ps2x.ButtonPressed(PSB_PAD_DOWN)) {    // BOTÃO SETA PARA BAIXO
  SetaBaixo = ! SetaBaixo;
  //Serial.print("Seta Baixo: ");
  //Serial.println( SetaBaixo);
  digitalWrite(A5, LOW);
}
if (SetaBaixo == HIGH) {
  digitalWrite(A5, HIGH);
}



// USADO PARA LEVANTAR E ABAIXAR A ESCASA DO CAMINHÃO
if (ps2x.Button(PSB_R2)) {          // BOTÃO R2
  ServoErgueAux++;
  if (ServoErgueAux > 180) {
    ServoErgueAux = 180;
  }
  servoErgueEscada.write(ServoErgueAux);
  delay(10);
  //Serial.print("Botão R1 pressionado: ");
  //Serial.println(ServoErgueAux);
}

// USADO PARA LEVANTAR E ABAIXAR A ESCASA DO CAMINHÃO
if (ps2x.Button(PSB_R1)) {          // BOTÃ R1
  ServoErgueAux--;
  if (ServoErgueAux < 1) {
    ServoErgueAux = 0;
  }
  servoErgueEscada.write(ServoErgueAux);
  delay(10);
  //Serial.print("Botão R2 pressionado: ");
  //Serial.println(ServoErgueAux);
}



// NÃO ESTA SENDO USADO
if (ps2x.Button(PSB_R3)) {          // R3 BOTÃO DO JOYSTICK ANALÓGICO DIREITO
  BotaoR3 = ! BotaoR3;
  //Serial.print("BotaoR3: ");
  //Serial.println( BotaoR3);
  digitalWrite(7, HIGH); // Desativa Motor da Bomba d'água!
} else {
  digitalWrite(7, LOW); // Ativa Motor da Bomba d'água!
}



// USADO PARA GIRAR A BASE DA ESCASA DO CAMINHÃO
if (ps2x.Button(PSB_L1)) {          // BOTÃO L1
  ServoGiraAux++;
  if (ServoGiraAux > 180) {
    ServoGiraAux = 180;
  }
  servoGiraEscada.write(ServoGiraAux);
  //delay(10);
  //Serial.print("Botão L1 pressionado: ");
  //Serial.println(ServoGiraAux);
}


// USADO PARA GIRAR A BASE DA ESCASA DO CAMINHÃO
if (ps2x.Button(PSB_L2)) {         // BOTÃO L2
  ServoGiraAux--;
  if (ServoGiraAux < 1) {
    ServoGiraAux = 0;
  }
  servoGiraEscada.write(ServoGiraAux);
  //delay(10);
  //Serial.print("Botão L2 pressionado: ");
  //Serial.println(ServoGiraAux);
}



// NÃO ESTA SENDO USADO
if (ps2x.ButtonPressed(PSB_L3)) {         // BOTÃO L3 (BOTÃO DO JOYSTICK ANALÓGICO ESQUERDO)
  BotaoL3 = ! BotaoL3;
  //Serial.print("BotaoL3: ");
  //Serial.println( BotaoL3);

  myDFPlayer.volumeDown(); //Volume Down
}


// USADO PARA GIROFLEX
if (ps2x.ButtonPressed(PSB_GREEN)) {              // BOTÃO TRIANGULO
  BotaoTriangulo = ! BotaoTriangulo;

  Giroflex++;
  if (Giroflex > 3) {
    Giroflex = 0;
  }
  Millis_Giroflex = millis();
  delay(30);


  //Serial.print("Botao Triangulo: ");
  //Serial.println( Giroflex);

  if (Giroflex == 0) {
    digitalWrite(A5, LOW);
    digitalWrite(4, LOW);
  }

}

if (BotaoTriangulo == HIGH) {
}

if (Giroflex == 1) {
  if ((millis() -  Millis_Giroflex > 100) && (millis() -  Millis_Giroflex < 200)) {
    digitalWrite(A3, LOW);
    digitalWrite(A4, HIGH);

    digitalWrite(A5, HIGH);
    digitalWrite(4, LOW);
  }
  if ((millis() -  Millis_Giroflex > 200) && (millis() -  Millis_Giroflex < 300)) {
    digitalWrite(A3, LOW);
    digitalWrite(A4, LOW);

    digitalWrite(A5, LOW);
    digitalWrite(4, HIGH);
  }
  if ((millis() -  Millis_Giroflex > 300) && (millis() -  Millis_Giroflex < 400)) {
    digitalWrite(A3, LOW);
    digitalWrite(A4, HIGH);

    digitalWrite(A5, HIGH);
    digitalWrite(4, LOW);
  }
  if ((millis() -  Millis_Giroflex > 400) && (millis() -  Millis_Giroflex < 500)) {
    digitalWrite(A3, LOW);
    digitalWrite(A4, LOW);

    digitalWrite(A5, LOW);
    digitalWrite(4, HIGH);
  }
  if ((millis() -  Millis_Giroflex > 500) && (millis() -  Millis_Giroflex < 600)) {
    digitalWrite(A3, LOW);
    digitalWrite(A4, HIGH);

    digitalWrite(A5, HIGH);
    digitalWrite(4, LOW);
  }
  if ((millis() -  Millis_Giroflex > 600) && (millis() -  Millis_Giroflex < 700)) {
    digitalWrite(A4, LOW);
    digitalWrite(A3, HIGH);

    digitalWrite(A5, LOW);
    digitalWrite(4, HIGH);
  }
  if ((millis() -  Millis_Giroflex > 700) && (millis() -  Millis_Giroflex < 800)) {
    digitalWrite(A4, LOW);
    digitalWrite(A3, LOW);

    digitalWrite(A5, HIGH);
    digitalWrite(4, LOW);
  }
  if ((millis() -  Millis_Giroflex > 800) && (millis() -  Millis_Giroflex < 900)) {
    digitalWrite(A4, LOW);
    digitalWrite(A3, HIGH);

    digitalWrite(A5, LOW);
    digitalWrite(4, HIGH);
  }
  if ((millis() -  Millis_Giroflex > 900) && (millis() -  Millis_Giroflex < 1000)) {
    digitalWrite(A4, LOW);
    digitalWrite(A3, LOW);

    digitalWrite(A5, HIGH);
    digitalWrite(4, LOW);
  }
  if ((millis() -  Millis_Giroflex > 1000) && (millis() -  Millis_Giroflex < 1100)) {
    digitalWrite(A4, LOW);
    digitalWrite(A3, HIGH);

    digitalWrite(A5, LOW);
    digitalWrite(4, HIGH);
    Millis_Giroflex = millis();
  }
} else if (Giroflex == 2) {

  if ((millis() -  Millis_Giroflex > 100) && (millis() -  Millis_Giroflex < 200)) {
    digitalWrite(A4, LOW);
    digitalWrite(A3, HIGH);

    digitalWrite(A5, HIGH);
    digitalWrite(4, LOW);
  }
  if ((millis() -  Millis_Giroflex > 200) && (millis() -  Millis_Giroflex < 300)) {
    digitalWrite(A4, HIGH);
    digitalWrite(A3, LOW);

    digitalWrite(A5, LOW);
    digitalWrite(4, HIGH);
    Millis_Giroflex = millis();
  }
} else if (Giroflex == 3) {

  if ((millis() -  Millis_Giroflex > 100) && (millis() -  Millis_Giroflex < 200)) {
    digitalWrite(A4, HIGH);
    digitalWrite(A3, HIGH);

    digitalWrite(A5, HIGH);
    digitalWrite(4, LOW);
  }
  if ((millis() -  Millis_Giroflex > 200) && (millis() -  Millis_Giroflex < 300)) {
    digitalWrite(A4, LOW);
    digitalWrite(A3, LOW);

    digitalWrite(A5, LOW);
    digitalWrite(4, HIGH);
    Millis_Giroflex = millis();
  }
}
else {
  digitalWrite(A3, LOW);
  digitalWrite(A4, LOW);
}


//🔵Pare de quebrar a cabeça e realize seu sonho de infância! Entenda de uma vez por todas,
//como dar vida a seus projetos e controlar tudo via controle remoto e com baixo custo, usando arduino!
//👉Acesse: https://arduinoparamodelismo.com



// USADO PARA FAROL E LANTERNAS
if (ps2x.ButtonPressed(PSB_RED)) {           // BOTÃO CÍRCULO
  BotaoCirculo = ! BotaoCirculo;
  //Serial.print("Botao Circulo: ");
  //Serial.println( BotaoCirculo);

  digitalWrite(2, LOW);        // Apaga Farol
  digitalWrite(3, LOW);        // Apaga Lanterna
}
if (BotaoCirculo == HIGH) {
  digitalWrite(2, HIGH);       // Acende Farol
  digitalWrite(3, HIGH);       // Acende lanterna traseira
}


// USADO PARA OS SONS DE SIRENE
if (ps2x.ButtonPressed(PSB_PINK)) {           // BOTÃO QUADRADO
  BotaoQuadrado = ! BotaoQuadrado;

  Sirene++;
  if (Sirene > 3) {
    Sirene = 0;
  }
  //Serial.print("Botao Quadrado: ");
  //Serial.println( Sirene);

  if (Sirene == 2) {

    myDFPlayer.play(1);  //Loop the first mp3
    //Serial.println( "Som 01");
  }
  if (Sirene == 1) {

    myDFPlayer.play(2);  //Loop the first mp3
    //Serial.println( "Som 02");
  }
  if (Sirene == 3) {

    myDFPlayer.play(3);  //Loop the first mp3
    //Serial.println( "Som 03");
  }
  if (Sirene == 0) {
    myDFPlayer.pause();  //pause the mp3
    //Serial.println( "parar");
  }
}

if (BotaoQuadrado == HIGH) {
}




// USADO PARA BUZINA *** PARA BUZINA FOI USADO O COMANDO ps2x.Button E NÃO ps2x.ButtonPressed PARA QUE A BUZINA SEJA ACIONADA APENAS ENQUANTO ESTIVERMOS PRECIONANDO O BOTÃO
if (ps2x.ButtonPressed(PSB_BLUE)) {          // BOTÃO X
  BotaoX = ! BotaoX;
  //Serial.print("Botao X: ");
  //Serial.println( BotaoX);
  myDFPlayer.play(4);  //Play the first mp3
  delay(100);
}

// Analógicos
if ((ps2x.Analog(PSS_LY) == 255) && (ps2x.Analog(PSS_LX) == 255) && (ps2x.Analog(PSS_RY) == 255) && (ps2x.Analog(PSS_RX) == 255)) {
  //Serial.println("Erro de Leitura");
} else {

  JoyDireitaX = ps2x.Analog(PSS_RX);
  JoyDireitaY = ps2x.Analog(PSS_RY);
  JoyEsquerdaX = ps2x.Analog(PSS_LX);
  JoyEsquerdaY = ps2x.Analog(PSS_LY);


  //Retire os Comentários para Exibir os Valores dos Joysticks Analógicos
  //Os valores estão comentados para não 'poluir' o monitor serial

  if ((JoyDireitaX > 130) || (JoyDireitaX < 125) || (JoyDireitaY > 130) || (JoyDireitaY < 125 )) {
    //Serial.print("Joy Direita X: ");
    //Serial.println( JoyDireitaX);
    //Serial.print("Joy Direita Y: ");
    //Serial.println( JoyDireitaY);
  }

  if ((JoyEsquerdaX > 130) || (JoyEsquerdaX < 125) || (JoyEsquerdaY > 130) || (JoyEsquerdaY < 125 )) {
    //Serial.print("Joy Esquerda X: ");
    //Serial.println( JoyEsquerdaX);
    //Serial.print("Joy Esquerda Y: ");
    //Serial.println( JoyEsquerdaY);
  }
}
// CONTROLE DE DIREÇÃO (SERVO MOTOR)

int aux = 0; // variável para auxiliar na converasão dos valores dos joysticks de 0 a 255 para os valores utilizados nos servo motores e ponte H

aux = map(JoyDireitaX, 0, 255, 10, 170);

servoDirecao.write(aux); // Configura a posição do Servo de acordo com a posição do joystick (valor da variável aux)



//🔵Pare de quebrar a cabeça e realize seu sonho de infância! Entenda de uma vez por todas,
//como dar vida a seus projetos e controlar tudo via controle remoto e com baixo custo, usando arduino!
//👉Acesse: https://arduinoparamodelismo.com

/*
  CONTROLE DE ACELERAÇÃO, FRENTE, RÉ E PARADO
  Para a elaboração do controle de aceleração, frente e ré foram levados em consideração os valores enviados pelo controle,
  que vai do valor minimo de 0 até o valor máximo 255, tendo o valor central do potenciômetro por volta de 127
*/


if (JoyEsquerdaY <= 125) {                  // FRENTE - quando o bastão do joystick virtual é empurado para cima (posição central é = 255, todo para cima = 0)
  aux = map(JoyEsquerdaY, 125, 0, 20, 255); // Converte os valores do joystick para o padrão PWM de 0 a 255 (neste caso iniciei em 20 pois valores abaixo disso não conseguem acionar o motor)
  analogWrite(5, LOW);               // ativa pino ponte H com valor LOW para que o veículo ande para frente
  analogWrite(6, aux);               // ativa pino ponte H com o valor da variável aux (controle de aceleração)

  digitalWrite(4, LOW); //Apaga Luz de Ré
  cont = 0;
  digitalWrite(3, digitalRead(2));   // Se o motor for acionada para Frente, verifica se o farol (D2) esta aceso, se estiver mantém a Lanterna (D3) acesa, se não apaga

} else if (JoyEsquerdaY >= 130) {             // RÉ - quando o bastão do joystick virtual é empurado para baixo (posição central é = 127, todo para baixo = 255)
  aux = map(JoyEsquerdaY, 130, 255, 20, 255); // Converte os valores do joystick para o padrão PWM de 0 a 255 (neste caso iniciei em 20 pois valores abaixo disso não conseguem acionar o motor)
  analogWrite(5, aux);                 // ativa pino ponte H com o valor da variável aux (controle de aceleração)
  analogWrite(6, LOW);                 // configura o pino ponte H com valor LOW para que o veículo ande para trás


  digitalWrite(4, HIGH); //Acende Luz de Ré
  cont = 0;

} else {               // MOTOR PARADO
  analogWrite(5, LOW); // desativa as duas portas da ponte h
  analogWrite(6, LOW); // desativa as duas portas da ponte h

  cont ++;
  if (cont == 1) {
    digitalWrite(4, LOW); //Apaga Luz de Ré
  }
  cont = 2;

  digitalWrite(3, digitalRead(2)); // Se o motor estiver parado, verifica se o farol (D2) esta aceso, se estiver mantém a Lanterna (D3) acesa, se não apaga
}
delay(20); // delay importante para o funcionamento da comunicação entre receptor ps2 e arduino
}

// FIM DO CÓDIGO

//🔵Pare de quebrar a cabeça e realize seu sonho de infância! Entenda de uma vez por todas,
//como dar vida a seus projetos e controlar tudo via controle remoto e com baixo custo, usando arduino!
//👉Acesse: https://arduinoparamodelismo.com
