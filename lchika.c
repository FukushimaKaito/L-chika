/*--------------------------------------------------------------
CON2に接続したアドオンモジュールDA00を操作し、接続したLEDを1秒ごとに点滅させるプログラム
20160928 ****
--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
        while(1)
        {
                // check
                if(system("date") == -1){
                        perror("system");
                }
                // 権限のいる領域なのでこの表記。コマンドラインに表示しないようにnull出力
                if(system("echo 1 | sudo tee /sys/class/gpio/DO1_CON2/value > /dev/null") == -1) {
                        perror("system");
                }
                //wait 1sec
                sleep(1);
                // check
                if(system("date") == -1){
                        perror("system");
                }
                if(system("echo 0 | sudo tee /sys/class/gpio/DO1_CON2/value > /dev/null")==-1){
                        printf("Error\n");
                }
                //wait 1sec
                sleep(1);
        }
        return 0;
}
