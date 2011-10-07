//Spinelli's Basic interpreter, ported to arduino bu Usmar A Padow usmpadow@gmail.com
//EULA you must include the name of the porter of the software on any place you use this code. please acknoladge my work :)
*/ sample program
10 PRINT "HOW OLD ARE YOU?"
20 INPUT A
30 IF A>18 THEN 80
40 PRINT "YOU ARE TOO YOUNG TO DRINK"
50 END
80 PRINT "HERE IS YOUR BEER"
99 END
*/
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
                gets2(buf);//needs to be gets on PC and gets2 on arduino
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
                                      
                                      
                                      gets2(buf);//needs to be gets on PC and gets2 on arduino
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
