
#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE
using namespace std;


Thread thread3;
Thread thread4;


UnbufferedSerial pc(USBTX, USBRX);
UnbufferedSerial  dev(PC_6, PC_7); //  BLE  UART6 PC_6 - TX, PC_7 - RX
UnbufferedSerial  rak811(D8, D2); //  RAK811  D8- TX, D2 - RX

int dev_RxLen=0;
int pc_RxLen=0;
int rak811_RxLen=0;
int nomer_stroki=0;
int konec_stroki=0;
int m=0;
int j;


int no_join=0;
  
 char dev_RxBuf[4096] = {0};
 char rak811_RxBuf[4096] = {0};

char pc_RxBuf[4096] = {0};
char buf4[512] = {0};
char buf5[512] = {0};


int dev_RxLen_print;
int iii;
int konec_predidushego;
 
 char Factory_ID[50][200] = {0};
  char iBeacon_ID[50][200] = {0};
   char Major_Minor_Measured_Power[50][200] = {0};
    char MAC[50][200]= {0};
     char RSSI[50][100] = {0};
  
// обработчики прерываний по приему байта с устройства и с компа- просто заполняют буферы свои- при достижении 64 байта в буфере -начинают заполнять снова с нуля

void dev_recv()
{

   if (dev_RxLen<4096)
   {
    dev.read(&dev_RxBuf[dev_RxLen], sizeof(dev_RxBuf[dev_RxLen]));    //  Got 1 char    
    dev_RxLen++;
}
else {
 dev_RxLen=0;
}
}

void rak811_recv()
{
   if (rak811_RxLen<4096)
{
    rak811.read(&rak811_RxBuf[dev_RxLen], sizeof(rak811_RxBuf[rak811_RxLen]));    //  Got 1 char    
    rak811_RxLen++;
}
else {
 rak811_RxLen=0;
}
}

 
void pc_recv()
{
       if (pc_RxLen<4096)
   {
    pc.read(&pc_RxBuf[pc_RxLen], sizeof(pc_RxBuf[pc_RxLen]));  //  Got 1 char
    pc_RxLen++;
   }
else {
 pc_RxLen=0;
}
}


/////////////////////////// не используется
void print_MAC()
{
 for (iii = 1; iii <konec_stroki+1; iii++) {  
printf("dev_RxLen_print= %d\r\n",dev_RxLen_print);
printf("konec_stroki=%d\r\n", konec_stroki);
printf("MAC_%d: %c%c%c%c%c%c%c%c%c%c%c%c\n", iii, MAC[iii][0],MAC[iii][1], MAC[iii][2], MAC[iii][3], MAC[iii][4], MAC[iii][5], MAC[iii][6], MAC[iii][7], MAC[iii][8], MAC[iii][9], MAC[iii][10], MAC[iii][11]);
printf("RSSI_%d: %c%c%c%c\n", iii, RSSI[iii][0],RSSI[iii][1], RSSI[iii][2], RSSI[iii][3]);
   ThisThread::sleep_for(200ms);
}
}
///////////////////////


