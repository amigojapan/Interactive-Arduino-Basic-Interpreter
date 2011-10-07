//C++ arduino simulator alpha Copyright 2011 Usmar A Padow
//Parallel communication between 2 arduinos
//to see output of this program do:
//g++ -g -o arduinosimulator_Parallel_protocol_dual_arduino arduinosimulator_Parallel_protocol_dual_arduino.cpp 
//a@carrydesktop:~/arduino$ ./arduinosimulator_Parallel_protocol_dual_arduino |less

#define HIGH 1
#define LOW 0
#define INPUT 1
#define OUTPUT 0
#define UNSET 2
#include <stdio.h>
#include <unistd.h> //for the usleep function and fork
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno */

class Pin {
 private:
 public:
   int *ppinval;
   int pinmode;
 Pin(){
	 this->pinmode=UNSET;
 }
   
   /*
   void connect(Pin _pin){
	 int test=1;
     this->ppinval=_pin.ppinval;
   }
   */
};

class Serial_ {
 private:
 public:
   void print(char str[]){
	 printf("%s",str);
   }
   char read(){//these functions need to be fixed so they do not require return to be pressed, perhaps using SDL or curses
	 return getchar();
   }
   int available(){
	 return 1;//getchar() != '';
   }
   int begin(int speed){
	 return speed;//getchar() != '';
   }

}Serial;

int digitalRead(Pin _pin){
  return *_pin.ppinval;
}
int digitalWrite(Pin _pin, int val){
  *_pin.ppinval = val;
}

Pin PinNull;//this pin is so that I can initialize the array with something to avoid segmentation faults
Pin *AssociativeArray[2][1000];//this array is to keep track of which pin is connected to which to check for short circuits		
void initArray(void) {
	int x;
	for(x=0;x<=1000-1;x++) {
		AssociativeArray[0][x]=&PinNull;
		AssociativeArray[1][x]=&PinNull;
	}
}

void pinMode(Pin *_pin, int mode){//I think I should add a way to warn which pin is short circuiting which pin
	if(mode==OUTPUT) {
		int x;
		for(x=0;x<=1000-1;x++) {
			//printf("AssociativeArray[0][x]=%x _pin=%x &PinNull=%x\n",AssociativeArray[0][x],_pin,&PinNull);
			if(AssociativeArray[0][x]==_pin) {
				Pin tmp=*AssociativeArray[1][x];
				if(tmp.pinmode==OUTPUT) {
					printf("ERROR! possible short circuit detected between two pins connected together and are both set to OUTPUT, this can fry a port on your arduino!\n");
					exit(1);
				}
			}
		}
	}
	//Pin tmp=*_pin;
	//tmp.pinmode=mode;
 //here is the next problem, I cannot seem to set this property
 //when I have a pointer to an object how can I assing a value to a variable inside the object?
  //*_pin.pinmode=mode;
  _pin->pinmode=mode;
}

void delayMicroseconds(int microseconds) {
	usleep(microseconds);//microseconds are a millionth of a second
}
void delay(int milliseconds) {
	usleep(milliseconds*1000);//microseconds are a millionth of a second
}

//make pin obects 
Pin reciever_recieve_ready;//acknowledgement pin
Pin reciever_send_ready;//acknowledgement pin
Pin reciever_dataPin0;
Pin reciever_dataPin1;
Pin reciever_dataPin2;
Pin reciever_dataPin3;
Pin reciever_dataPin4;
Pin reciever_dataPin5;
Pin reciever_dataPin6;
Pin reciever_dataPin7;

Pin sender_recieve_ready;//acknowledgement pin
Pin sender_send_ready;//acknowledgement pin
Pin sender_dataPin0;
Pin sender_dataPin1;
Pin sender_dataPin2;
Pin sender_dataPin3;
Pin sender_dataPin4;
Pin sender_dataPin5;
Pin sender_dataPin6;
Pin sender_dataPin7;

//create wires
int wire0=LOW;
int wire1=LOW;
int wire2=LOW;
int wire3=LOW;
int wire4=LOW;
int wire5=LOW;
int wire6=LOW;
int wire7=LOW;
int wire8=LOW;
int wire9=LOW;
//define how often the circuit should be tested the smaller the faster it will be updated
//int CircuitUpdateDelay=200;//wait 200 milliseconds before updating circuit

