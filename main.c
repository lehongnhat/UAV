//#include<xc.h>
#include<p33FJ256MC710.h>
#include "UART.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

_FOSCSEL(FNOSC_PRI & IESO_OFF)
_FOSC(POSCMD_HS & OSCIOFNC_ON & FCKSM_CSDCMD)
_FWDT(WINDIS_OFF & FWDTEN_OFF)

char a[81];
char lat[12], buf[12];
char clat2[10],clat3[12],clon2[11],clon3[12],calt2[12],calt3[12];
unsigned int k = 0,j = 0 ,h = 0;
int R = 6371;// don vi km
char kc12[11],kc13[11],kc23[11],gocPan[11],gocTilt[11];

double calDistance(double lon1, double lat1, double lon2, double lat2);
double calAngle(double a, double b, double c);
double calPalAngle(double lon1, double lat1, double lon2, double lat2, double lon3, double lat3);
double calTiltAngle(double canh12, double canh13, double alt1, double alt2, double alt3);
void doubleToCharAndSendUART2(double a);
double charToDouble(char *String);

int main(int argc, char** argv) {

    double lat1,lon1,alt1,lat2,lon2,alt2,lat3,lon3,alt3,canh12,canh13, panAngle, tiltAngle,f;
    
    unsigned int i = 0, count = 0,u = 0;    
    
    lat1 = 0;
    lon1 = 0;
    alt1 = 0;
    lat2 = -4;
    lon2 = 4;
    alt2 = 12000;
    lat3 = -4;
    lon3 = -4;
    alt3 = 15000;
    
    TRISFbits.TRISF1 = 0;
    PORTFbits.RF1 = 1;      //led debug
    
    OpenUART2();
    
    panAngle = calPalAngle(lon1,lat1,lon2,lat2,lon3,lat3);
    canh12 = calDistance(lon1, lat1, lon2, lat2);
    canh13 = calDistance(lon1, lat1, lon3, lat3);
    tiltAngle = calTiltAngle(canh12, canh13, alt1, alt2, alt3);
    
    //gui qua uart

    PrintStringUART2("Goc pan = ");
    doubleToCharAndSendUART2(panAngle);
    WriteCharToUART2(10);
    WriteCharToUART2(13);
    PrintStringUART2("Goc tilt = ");
    doubleToCharAndSendUART2(tiltAngle);
    WriteCharToUART2(10);
    WriteCharToUART2(13);    
    
    while(1)
    {
        if (h == 1){
            PrintStringUART2("_____________________");
            WriteCharToUART2(10);
            WriteCharToUART2(13);
            PrintStringUART2(a);
            WriteCharToUART2(10);
            WriteCharToUART2(13);

            lat3 = lat2;
            lon3 = lon2;
            alt3 = alt2;
            for(i = 0; i< 10; i++)
            {
                clat2[i] = a[i+16];           
            }
            for(i = 0; i< 11; i++)
            {
                clon2[i] = a[i+29];           
            }
            for(i = 0;i <strlen(a);i++){
                if(a[i] == ','){
                    count++;
                }
                if(count==9){
                    if(u == 0) {
                        i++;
                    }
                    calt2[u] = a[i];
                    u++;
                    }
            }
            count=0;
            u=0;
            PrintStringUART2(clat2);WriteCharToUART2(a[27]);
            PrintStringUART2(",");
            PrintStringUART2(clon2);WriteCharToUART2(a[41]);
            PrintStringUART2(",");
            PrintStringUART2(calt2);
            WriteCharToUART2(10);
            WriteCharToUART2(13);
            lat2 = charToDouble(clat2);
            lon2 = charToDouble(clon2);
            alt2 = charToDouble(calt2);
            
            lat2 = (lat2 - ((int)lat2/100) * 100)/60 +((int)lat2/100);
            lon2 = (lon2 - ((int)lon2/100) * 100)/60 +((int)lon2/100);
            
            if(a[27] == 'S'){
                lat2 = lat2 * (-1);
            }
            if(a[41] == 'W'){
                lon2 = lon2 * (-1);
            }
            
            PrintStringUART2("flat2 = ");
            doubleToCharAndSendUART2(lat2);
            WriteCharToUART2(10);
            WriteCharToUART2(13);
            PrintStringUART2("flon2 = ");
            doubleToCharAndSendUART2(lon2);
            WriteCharToUART2(10);
            WriteCharToUART2(13);
            PrintStringUART2("falt2 = ");
            doubleToCharAndSendUART2(alt2);
            WriteCharToUART2(10);
            WriteCharToUART2(13);
            PrintStringUART2("flat3 = ");
            doubleToCharAndSendUART2(lat3);
            WriteCharToUART2(10);
            WriteCharToUART2(13);
            PrintStringUART2("flon3 = ");
            doubleToCharAndSendUART2(lon3);
            WriteCharToUART2(10);
            WriteCharToUART2(13);
            PrintStringUART2("falt3 = ");
            doubleToCharAndSendUART2(alt3);
            WriteCharToUART2(10);
            WriteCharToUART2(13);
            
            panAngle = calPalAngle(lon1,lat1,lon2,lat2,lon3,lat3);
            canh12 = calDistance(lon1, lat1, lon2, lat2);
            canh13 = calDistance(lon1, lat1, lon3, lat3);
            tiltAngle = calTiltAngle(canh12, canh13, alt1, alt2, alt3);
    
            //gui qua uart

            PrintStringUART2("Goc pan = ");
            doubleToCharAndSendUART2(panAngle);
            WriteCharToUART2(10);
            WriteCharToUART2(13);
            PrintStringUART2("Goc tilt = ");
            doubleToCharAndSendUART2(tiltAngle);
            WriteCharToUART2(10);
            WriteCharToUART2(13);
            
            h = 0;
        }
        
    }
    return 0;

    //return (EXIT_SUCCESS);
}

