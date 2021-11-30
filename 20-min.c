#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char*Questions[]={"Are you transgender?","Are you gay/lesbian/bi/etc.?",
"Are you an adult?","Do you know C++ well?",
"Are you currently in a relationship?","Are you a part of the staff?",
"Are you currently in school?",
"Do you have your own Discord server? (doesn't include \"test servers\")",
"Do you have a GitHub account?","Do you live in the U.S.A.?",
"Do you routinely exercize?","Have you ever had a pet?",
"Do you regularly use Rust?",
"Do you often try to program in esoteric programming languages?",
"Are you a part of the GEORGE webring?",
"Do you have more than 10 GitHub repositories?","Tabs (Y) or spaces (N)?",
"Is programming your primary hobby?","Do people call you by your nickname/real name more often than your username/online name?"
,"`if(...)` (Y) or `if (...)` (N)?","Do you actively participate in the Esolangs community? (ie. develop and utilize esolangs)"
,"Do you enjoy linguistics? (eg. conlangs)","Do you use Windows primarily?",
"Would you consider yourself a \"computer whiz\"?",
"Are you content with your current living situation?",
"Do you primarily use Python? (ie. use it more often than other languages)",
"Are you a \"long-standing\" member of Esolangs?",
"Do you have a (meaningful) website?","Are you currently in high school?",
"Have you ever participated in code guessing?",
"Have you ever had a LyricLy or gollark name derivative?","Are you from Europe?"
,"Have you ever developed for an embedded system?","Are you multilingual?",
"Is English your primary language?",""};typedef struct{char name[(7*(2*1+0)+6)+
(2*(1*1+0)+0)];unsigned char q[((sizeof(Questions)/sizeof(Questions[0]))-
(1*(1*1+0)+0))+(1*(1*1+0)+0)];}tdat;typedef struct{tdat info;float chance;}
character;character*Characters;unsigned int CharactersLen=0;tdat*TrainingDat;
unsigned int TrainingDatLen=0;unsigned char CurAns[((sizeof(Questions)/sizeof(
Questions[0]))-(1+0))+(0+1)];unsigned int Target=0;float chance(character*C){
unsigned int e=0;for(unsigned int i=0;i!=((sizeof(Questions)/sizeof(Questions[0]
))-(1*1+0));i++){if((CurAns[i]==0)||(C->info.q[i]==0))continue;if(CurAns[i]==C->
info.q[i])e++;else e=((int)e-(int)(((sizeof(Questions)/sizeof(Questions[0]))-
(1*(1*1+0)+0))/(1+1))<0)?0:e-(((sizeof(Questions)/sizeof(Questions[0]))-(1*1+0))
/(1+1));}return(C->chance=(e/((sizeof(Questions)/sizeof(Questions[0]))-
(1*(1*1+0)+0))));}unsigned char parseans(char a){switch(a){case'Y':case'y':case
'T':case't':return (1+1);case'N':case'n':case'F':case'f':return (1*1+0);default:
return 0;}}unsigned int highestchance(void){float curch=0;unsigned int h=0;for(
unsigned int c=0;c!=TrainingDatLen;c++){if(chance(&Characters[c])>curch){curch=
Characters[c].chance;h=c;}}return h;}void train(unsigned int qu,unsigned char an
){Target=highestchance();CurAns[qu]=an;}unsigned char loadchars(void){{char 
answers[((sizeof(Questions)/sizeof(Questions[0]))-(1+0))+(1*(2*1+0)+0)];FILE*f=
fopen(".td","r");if(f==NULL)return (1*(1*1+0)+0);unsigned int ent;for(ent=0;;ent
++){{char fmt[(20*1+0)];snprintf(fmt,(sizeof(fmt)/sizeof(fmt[0])),
"%%%u[^,],%%%u[^\n]\n",(unsigned int)((10+10)+(0+1)),(unsigned int)((sizeof(
Questions)/sizeof(Questions[0]))-(0+1))+(0+1));if(fscanf(f,fmt,TrainingDat[ent].
name,answers)==EOF)goto done;}if(ent>=TrainingDatLen)TrainingDat=(tdat*)realloc(
TrainingDat,((TrainingDatLen*=1.5)+(1*(1*1+0)+0))*sizeof(tdat));for(unsigned int
 i=0;i!=((sizeof(Questions)/sizeof(Questions[0]))-(1*1+0));i++)TrainingDat[ent].
q[i]=parseans(answers[i]);}done:TrainingDatLen=CharactersLen=ent;fclose(f);}
TrainingDat=(tdat*)realloc(TrainingDat,(TrainingDatLen+(0+1))*sizeof(tdat));
Characters=(character*)malloc((CharactersLen+(2*(1*1+0)+0))*sizeof(character));
for(unsigned int ent=0;ent!=TrainingDatLen;ent++){for(unsigned char i=0;i!=
(10+10);i++)Characters[ent].info.name[i]=TrainingDat[ent].name[i];for(unsigned 
int i=0;i!=((sizeof(Questions)/sizeof(Questions[0]))-(1+0));i++)Characters[ent].
info.q[i]=TrainingDat[ent].q[i];Characters[ent].chance=0;}return 0;}unsigned 
char savechars(void){FILE*f=fopen(".td","w");if(f==NULL)return (1*(1*1+0)+0);for
(unsigned int ent=0;ent!=CharactersLen;ent++){char answers[((sizeof(Questions)/
sizeof(Questions[0]))-(0+1))+(1*2+0)];for(unsigned int qu=0;qu!=((sizeof(
Questions)/sizeof(Questions[0]))-(1*1+0))+(1*1+0);qu++){switch(Characters[ent].
info.q[qu]){case (1+1):answers[qu]='T';break;case (1+0):answers[qu]='F';break;
case 0:answers[qu]='?';break;}}answers[((sizeof(Questions)/sizeof(Questions[0]))
-(1*1+0))+(0+1)]='\0';fprintf(f,"%s,%s\n",Characters[ent].info.name,answers);}
fclose(f);return 0;}unsigned int getquestion(void){unsigned int q;for(;;){while(
CurAns[(q=rand()%(((sizeof(Questions)/sizeof(Questions[0]))-(1*1+0))))]!=0);if((
rand()%(1+1))==0){if(Characters[Target].info.q[q]==0)return q;}else return q;}}
void init(void){free(TrainingDat);free(Characters);TrainingDat=(tdat*)malloc(((
TrainingDatLen=100)+(1*(1*1+0)+0))*sizeof(tdat));for(unsigned int i=0;i!=((
sizeof(Questions)/sizeof(Questions[0]))-(0+1));i++)CurAns[i]=0;srand(time(0));}
void deinit(void){free(TrainingDat);free(Characters);TrainingDatLen=0;
CharactersLen=0;}void init_td(void){const char initialcsv[]="(null),?";FILE*f=
fopen(".td","w");if(f==NULL)return;fprintf(f,"%s",initialcsv);fclose(f);}void 
trainchar(unsigned int c){for(unsigned int q=0;q!=((sizeof(Questions)/sizeof(
Questions[0]))-(1*(1*1+0)+0));q++){if(CurAns[q]!=0){if(TrainingDat[c].q[q]==0)
Characters[c].info.q[q]=CurAns[q];else if(TrainingDat[c].q[q]!=CurAns[q])
Characters[c].info.q[q]=0;}}}void insertchar(char*name){for(unsigned int c=0;c!=
CharactersLen;c++){if(!strcmp(Characters[c].info.name,name)){trainchar(c);return
;}}for(unsigned char i=0;i!=(15*1+5)+(1+0);i++)Characters[CharactersLen].info.
name[i]=name[i];for(unsigned int i=0;i!=((sizeof(Questions)/sizeof(Questions[0])
)-(1*(1*1+0)+0));i++)Characters[CharactersLen].info.q[i]=CurAns[i];CharactersLen
++;}int main(){begin:init();if(loadchars()){printf(
"Warning: Training data not found, should I create it? (Y/N)\t");if(parseans(
getchar())==(1+1)){init_td();goto begin;}else{puts("Aborting");exit((0+1));}}for
(unsigned char i=0;i!=(13*1+7);i++){unsigned int qu=getquestion();printf(
"%u\t%s (Y/N)\t",i+(1*(1*1+0)+0),Questions[qu]);unsigned char an=parseans(
getchar());while(getchar()!='\n');train(qu,an);}printf("Are you %s? ",Characters
[Target].info.name);if(parseans(getchar())==(1+1)){puts("Yay!");trainchar(Target
);}else{char name[(6*(3*1+0)+2)+(2*(1*1+0)+0)];char fmt[(12*1+8)];snprintf(fmt,(
sizeof(fmt)/sizeof(fmt[0])),"%%%u[^\n]",(unsigned int)((18*(1*1+0)+2)+(1*1+0)));
while(getchar()!='\n');do printf("Aww...\nWho are you, then? ");while(scanf(fmt,
name)==EOF);insertchar(name);}savechars();deinit();}
