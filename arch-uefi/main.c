#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main() {
    char username[50];
    char password[50];
    int option_os;
    char input[50];
    char splash[256];
    char touch_filename[50];
    char ls[256];
    char echo[256];
    printf("GRUB\n");
    printf("1 : Ministros OS\n");
    printf("Choose Operating System To Boot\n");
    scanf("%d",&option_os);
    getchar();
    if (option_os == 1) {
    sleep(1);
    printf("Booting...\n");
    FILE *file = fopen("splash.txt","r");
    if (file == NULL) {
    printf("BOOT ERROR : SPLASH NOT FOUND\n");
    
    }
    else{
    while (fgets(splash,sizeof(splash),file)) {
    printf("%s",splash);
    }
    fclose(file);
    }
    
    printf("OS\n");
    printf("Username\n");
    fgets(username,sizeof(username),stdin);
    username[strcspn(username,"\n")] = '\0';
    printf("Password\n");
    fgets(password,sizeof(password),stdin);
    password[strcspn(password,"\n")] = '\0';
    
    while (1) {
        printf("%s@ministros [~] $> ",username);
        fgets(input,sizeof(input),stdin);
        input[strcspn(input,"\n")] = '\0';
        if (strcmp(input,"whoami") == 0) {
            printf("%s\n",username);
        } 
        
        else if (strcmp(input,"ls") == 0) {
            FILE *touch_file = fopen("touch.db","r");
        if (touch_file == NULL) {
            printf("Touch DB Error : 404 FILE NOT FOUND\n");
        }
        else{
            while (fgets(ls,sizeof(ls),touch_file)) {
            ls[strcspn(ls,"\n")] = '\0';
            printf("%s\n",ls);
            }
             fclose(touch_file);
        }
       
      }
     else if (strcmp(input,"sudo pacman -Syu") == 0) {
        printf("Updating Ministros OS\n");
        sleep(1);
         printf("Updating Ministros OS\n");
         sleep(1);
        printf("Updating Ministros OS\n");
        sleep(1);
        printf("Ministros OS Is Updated\n");
     }
     else if (strcmp(input,"sysinfo") == 0) {
        printf("CPU : INTEL CORE ULTRA 9 285K\n");
        printf("GPU : NVIDIA GEFORCE RTX 5090\n");
        printf("RAM : 64GB DDR5\n");
        printf("Storage : 2TB NVME M.2 SSD\n");
     }
     else if (strcmp(input,"--version") == 0) {
        printf("Ministros OS v1.4\n");
        
     }
     else if (strcmp(input,"shutdown now") == 0 || strcmp(input,"exit") == 0) {
        printf("Shutting down..\n");
        break;
     }     
     else if (strcmp(input,"ipconfig") == 0) {
        printf("192.168.0.1\n");
     }
     else if (strcmp(input,"ip a") == 0) {
        printf("192.168.0.1\n");
     }
     else if (strcmp(input,"getmac") == 0) {
        printf("00:11:22:33:44:55\n");
     }
    else{
        printf("Command Not Found\n");
    }
    
    }
    }
    else{
        printf("Boot Option Not Found\n");
    }
    return 0;
}