//void __attribute__((__interrupt__)) _U1TXInterrupt(void){
//    IFS0bits.U1TXIF = 0;
//}
//
//void __attribute__((__interrupt__)) _U1RXInterrupt(void){
//    unsigned char t;
//    IFS0bits.U1RXIF = 0;
//    t = ReadCharUART1();
//    if(t=='$'){
//        j = 1;
//        i = 0;
//        for(d=0;d<70;d++){
//            a[d] = ' ';
//        }
//    }
//    if(t=='*'){
//        j = 0;
//        h = 1;
//    }
//    if(j){
//        if(t!='$'){
//            a[i++] = t;
//        }
//    }
//      
//}

void __attribute__((__interrupt__)) _U2TXInterrupt(void){
    IFS1bits.U2TXIF = 0;
}

void __attribute__((__interrupt__)) _U2RXInterrupt(void){
    unsigned int d = 0;
    char t;
    IFS1bits.U2RXIF = 0;
    t = ReadCharUART2();
    if((t=='$')&&(h == 0)){
        j = 1;
        k = 0;
        for(d=0;d<11;d++){
            a[d] = ' ';
        }
    }
    if(t=='*'){
        j = 0;
        if((a[2] == 'G')&&(a[3]=='G')&&(a[4]=='A')){
            h = 1;        
        }
    }
    if(j){
        if(t!='$'){
            a[k++] = t;            
        }
    }
      
}

double calDistance(double lon1, double lat1, double lon2, double lat2){
    double rlat1,rlat2,rlon1,rlon2,dlon,dlat,b,c,f;
    rlat1 = lat1/180*3.14;
    rlon1 = lon1/180*3.14;
    rlat2 = lat2/180*3.14;
    rlon2 = lon2/180*3.14;
    dlon = fabs(rlon2 - rlon1);
    dlat = fabs(rlat2 - rlat1);
    b = pow(sin(dlat/2),2) + cos(rlat1)*cos(rlat2)*pow(sin(dlon/2),2);
    c = 2 * asinf(sqrt(b));
    f = 6367*c;
    return f;
}

double calAngle(double a, double b, double c){
    double Angle; //rad
    Angle = acos((a*a+b*b-c*c)/2/a/b);
    return Angle;
}

