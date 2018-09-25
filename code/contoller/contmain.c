/*2018/09/22 新入生工作用にシリアルで送るデータ構造を改編
 * 
 * 　ナベコンVer2.01
 * 仕様
 * XbeeとPIC16f1938を用いたLRボタン付き14ボタンコントローラー
 * ほぼ弄らなくてもよいが、どのボタンを同時押しするかで
 * 信号作成方法を変える必要がある
 * 下記に示すのはオムニorメカナムを用いた機体を想定している
 *  
 * 本プロラムとあまり関係ないが、
 * Teratermで信号を見るときはデバッグモードでないと正しく表示されないので注意
 * 
 * 2017/11/22記
 * 
 * 
 * Ver2にマイナーアップデートしました。
 * まあスイッチの位置変わったから定義しなおしただけよ
 * あとわい用にLRボタンの優先度を上げたンゴ
 * 
 * ボタン番号(簡易的にに示す)
 *      1               2
 *  8   3   7           11
 *  6       5       14      13
 *  10  4   9           12
 *    
 * 2017/12/10記
 */



#include <xc.h>
//スイッチのポートをマクロ化しているもの、回路により変更する
#define SW1  RA3
#define SW2  RA7
#define SW3  RC0
#define SW4  RB3
#define SW5  RC2
#define SW6  RC3
#define SW7  RA6
#define SW8  RC1
#define SW9  RC4
#define SW10 RB4
#define SW11 RA2
#define SW12 RB5
#define SW13 RA0
#define SW14 RA1

#define _XTAL_FREQ 8000000      //delay関数を使う場合に入れる、周波数を値におく(今回は4MHz)

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)


//送信用関数
void Send(unsigned char s){
    while(TXIF == 0) ;     // 送信可能になるまで待つ
    TXREG = s ;          // 送信する
}



void main(){
    unsigned char x,y,z,n,m,p,q;
    
    OSCCON = 0b01110010;        //ＰＩＣ内臓8ＭＨｚ駆動
    
    TRISA  = 0b11001111 ;       //作成回路要確認
    TRISB  = 0b00111000 ;
    TRISC  = 0b10001111 ; 
    PORTA  = 0 ;
    PORTB  = 0 ;
    PORTC  = 0 ; 
    
    ANSELA = 0b00000000;        //アナログは使わないけど設定は必要
    ANSELB = 0b00000000;
    
    // USART機能の設定を行う
    TXSTA  = 0b00100100 ;     // 送信情報設定：非同期モード　８ビット・ノンパリティ
    RCSTA  = 0b10010000 ;     // 受信情報設定
    SPBRG  = 25 ;            // ボーレートを19200(高速モード)に設定
    RCIF = 0 ;                // ＵＳＡＲＴ割込み受信フラグの初期化
    RCIE = 1 ;                // ＵＳＡＲＴ割込み受信を有効にする
    PEIE = 1 ;                // 周辺装置割込みを有効にする
    GIE  = 1 ;                // 全割込み処理を許可する
    
    p = 0 ;
    q = 0 ;
    n = 0 ;
    m = 0 ;
    
    while(1){
        
        //以下移動用なので同時押しを想定していないボタン達の設定
        //xは4bit
        if(SW3 == 1) x = 1;
        else if(SW4 == 1) x = 2;
        else if(SW5== 1) x = 3;
        else if(SW6 == 1) x = 4;
        else if(SW7 == 1) x = 5;
        else if(SW8 == 1) x = 6;
        else if(SW9 == 1) x = 7;
        else if(SW10 == 1) x = 8;
        else if(SW1 == 1) x = 9;
        else if(SW2 == 1) x = 10;
        else x = 0;
        
        //yは3bit
        if(SW11 == 1) y = 1;
        else if(SW12 == 1) y = 2;
        else if(SW13 == 1) y = 3;
        else if(SW14 == 1) y = 4;
        else y = 0;
        
        
        //信号を作成
        //別途”n”を用意したのは何となく(すべて z にまとめても良いと思う)
        x = x << 3;
        n = x + y;

        //信号に変化があった場合のみ送信
        if(n != p){
            Send(n);
            __delay_ms(5);
        }
        
        p = n;      //信号の変化記録用
        
        m = x + y + 128 ;
        //頭（７bit目に１を加える）
        
        //ノイズ対策の為に２回信号を送る
        if(m != q){
            Send(m);
            __delay_ms(5);
        }
        
        q = m;      //信号の変化記録用
        
    }
}


/*
 * スイッチの状態が変化した後、一度のみ信号を送信している。
 * これは、送信する信号をなるべく少なくするためである。多すぎるとXbee側（UART通信）が間に合わず固まる
 * 文字列を送信することもできるが、長すぎると通信速度や内部クロックなどの影響により、通信にラグが生じる。
 * よって、文字数はなるべく減らすことを考えるとうまくいきやすい。
 */