void copy_to_my_bufer()
{
int ii=0; 
char last_bukva;
int dvoetochie;
int nomer_stroki=0;
 
if (dev_RxLen>0)
{
dev_RxLen_print=dev_RxLen;

for (int j = 0; j < dev_RxLen; j++) 
{
   // if start scan (DISIS\r\n)
        if((dev_RxBuf[j] == 'D')&&(dev_RxBuf[j+1] == 'I')&&(dev_RxBuf[j+2] == 'S')&&(dev_RxBuf[j+3] == 'I')&&(dev_RxBuf[j+4] == 'S')&&(dev_RxBuf[j+5] == '\r')&&(dev_RxBuf[j+6] == '\n'))
{     
    dvoetochie=0;
    nomer_stroki=0;
    ii=0;   
    j=j+7;
} 
//если  \r\n OK+DISC
 else if ((dev_RxBuf[j] == '\r')&&(dev_RxBuf[j+1] == '\n')&&(dev_RxBuf[j+2] == 'O')&&(dev_RxBuf[j+3] == 'K')&&(dev_RxBuf[j+4] == '+')
 &&(dev_RxBuf[j+5] == 'D')&&(dev_RxBuf[j+6] == 'I')&&(dev_RxBuf[j+7] == 'S')&&(dev_RxBuf[j+8] == 'C'))
    {  
    dvoetochie=0;
    nomer_stroki++;
    }
      
    if((dev_RxBuf[j] == ':')
    // если пришло  ERROR: то не считать
    &&!((dev_RxBuf[j-5] == 'E')&&(dev_RxBuf[j-4] == 'R')&&(dev_RxBuf[j-3] == 'R')&&(dev_RxBuf[j-2] == 'O')&&(dev_RxBuf[j-1] == 'R'))
    // если пришло  =lora: то не считать
    &&!((dev_RxBuf[j-5] == '=')&&(dev_RxBuf[j-4] == 'l')&&(dev_RxBuf[j-3] == 'o')&&(dev_RxBuf[j-2] == 'r')&&(dev_RxBuf[j-1] == 'a'))
      // если пришло  =lora:1: то не считать
    &&!((dev_RxBuf[j-7] == '=')&&(dev_RxBuf[j-6] == 'l')&&(dev_RxBuf[j-5] == 'o')&&(dev_RxBuf[j-4] == 'r')&&(dev_RxBuf[j-3] == 'a')&&(dev_RxBuf[j-2] == ':')&&(dev_RxBuf[j-1] == '1'))     
    )
    {
    dvoetochie++;
    ii=0;
    }

    else
 {  
      // если пришло  0x00  или если пришло  \r  или \n то убрать
        if (!((dev_RxBuf[j] == 0x00)||(dev_RxBuf[j]=='\r')||(dev_RxBuf[j]=='\n')))
{       
  
  if(dvoetochie == 1)
        {  
    Factory_ID[nomer_stroki][ii]=dev_RxBuf[j];
    ii++; 
        }
  if(dvoetochie == 2)
        {  
    iBeacon_ID[nomer_stroki][ii]=dev_RxBuf[j];
    ii++; 
        }
if(dvoetochie == 3)
        {  
    Major_Minor_Measured_Power[nomer_stroki][ii]=dev_RxBuf[j];
    ii++; 
        }
if(dvoetochie == 4)
        {  
    MAC[nomer_stroki][ii]=dev_RxBuf[j];
    ii++; 
        }
        // только 4 цифры
if((dvoetochie == 5)&&(ii<4))
        {     
    RSSI[nomer_stroki][ii]=dev_RxBuf[j];
    ii++; 
        }

 }
 }
   // if if stop scan  (DISCE\r\n)
     if((dev_RxBuf[j] == 'D')&&(dev_RxBuf[j+1] == 'I')&&(dev_RxBuf[j+2] == 'S')&&(dev_RxBuf[j+3] == 'C')&&(dev_RxBuf[j+4] == 'E')&&(dev_RxBuf[j+5] == '\r')&&(dev_RxBuf[j+6] == '\n'))
{
konec_stroki=nomer_stroki;
}   
/* формат строки выдваемой сканером 
после OK+DISC через двоеточие следующие параметры
//Factory ID[ii]
//iBeacon ID[ii] — ID маяка (iBeacon ID)
//Major_Minor_Measured Power[ii]
///MAC[ii]
//RSSI[ii]
*/
}
 dev_RxLen=0;
  }
}

void scan(){

      sprintf(buf4,"AT+DISI?\r\n"); 
  for (uint8_t i = 0; i < 10; i++) {  
    dev.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  модуль
//    pc.write(&buf4[i], sizeof(buf4[i]));
  }
       ThisThread::sleep_for(5s);// 
  
  copy_to_my_bufer();
 // print_MAC();  распечатать МАС - только для отладки -выключено
       ThisThread::sleep_for(1s);
}