//define the behaviour of your circuit here, I will be pretending I have an LED connected to a wire(lets ignore the needed ressistor for now)
void circuit(void) {
	//in this circuit the wires just transfer power(in real life we will need resistors). we dont need to define circuit behaviour
}

void connect(){
	//set initial values of a pin or you will get a segmentation fault
    reciever_recieve_ready.ppinval=&wire0;//acknowledgement pin
    reciever_send_ready.ppinval=&wire1;//acknowledgement pin

	reciever_dataPin0.ppinval=&wire2;
	reciever_dataPin1.ppinval=&wire3;
	reciever_dataPin2.ppinval=&wire4;
	reciever_dataPin3.ppinval=&wire5;
	reciever_dataPin4.ppinval=&wire6;
	reciever_dataPin5.ppinval=&wire7;
	reciever_dataPin6.ppinval=&wire8;
	reciever_dataPin7.ppinval=&wire9;

    sender_recieve_ready.ppinval=&wire0;//crossing wires
    sender_send_ready.ppinval=&wire1;//crossing wires

	sender_dataPin0.ppinval=&wire2;
	sender_dataPin1.ppinval=&wire3;
	sender_dataPin2.ppinval=&wire4;
	sender_dataPin3.ppinval=&wire5;
	sender_dataPin4.ppinval=&wire6;
	sender_dataPin5.ppinval=&wire7;
	sender_dataPin6.ppinval=&wire8;
	sender_dataPin7.ppinval=&wire9;
	
	//populare array of pins that are connected to each other
	//I wond do this here because I wont be cchanging the pinMode at runtime
}


//write you arduino code here
//these libraries for the binary converter functions
//#include <fstream>
#include <cmath>
#include <string.h>

//binary conversion functions
 void BtoA(unsigned char ch, char *b){
  int mask = 0x80;
  int i;
 
  for (i = 0; i < 8; ++i)
  {
    b[i] = ch & mask ? '1' : '0';
    mask >>= 1;
  }
 
  b[i] = '\0';}
 int AtoB(char *b){
  int value = 0;
  int i;
 
  for (i = 0; i < 8; ++i)
  {
    value <<= 1;
    value |= (b[i] == '1') ? 1 : 0;
  }
 
  return value;}
  
void reciever_set_datapins_to_input_mode() {
	pinMode(&reciever_dataPin0,INPUT);
	pinMode(&reciever_dataPin1,INPUT);
	pinMode(&reciever_dataPin2,INPUT);
	pinMode(&reciever_dataPin3,INPUT);
	pinMode(&reciever_dataPin4,INPUT);
	pinMode(&reciever_dataPin5,INPUT);
	pinMode(&reciever_dataPin6,INPUT);
	pinMode(&reciever_dataPin7,INPUT);
}	

void reciever_set_datapins_to_output_mode() {
	pinMode(&reciever_dataPin0,OUTPUT);
	pinMode(&reciever_dataPin1,OUTPUT);
	pinMode(&reciever_dataPin2,OUTPUT);
	pinMode(&reciever_dataPin3,OUTPUT);
	pinMode(&reciever_dataPin4,OUTPUT);
	pinMode(&reciever_dataPin5,OUTPUT);
	pinMode(&reciever_dataPin6,OUTPUT);
	pinMode(&reciever_dataPin7,OUTPUT);
}	

