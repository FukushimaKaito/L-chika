/*------------------------------------------------------------
割込み処理を用いたLED点滅
20160930 ****

権限がないのでできないときはスーパーユーザになってから実行するか
事前にchomodで/sys/class/gpio/DO1_CON2/valueに対して権限追加する。
-------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

//タイマーハンドら―
void timer_handler(int);

int main(void)
{
        struct sigaction act, oldact;
        timer_t tid;
        struct itimerspec itval;

        //メモリ領域確保
        memset(&act, 0, sizeof(struct sigaction));
        memset(&oldact, 0, sizeof(struct sigaction));

        //初期化
        act.sa_handler = timer_handler;
        act.sa_flags = SA_RESTART;
        //シグナル設定
        if(sigaction(SIGALRM, &act, &oldact) < 0) {
                perror("sigaction");
                return -1;
        }
        //timer 1st-1sec next-1sec
        itval.it_value.tv_sec = 1;
        itval.it_value.tv_nsec = 0;
        itval.it_interval.tv_sec = 1;
        itval.it_interval.tv_nsec = 0;

        //タイマー作成
        if(timer_create(CLOCK_REALTIME, NULL, &tid) != 0) {
                perror("timer_create");
                if(timer_delete(tid) != 0){
                        perror("timer_delete");
                }
                if(sigaction(SIGALRM, &oldact, NULL) != 0){
                        perror("sigaction");
                }
                return -1;
        }
        //タイマー設定
        if(timer_settime(tid, 0, &itval, NULL) < 0) {
                perror("timer_settime");
                if(timer_delete(tid) != 0){
                        perror("timer_delete");
                }
                if(sigaction(SIGALRM, &oldact, NULL) != 0){
                        perror("sigaction");
                }
                return -1;
        }

        //冗長処理
        while(1){
        }

        //解放
        if(timer_delete(tid) != 0){
                perror("timer_delete");
        }
        if(sigaction(SIGALRM, &oldact, NULL) != 0){
                perror("sigaction");
        }

        return 0;
}

// 時間が来たらする処理 使える関数に注意
void timer_handler(int signum)
{
        char buf[]="0";
        int fp;

        //LED open
        if((fp=open("/sys/class/gpio/DO1_CON2/value",O_RDWR))==-1){
                perror("Error");
        }

        //LED 1read
        if(read(fp,buf,sizeof(buf)) == -1){
                perror("read");
        }

        //light unlight
        if(buf[0]=='0'){
                buf[0]='1';
        }
        else{
                buf[0]='0';
        }

        if(write(fp, buf, sizeof(buf)) == -1){
                perror("write");
        }

        close(fp);
}