void MAC_send_to_server_lora()
{

     while (true) {
// старт скан 
    scan();
//ПОСЛАТЬ ВСЕ ПАРАМЕТРЫ МАС мАЯЧКА***************ПОСЫЛАЕТ ТОЛЬКО ЕГО (МАС НАЧИНАЕТСЯ С 8801F98)
//////////////////////////////
for (iii = 1; iii <konec_stroki; iii++) 
{ 
///
    /////закоментить чтобы выдавать все найденные МАС
if ((MAC[iii][0]=='8')&&(MAC[iii][1]=='8')&&(MAC[iii][2]=='0')&&(MAC[iii][3]=='1')&&(MAC[iii][4]=='F')&&(MAC[iii][5]=='9')&&(MAC[iii][6]=='8'))   /////закоментить чтобы выдавать все найденные МАС
{   /////закоментить чтобы выдавать все найденные МАС
///



  //вместо '-' передаем '0'
    RSSI[iii][0]='0'; 
    sprintf(buf5,"at+send=lora:1:%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\r\n",
     MAC[iii][0],MAC[iii][1], MAC[iii][2], MAC[iii][3], MAC[iii][4], MAC[iii][5], MAC[iii][6], MAC[iii][7], MAC[iii][8], MAC[iii][9], MAC[iii][10], MAC[iii][11], 
    RSSI[iii][0],RSSI[iii][1], RSSI[iii][2], RSSI[iii][3], 
    Major_Minor_Measured_Power[iii][0], Major_Minor_Measured_Power[iii][1], Major_Minor_Measured_Power[iii][2], Major_Minor_Measured_Power[iii][3], Major_Minor_Measured_Power[iii][4], Major_Minor_Measured_Power[iii][5], Major_Minor_Measured_Power[iii][6], Major_Minor_Measured_Power[iii][7], Major_Minor_Measured_Power[iii][8], Major_Minor_Measured_Power[iii][9],
    Factory_ID[iii][0], Factory_ID[iii][1], Factory_ID[iii][2], Factory_ID[iii][3], Factory_ID[iii][4], Factory_ID[iii][5], Factory_ID[iii][6], Factory_ID[iii][7] 
      );

 
          for (uint8_t i = 0; i < 51; i++) {  //15+12+2=29     15+12*2+2=41                       15+12+4+10+8+2=51
    rak811.write(&buf5[i], sizeof(buf5[i]));
    //продублировать это в консоль
   pc.write(&buf5[i], sizeof(buf5[i]));
   }          
      ThisThread::sleep_for(1s); // задержка после выдачи на сервер МАС адреса нашего маячка (не только МАС . это 1 пакет)






// 2-УЮ ПОСЫЛКУ 


  sprintf(buf5,"at+send=lora:1:%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\r\n",
     iBeacon_ID[iii][0], iBeacon_ID[iii][1], iBeacon_ID[iii][2], iBeacon_ID[iii][3], iBeacon_ID[iii][4], iBeacon_ID[iii][5], iBeacon_ID[iii][6], iBeacon_ID[iii][7], 
     iBeacon_ID[iii][8], iBeacon_ID[iii][9], iBeacon_ID[iii][10], iBeacon_ID[iii][11], iBeacon_ID[iii][12], iBeacon_ID[iii][13], iBeacon_ID[iii][14], iBeacon_ID[iii][15], 
      iBeacon_ID[iii][16], iBeacon_ID[iii][17], iBeacon_ID[iii][18], iBeacon_ID[iii][19], iBeacon_ID[iii][20], iBeacon_ID[iii][21], iBeacon_ID[iii][22], iBeacon_ID[iii][23], 
       iBeacon_ID[iii][24], iBeacon_ID[iii][25], iBeacon_ID[iii][26], iBeacon_ID[iii][27], iBeacon_ID[iii][28], iBeacon_ID[iii][29], iBeacon_ID[iii][30], iBeacon_ID[iii][31]  
      );


 
          for (uint8_t i = 0; i < 49 ; i++) {  //15+32+2=49
    rak811.write(&buf5[i], sizeof(buf5[i]));
    //продублировать это в консоль
   pc.write(&buf5[i], sizeof(buf5[i]));
   }          
      ThisThread::sleep_for(1s); // задержка после выдачи на сервер 2-го пакета данных нашего маячка



}  /////закоментить чтобы выдавать все найденные МАС


/////////////////////////


else 
// ЗДЕСЬ ВЫДАЕТ ЧТО ЭТО НЕ ТОТ АДРЕС И  СВОЙ НОМЕР В СПИСКЕ НАЙДЕННЫХ. ПО ИХ КОЛИЧЕСТВУ МОЖНО ОЦЕНИТЬ СКОЛЬКО ОН ВООБЩЕ НАШЕЛ
{

    sprintf(buf4,"NE MAC 8801F98____%d\r\n", iii); 
  for (uint8_t i = 0; i < 23; i++) {
//распечатать на консоль 
    pc.write(&buf4[i], sizeof(buf4[i]));
  }



  //////////////////// если это последний найденный МАС то выдать его - то есть наш МАС не найден
 if (iii==konec_stroki-1) {

  /////////////послать 0x000 вместо МАС если не найдено
  /*
char net_MAC[]={'0','0','0','0','0','0','0','0', '0','0','0','0'};
sprintf(buf5,"at+send=lora:1:%c%c%c%c%c%c%c%c%c%c%c%c\r\n", net_MAC[0], net_MAC[1],  net_MAC[2],  net_MAC[3],  net_MAC[4],  net_MAC[5],  net_MAC[6],  net_MAC[7],  net_MAC[8],  net_MAC[9],  net_MAC[10],  net_MAC[11]) ;

          for (uint8_t i = 0; i < 29; i++) {  //15+12+2=29     
    rak811.write(&buf5[i], sizeof(buf5[i]));
    //продублировать это в консоль
   pc.write(&buf5[i], sizeof(buf5[i]));
   }  
*/


//вместо '-' передаем '0'
    RSSI[iii][0]='0'; 
    sprintf(buf5,"at+send=lora:1:%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\r\n",
     MAC[iii][0],MAC[iii][1], MAC[iii][2], MAC[iii][3], MAC[iii][4], MAC[iii][5], MAC[iii][6], MAC[iii][7], MAC[iii][8], MAC[iii][9], MAC[iii][10], MAC[iii][11], 
    RSSI[iii][0],RSSI[iii][1], RSSI[iii][2], RSSI[iii][3], 
    Major_Minor_Measured_Power[iii][0], Major_Minor_Measured_Power[iii][1], Major_Minor_Measured_Power[iii][2], Major_Minor_Measured_Power[iii][3], Major_Minor_Measured_Power[iii][4], Major_Minor_Measured_Power[iii][5], Major_Minor_Measured_Power[iii][6], Major_Minor_Measured_Power[iii][7], Major_Minor_Measured_Power[iii][8], Major_Minor_Measured_Power[iii][9],
    Factory_ID[iii][0], Factory_ID[iii][1], Factory_ID[iii][2], Factory_ID[iii][3], Factory_ID[iii][4], Factory_ID[iii][5], Factory_ID[iii][6], Factory_ID[iii][7] 
      );

 
          for (uint8_t i = 0; i < 51; i++) {  //15+12+2=29     15+12*2+2=41                       15+12+4+10+8+2=51
    rak811.write(&buf5[i], sizeof(buf5[i]));
    //продублировать это в консоль
   pc.write(&buf5[i], sizeof(buf5[i]));
   }          
      ThisThread::sleep_for(1s); // задержка после выдачи на сервер чужого  МАС адреса (выдает последный найденный то есть случайный)



 }

      ThisThread::sleep_for(20ms); // задержка после выдачи в консоль сообщения что это наш маячок 
      //- нужна только для отладки ( я уменьшил еще дополнительно -было 100мс)



}

///////////////////

}
}
}