char reciever_buffer='R';
void reciever_loop() {
	//reciever
	//set ready to input to on
	digitalWrite(reciever_recieve_ready, HIGH);
	//while ready to send is off do nothing
	//when ready to send goes on, 
	if(digitalRead(reciever_send_ready)==HIGH) {
		//set ready to recieve off,
		digitalWrite(reciever_recieve_ready, LOW);
		//the transition from low to high will probably be too fast so I think i need a delay here
		delay(1000);
		//then read the datapins
		char bitbuffer[9]="";
		//Read all datapins
		sprintf(bitbuffer,"%i%i%i%i%i%i%i%i"
		,digitalRead(reciever_dataPin0)
		,digitalRead(reciever_dataPin1)
		,digitalRead(reciever_dataPin2)
		,digitalRead(reciever_dataPin3)
		,digitalRead(reciever_dataPin4)
		,digitalRead(reciever_dataPin5)
		,digitalRead(reciever_dataPin6)
		,digitalRead(reciever_dataPin7)
		);
		//Convert from binary to byte
		unsigned char read_character;
		read_character=AtoB(bitbuffer);
		//print to serial port
		char output[2];
		sprintf(output,"%c",read_character);
		Serial.print(output);
		//for some reason this doesnt print anything in the console, but the debugger says ti works
		//printf("%s",output);
		//set ready to reciveve back on
		digitalWrite(reciever_recieve_ready	, HIGH);
	}
}

void sender_set_datapins_to_input_mode() {
	pinMode(&sender_dataPin0,INPUT);
	pinMode(&sender_dataPin1,INPUT);
	pinMode(&sender_dataPin2,INPUT);
	pinMode(&sender_dataPin3,INPUT);
	pinMode(&sender_dataPin4,INPUT);
	pinMode(&sender_dataPin5,INPUT);
	pinMode(&sender_dataPin6,INPUT);
	pinMode(&sender_dataPin7,INPUT);
}	

void sender_set_datapins_to_output_mode() {
	pinMode(&sender_dataPin0,OUTPUT);
	pinMode(&sender_dataPin1,OUTPUT);
	pinMode(&sender_dataPin2,OUTPUT);
	pinMode(&sender_dataPin3,OUTPUT);
	pinMode(&sender_dataPin4,OUTPUT);
	pinMode(&sender_dataPin5,OUTPUT);
	pinMode(&sender_dataPin6,OUTPUT);
	pinMode(&sender_dataPin7,OUTPUT);
}	


char sender_buffer='S';
void sender_loop() {
	//sender
	//get input from serial
	if(Serial.available()) {
		//sender_buffer=Serial.read();
	}	
	//set ready to send of
	digitalWrite(sender_send_ready, LOW);
	//while there is no input just loop
	//if there is input,
	if(sender_buffer!='\0') {
		//then process chacarter to binary
		//convert sender_buffer to binary
		char bitbuffer[9]="";
		BtoA(sender_buffer, bitbuffer);
		//set datapins
		//Write the state of each bit to a wire
		digitalWrite(sender_dataPin0, bitbuffer[0]=='1'?HIGH:LOW);
		digitalWrite(sender_dataPin1, bitbuffer[1]=='1'?HIGH:LOW);
		digitalWrite(sender_dataPin2, bitbuffer[2]=='1'?HIGH:LOW);
		digitalWrite(sender_dataPin3, bitbuffer[3]=='1'?HIGH:LOW);
		digitalWrite(sender_dataPin4, bitbuffer[4]=='1'?HIGH:LOW);
		digitalWrite(sender_dataPin5, bitbuffer[5]=='1'?HIGH:LOW);
		digitalWrite(sender_dataPin6, bitbuffer[6]=='1'?HIGH:LOW);
		digitalWrite(sender_dataPin7, bitbuffer[7]=='1'?HIGH:LOW);			

		//set ready to send to on
		digitalWrite(sender_send_ready, HIGH);
		//set buffer off
		sender_buffer='\0';
	}
	//wait until ready to recieve goes off then set ready to send off
	if(digitalRead(sender_recieve_ready)==LOW) {
		digitalWrite(sender_send_ready, LOW);
	}
}

//Spinelli's Basic interpreter, ported to arduino bu Usmar A Padow usmpadow@gmail.com
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int *callp,callstk[10],forln[10],forend[10],var[100],l,i,quoted;
char buf[100],F[2];
char *ln[100],*p,*q,*x,*y,*z,*s,*d,*f;