double calPalAngle(double lon1, double lat1, double lon2, double lat2, double lon3, double lat3){
    double panAngle = 0;
    double lon4,lat4;
    double canh12,canh13,canh14,canh24,canh34,gocMoi, gocCu;    

    //tinh toa do diem thu 4
    lon4 = lon1;
    lat4 = lat1 +0.25;
    //tinh goc moi va goc cu
    canh12 = calDistance(lon1, lat1, lon2, lat2);
    canh13 = calDistance(lon1, lat1, lon3, lat3);
    canh14 = calDistance(lon1, lat1, lon4, lat4);
    canh24 = calDistance(lon2, lat2, lon4, lat4);
    canh34 = calDistance(lon3, lat3, lon4, lat4);
    if(lon1 ==lon2){
        if(lat2>=lat1){
            gocMoi = 0;
        }else{
            gocMoi = 3.14;
        }
    }
    else{
    gocMoi = calAngle(canh12,canh14,canh24);
    }
    if(lon1 ==lon3){
        if(lat3>=lat1){
            gocCu = 0;
        }else{
            gocCu = 3.14;
        }
    }
    else{
    gocCu  = calAngle(canh13,canh14,canh34);
    }
//    snprintf(sbuf,10,"%f",canh12);

    PrintStringUART2("Canh12 = ");
    doubleToCharAndSendUART2(canh12);
    WriteCharToUART2(10);
    WriteCharToUART2(13);
    PrintStringUART2("Canh13 = ");
    doubleToCharAndSendUART2(canh13);
    WriteCharToUART2(10);
    WriteCharToUART2(13);
    PrintStringUART2("Canh14 = ");
    doubleToCharAndSendUART2(canh14);
    WriteCharToUART2(10);
    WriteCharToUART2(13);
    PrintStringUART2("Canh24 = ");
    doubleToCharAndSendUART2(canh24);
    WriteCharToUART2(10);
    WriteCharToUART2(13);
    PrintStringUART2("Canh34 = ");
    doubleToCharAndSendUART2(canh34);
    WriteCharToUART2(10);
    WriteCharToUART2(13);
    PrintStringUART2("Goc moi = ");
    doubleToCharAndSendUART2(gocMoi);
    WriteCharToUART2(10);
    WriteCharToUART2(13);
    PrintStringUART2("Goc cu = ");
    doubleToCharAndSendUART2(gocCu);
    WriteCharToUART2(10);
    WriteCharToUART2(13);
    
    //kiem tra vi tri tuong doi so voi PTU, 2 la vi tri moi, 3 la vi tri cu
    //TH1:cung nam ben phai PTU
    if((lon3>=lon1)&&(lon2>=lon1)){
        panAngle = (gocMoi - gocCu)/3.14*180;
        if (panAngle >= 0){
            //printf("1quay phai goc %f",fabs(panAngle));
            PrintStringUART2("1) quay phai goc ");
            doubleToCharAndSendUART2(fabs(panAngle));
            WriteCharToUART2(10);
            WriteCharToUART2(13);
        }else{
            //printf("1quay trai goc %f",fabs(panAngle));
            PrintStringUART2("1) quay trai goc ");
            doubleToCharAndSendUART2(fabs(panAngle));
            WriteCharToUART2(10);
            WriteCharToUART2(13);
        }
    }
    //TH2:cung nam ben trai PTU
    else if((lon3<=lon1)&&(lon2<=lon1)){
        panAngle = (gocMoi - gocCu)/3.14*180;
        if (panAngle >= 0){
            //printf("2quay trai goc %f",fabs(panAngle));
            PrintStringUART2("2) quay trai goc ");
            doubleToCharAndSendUART2(fabs(panAngle));
            WriteCharToUART2(10);
            WriteCharToUART2(13);
        }else{
            //printf("2quay phai goc %f",fabs(panAngle));
            PrintStringUART2("2) quay phai goc ");
            doubleToCharAndSendUART2(fabs(panAngle));
            WriteCharToUART2(10);
            WriteCharToUART2(13);
        }
    }
    //TH3:nam 2 ben PTU
    else{
        panAngle = (gocMoi+gocCu)/3.14*180;
        if ((lon2>lon1)&&(lon3<lon1)){
            if(panAngle<=180){
                //printf("3quay phai goc %f",panAngle);
                PrintStringUART2("3) quay phai goc ");
                doubleToCharAndSendUART2(panAngle);
                WriteCharToUART2(10);
                WriteCharToUART2(13);
            }else{
                //printf("3quay trai goc %f",(360-panAngle));
                PrintStringUART2("3) quay trai goc ");
                doubleToCharAndSendUART2((360 - panAngle));
                WriteCharToUART2(10);
                WriteCharToUART2(13);
            }
        }else if((lon2<lon1)&&(lon3>lon1)){
            if(panAngle<180){
                //printf("4quay trai goc %f",panAngle);
                PrintStringUART2("4) quay trai goc ");
                doubleToCharAndSendUART2(panAngle);
                WriteCharToUART2(10);
                WriteCharToUART2(13);
            }else{
                //printf("4quay phai goc %f",(360-panAngle));
                PrintStringUART2("4) quay phai goc ");
                doubleToCharAndSendUART2((360-panAngle));
                WriteCharToUART2(10);
                WriteCharToUART2(13);
            }
        }
    }

    return panAngle;
}

double calTiltAngle(double canh12, double canh13, double alt1, double alt2, double alt3){
    double TiltAngle; //don vi do
    double cao1,cao2,goc1,goc2;// goc 1 la vi tri moi, goc 2 la vi tri cu
    if((alt1>alt2)||(alt1>alt3)) return 0;
    cao1 = alt2-alt1;
    cao2 = alt3-alt1;
    goc1 = atan(cao1/canh12/1000);
    goc2 = atan(cao2/canh13/1000);
    TiltAngle = (goc1-goc2)/3.14*180;
    if(TiltAngle>=0){
        PrintStringUART2("5) quay len goc ");
        doubleToCharAndSendUART2(fabs(TiltAngle));
        WriteCharToUART2(10);
        WriteCharToUART2(13);
    }else{
        PrintStringUART2("5) quay xuong goc ");
        doubleToCharAndSendUART2(fabs(TiltAngle));
        WriteCharToUART2(10);
        WriteCharToUART2(13);
    }
    return TiltAngle;
}

void doubleToCharAndSendUART2(double a){
    char sbuf[12];
    snprintf(sbuf,11,"%f",a);    
    PrintStringUART2(sbuf);
}

double charToDouble(char *String){
    //char str[] = "12345.56";
    double d = 0;
    sscanf(String, "%lf", &d);
    return d;
}