void recieve_otvety()
{                   
while(1)
{    
        if (rak811_RxLen>0)
        {    
    //нужно только для отладки -можно убрать
    //распечатать на консоль то что пришло  с рак811
    for (uint8_t i = 0; i < rak811_RxLen; i++) {  
   pc.write(&rak811_RxBuf[i], sizeof(rak811_RxBuf[i]));
   
   ////если нет подключения --- OK Join Success то 

   /*
          if (!((rak811_RxBuf[i]=='O')&&(rak811_RxBuf[i+1]=='K')&&(rak811_RxBuf[i+2]==' ')
          &&(rak811_RxBuf[i+3]=='J')&&(rak811_RxBuf[i+4]=='o')&&(rak811_RxBuf[i+5]=='i')&&(rak811_RxBuf[i+6]=='n')&&(rak811_RxBuf[i+7]==' ')
          &&(rak811_RxBuf[i]=='S')&&(rak811_RxBuf[i]=='u')&&(rak811_RxBuf[i]=='c')&&(rak811_RxBuf[i]=='c')
          &&(rak811_RxBuf[i+8]=='e')&&(rak811_RxBuf[i+9]=='s')&&(rak811_RxBuf[i+10]=='s')))
*/
        if (!((rak811_RxBuf[i]=='J')&&(rak811_RxBuf[i+1]=='o')&&(rak811_RxBuf[i+2]=='i')&&(rak811_RxBuf[i+3]=='n')))
          {
              no_join=1;
          }
          else { no_join=0;}
   
     
   }
    
        rak811_RxLen=0;
       // ThisThread::sleep_for(1s);      
        }
        ThisThread::yield();
   }
   
}