char *findstr(char *s,char *o) {
        for(x=s;*x;x++){
                for(y=x,z=o; *z&&*y==*z; y++)
                        z++;
                if(z>o&&!*z)
                        return x;
        }
        return 0;
}
void editln(){
        l=atoi(buf);
        //ln[l] && free(ln[l]);
        /*
        (p=findstr(buf," "))
                ? strcpy(ln[l]=malloc(strlen(p)),p+1)
                : (ln[l]=0,0);
        */
        p=findstr(buf," ");
        if(findstr(buf," ")) {
                //ln[l]= new char [strlen(p)];
        
                if(!(ln[l]=(char*)malloc(sizeof(char)*strlen(p)))){
                        Serial.print("Insufficient memory");
                }
                 
                strcpy(ln[l],p+1);
        } else {
                (ln[l]=0,0);
        }
}

int eqexp();
int atomexp();
int mulexp();
int addexp();
int cmpeexp();
int cmpexp();

int eqexp(){
        int o=cmpexp();
        switch(*p++){
                ;
                break;
        case '=':
                return o == eqexp();
                ;
                break;
        case '#':
                return o != eqexp();
        default:
                p--;
                return o;
        }
}
int debug(){
        char tmp[6];
        sprintf(tmp,"*p is %s,var[*p] is %d\r\n",*p,var[*p]);
        Serial.print(tmp);
        return 0;
}
    
int atomexp(){
        int o;
        return *p=='-'
                ? p++,-atomexp()
                : *p>='0' && *p<='9'
                        ? strtol(p,&p,0)
                        : *p=='('
                                ? p++,o=eqexp(),p++,o
                                : var[*p++];
}

int mulexp(){
        int o=atomexp();
        switch(*p++){
                ;
                break;
        case '*':
                return o * mulexp();
                ;
                break;
        case '/':
                return o / mulexp();
        default:
                p--;
                return o;
        }
}

int addexp(){
        int o=mulexp();
        switch(*p++){
                ;
                break;
        case '+':
                return o + addexp();
                ;
                break;
        case '-':
                return o - addexp();
        default:
                p--;
                return o;
        }
}

int cmpeexp(){
        int o=addexp();
        switch(*p++){
                ;
                break;
        case '$':
                return o <= cmpeexp();
                ;
                break;
        case '!':
                return o >= cmpeexp();
        default:
                p--;
                return o;
        }
}
int cmpexp(){
        int o=cmpeexp();
        switch(*p++){
                ;
                break;
        case '<':
                return o < cmpexp();
                ;
                break;
        case '>':
                return o > cmpexp();
        default:
                p--;
                return o;
        }
}
void puts2(char *s) {
       //Serial.print("puts2");
       Serial.print(s);
       //return 0;
}
void gets2(char *s) {
      int inbyte=0;
      char tmp[2];
      strcpy(s,"");//clear string
      while(inbyte!=13){//13 is enter
            if(Serial.available()) {
                inbyte=Serial.read();
                
                sprintf(tmp,"%c",inbyte);
                strcat(s,tmp);//add this byte
                if(inbyte!=13) Serial.print(tmp);//echo byte to terminal
            }

      }
      //Serial.print(s);//print string

}

void setup(){
      //Serial.begin(57600);
      Serial.begin(9600);
}
void loop(){
  //Serial.print("here");
        //puts2("test");
        //ln[11*100]="E";
        //ln[1]="E";
        //while(1) {
        ln[99]=(char*)malloc(sizeof(char)*4);
        strcpy(ln[99],"END");
        while(1) {
                puts2("\n\rOk\n\r"); 
                gets(buf);//needs to be gets on PC and gets2 on arduino
                //Serial.print(buf);
                switch(*buf) {
                        ;
                        break;
                case 'R': // RUN
                        callp=callstk;
                        l=1;
                        for(i=0;i<100;var[i++]=0);//I think this reinitializes all variables to 0
                        while(l){
                                while(!(s=ln[l]))//this ends up putting the next line value into s.isint this always true since it is an assignment? guess not since it terminates. hmmm
                                        l++;
                                if (!findstr(s,"\"")){
                                        while(p=findstr(s,"<>"))
                                                *p++='#',*p=' ';
                                        while(p=findstr(s,"<="))
                                                *p++='$',*p=' ';
                                        while(p=findstr(s,">="))
                                                *p++='!',*p=' ';
                                }
                                d=buf;
                                // this algorythm seems to remove what is in d, like "RUN" and set it to the last character in s, so it sets d to the variable name I think, I think this may be failing in arduino for some reason(i dont know who wrote this comment)=put line in buf with no spaces 
                                //maybe I can replace this with just something like *d=s[6]   not sure if 6, but the position of the variable name....  but I dont know if this will brake other functionality of the program
                                //*d=s[6];
  
                                if(s[0]=='P') {//if it is PRINT
                                                if(s[6]=='"') {
                                                        *d=0; //puts2(buf+6);
                                                        int counter=7;
                                                        char tmp[2];
                                                        while(1){
                                                            if(s[counter]=='"') break;//look for the ending quotes
                                                            counter++;
                                                        }
                                                        Serial.print("\n\r");
                                                        int counter2;
                                                        for(counter2=7;counter2<=counter-1;counter2++) {
                                                              //sprintf(tmp,"%c",s[counter2]);
                                                              tmp[0]=s[counter2];
                                                              tmp[1]='\0';
                                                              Serial.print(tmp);
                                                        }  
                                                } else {
                                                      char tmp[20];
                                                      p=buf+5;
                                                      p[1]='\0';
                                                      //sprintf(tmp,"\r\np is %s|\r\n",p);
                                                      //Serial.print(tmp);
                                                      //sprintf(tmp,"\r\n%d\r\n",eqexp());
                                                      sprintf(tmp,"\r\n%d\r\n",var[s[6]]);
                                                      //sprintf(tmp,"%d\r\n",eqexp());
                                                      Serial.print(tmp);
                                                      //sprintf(tmp,"\r\nvar['Y'] is %d\r\n",var['Y']);
                                                      //sprintf(tmp,"%d\r\n",eqexp());
                                                      //Serial.print(tmp);
                                                }
                                
                                }
                                if(s[0]=='I' && s[1]=='N') {//if it is INPUT
                                      Serial.print("\r\nINPUT NUMBER:\r\n");
                                      //char tmp[60];
                                      //sprintf(tmp,"\r\ns is %s\r\n",s);
                                      //Serial.print(tmp);
                                      //*d=s[6];
                                      char varname;
                                      varname=s[6];
 
                                      //sprintf(tmp,"\r\n*d is %c\r\n",*d);
                                      //Serial.print(tmp);
                                      
                                      
                                      gets(buf);//needs to be gets on PC and gets2 on arduino
                                      p=buf;
                                      //sprintf(tmp,"\r\n*d is %c\r\n",*d);
                                      //Serial.print(tmp);
                                      //*d='Y';// it seems there is a bug here, *d is not set to the right value, but even when I set it right, the print statement wont print the value
                                      //sprintf(tmp,"*d is %c eqexp is %d\r\n",*d,eqexp());
                                      //Serial.print(tmp);
                                      //var[*d]=eqexp();
                                      var[varname]=eqexp();
                                      //var['Y']=eqexp();//somehow it is not setting the variable to the expression, eventhough the expression contains the right value, ok, eqexp() it seems looses it's value after it is used more than once
                                      //hard coding this works var['Y']=33;
 
                                     
                                      
                                      
                                } else {
                                      while(*F=*s){
                                              *s=='"' && quoted++;
                                              if(quoted&1 || !findstr(" \t",F))//what in the world does this line do?
                                                      *d++=*s;
                                              s++;
                                      }
                                      *d--=quoted=0;
                                }
                                if(buf[1]!='=')
                                        switch(*buf) {
                                                ;
                                                break;
                                        case 'E':        // END 
                                                l=-1;
                                                break;
                                        case 'R':        // REM|RETURN
                                                buf[2]!='M' && (l=*--callp);
                                                break;
                                        case 'I':        // INPUT|IF 
                                        /*
                                                buf[1]=='N'
                                                        ? gets2(p=buf),
                                                                var[*d]=eqexp()
                                                        : (*(q=findstr(buf,"TH"))=0,
                                                                p=buf+2,
                                                                eqexp() && (p=q+4, l=eqexp()-1));
                                        */
                                                if(buf[1]=='N') {
                                                  /*
                                                            gets2(buf);
                                                            p=buf;
                                                            char tmp[60];
                                                            sprintf(tmp,"*d is %c\r\n",*d);
                                                            Serial.print(tmp);
                                                            //*d='Y';// it seems there is a bug here, *d is not set to the right value, but even when I set it right, the print statement wont print the value
                                                            //sprintf(tmp,"*d is %c eqexp is %d\r\n",*d,eqexp());
                                                            //Serial.print(tmp);
                                                            var[*d]=eqexp();
                                                            //var['Y']=eqexp();//somehow it is not setting the variable to the expression, eventhough the expression contains the right value, ok, eqexp() it seems looses it's value after it is used more than once
                                                            //hard coding this works var['Y']=33;
                                                   */
                                                } else {
                                                        
                                                        (*(q=findstr(buf,"TH"))=0,
                                                        p=buf+2,
                                                        eqexp() && (p=q+4, l=eqexp()-1));
                                                }
                                                break;
                                        case 'P':        // PRINT
                                        /*
                                                if(buf[5]=='"') {
                                                        *d=0; //puts2(buf+6);
                                                        int counter=6,tmp;
                                                        while(1){
                                                            if(buf[counter]=='"') break;
                                                            counter++;
                                                        }
                                                        Serial.print("\n\r");
                                                        int counter2;
                                                        for(counter2=6;counter2<=counter-1;counter2++) {
                                                              char tmp[2];
                                                              sprintf(tmp,"%c",buf[counter2]);
                                                              Serial.print(tmp);
                                                        }  
                                                } else {
                                                      char tmp[20];
                                                      p=buf+5;
                                                      p[1]='\0';
                                                      //sprintf(tmp,"\r\np is %s|\r\n",p);
                                                      //Serial.print(tmp);
                                                      //sprintf(tmp,"\r\n%d\r\n",eqexp());
                                                      sprintf(tmp,"\r\ns%s\r\n",s);
                                                      //sprintf(tmp,"%d\r\n",eqexp());
                                                      Serial.print(tmp);
                                                      //sprintf(tmp,"\r\nvar['Y'] is %d\r\n",var['Y']);
                                                      //sprintf(tmp,"%d\r\n",eqexp());
                                                      //Serial.print(tmp);
                                                }
                                                */
                                                break;
                                        case 'G':        // GOTO|GOSUB
                                                p=buf+4,
                                                buf[2]=='S' && (*callp++=l, p++),
                                                l=eqexp()-1;
                                                break;
                                        case 'F':        // FOR 
                                                *(q=findstr(buf,"TO"))=0;
                                                p=buf+5;
                                                var[i=buf[3]]=eqexp();
                                                p=q+2;
                                                forend[i]=eqexp();
                                                forln[i]=l;
                                                break;
                                        case 'N':        // NEXT 
                                                ++var[*d]<=forend[*d] && (l=forln[*d]);
                                        }
                                else
                                        p=buf+2, var[*buf]=eqexp();
                                l++;
                        };
                        break;
                case 'L':        // LIST 
                        Serial.print("\n\r");
                        for(i=0;i<100;i++)
                                //#include <ctype.h>//for isdigit
                                if(ln[i]) {
                                      char tmp[100];
                                      sprintf(tmp,"%d %s\n",i,ln[i]);
                                      p=buf+5; Serial.print(tmp);
                                }

                                //ln[i] && ;
                        break;
                case 'N':        // NEW
                        for(i=0;i<11*100;i++)
                                //ln[i] && free(ln[i]), ln[i]=0;
                        break;
                case 'Q':        // QUIT 
                        Serial.print("You can't quit");
                        break;
                case 'S':        // SAVE
                        //f=fopen(buf+5,"w");
                        //for(i=0;i<11*100;i++)
                        //        ln[i]&& fprintf(f,"%d %s\n",i,ln[i]);
                        //fclose(f);
                        //break;
                case 'O':        // OLD
                        //f=fopen(buf+4,"r");
                        //while(fgets(buf,100,f))
                        //        (*findstr(buf,"\n")=0,editln());
                        //fclose(f);
                        //break;
                case 0:
                default:        // labeled line 
                        editln();
                }
      }
}



int main(void) {
	initArray();
	connect();
	setup();
	while(1){
		loop();
	}
}