int main()
{
    pc.attach(&pc_recv, UnbufferedSerial::RxIrq);
    dev.attach(&dev_recv, UnbufferedSerial::RxIrq);
    rak811.attach(&rak811_recv, UnbufferedSerial::RxIrq);    
    pc.baud(115200);
    dev.baud(9600);
    rak811.baud(115200);

// подготовка к сканирорвание 
///////////////////////////////

    
sprintf(buf4,"AT+RENEW\r\n"); 
  for (uint8_t i = 0; i < 10; i++) {
    dev.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  модуль
    pc.write(&buf4[i], sizeof(buf4[i]));
  }
      ThisThread::sleep_for(1s);
    sprintf(buf4,"AT+RESET\r\n"); 
  for (uint8_t i = 0; i < 10; i++) {
    dev.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  модуль
    pc.write(&buf4[i], sizeof(buf4[i]));
  }
    ThisThread::sleep_for(1s);
/*

*/


sprintf(buf4,"AT+RESET\r\n"); 
  for (uint8_t i = 0; i < 10; i++) {
    dev.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  модуль
    pc.write(&buf4[i], sizeof(buf4[i]));
  }
    ThisThread::sleep_for(1s);

    sprintf(buf4,"AT\r\n"); 
  for (uint8_t i = 0; i < 4; i++) { 
    dev.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  модуль
    pc.write(&buf4[i], sizeof(buf4[i]));
  }

    ThisThread::sleep_for(1s);

    sprintf(buf4,"AT+ROLE1\r\n"); 
  for (uint8_t i = 0; i < 10; i++) 
  { 
    dev.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  модуль
    pc.write(&buf4[i], sizeof(buf4[i]));
  }

    ThisThread::sleep_for(1s);

    sprintf(buf4,"AT+IMME1\r\n"); 
  for (uint8_t i = 0; i < 10; i++) {  
    dev.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  модуль
    pc.write(&buf4[i], sizeof(buf4[i]));
  }

    ThisThread::sleep_for(1s);

    sprintf(buf4,"AT+RESET\r\n"); 
  for (uint8_t i = 0; i < 10; i++) { 
    dev.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  модуль
    pc.write(&buf4[i], sizeof(buf4[i]));
  }

    ThisThread::sleep_for(1s);

  
 thread4.start(recieve_otvety);

/* This command is used to set the number of retransmitting attempts on an uplink message. When activated, the
board will resend a message if its corresponding ACK (downlink) is not received after sending a confirmed uplink
message. The default value is 0, which means that the board will not resend any message by default.
*/
//количество повторов передачи на ЛОРАВАН =5 . (по умолчанию =0)
sprintf(buf4,"at+set_config=lora:send_repeat_cnt:5\r\n"); 
  for (uint8_t i = 0; i < 38; i++) {  
    rak811.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  рак811
    pc.write(&buf4[i], sizeof(buf4[i]));
        }  
               ThisThread::sleep_for(2s); 

// установим соединение с сервером ЛОРА           
   sprintf(buf4,"at+join\r\n"); 
  for (uint8_t i = 0; i < 9; i++) {  
    rak811.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  рак811
    pc.write(&buf4[i], sizeof(buf4[i]));
        }  
               ThisThread::sleep_for(5s); 

              
   
        ///////////////////////   
/*
   if ( no_join==1) 

{
    while(1)
    {
  sprintf(buf4,"no join\r\n"); 
  for (uint8_t i = 0; i < 9; i++) {  
    rak811.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  рак811
    pc.write(&buf4[i], sizeof(buf4[i]));
        }  
               ThisThread::sleep_for(10s); 

  sprintf(buf4,"at+join\r\n"); 
  for (uint8_t i = 0; i < 9; i++) {  
    rak811.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  рак811
    pc.write(&buf4[i], sizeof(buf4[i]));
        }  
               ThisThread::sleep_for(10s); 

}
}
else { no_join=0;}
*/


            
            /// меняем скорость
        sprintf(buf4,"AT+BAUD4\r\n"); 
  for (uint8_t i = 0; i < 10; i++) {
    dev.write(&buf4[i], sizeof(buf4[i]));
//распечатать на консоль то что послано на  модуль
    pc.write(&buf4[i], sizeof(buf4[i]));
  }
    ThisThread::sleep_for(1s); 
    dev.baud(115200);
    ThisThread::sleep_for(1s); 
///  

//сканировать и посылать на сервер ЛОРА каждые ? секунд
 thread3.start(MAC_send_to_server_lora);    
}
