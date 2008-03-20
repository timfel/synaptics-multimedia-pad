#include "config.h"

Config 
read_config(Config* config)
{
	Config std;
	std = *config;
	
	std.xmax = 980;
	std.ymax = 750;
	std.zmin = 30;
	std.triggerx = 960;
	std.triggery = 80;
	std.actsound = 1;
	std.soundname[0] = '\0';
	std.a1[0] = '\0';
	std.a2[0] = '\0';
	std.a3[0] = '\0';
	std.a4[0] = '\0';
	std.a5[0] = '\0';
	std.a6[0] = '\0';
	std.a7[0] = '\0';
	std.a8[0] = '\0';
	std.a9[0] = '\0';
	std.alsamixer[0] = '\0';
	char* actions[9] = {std.a1, std.a2, std.a3, std.a4, std.a5, std.a6, std.a7, std.a8, std.a9};
	
    char configfile[40] = {'\0'};
    FILE* cfile;
    char buffer[10] = {'\0'};
    char c;
    char buffer2[127] = {'\0'};
    int i;
    
    strcpy(configfile, getenv("HOME"));
    strcat(configfile, "/.medion-multimedia.cfg");
    printf("Configfile: %s \n", configfile);
    
    cfile = fopen((const char*)configfile, "r");
    
    if (cfile != NULL) {
        printf("%4s %4s %5d %5d %5d %5d %5d %5d %5d %5d %5d %5s %5s\n",
                "MaxX", "MaxY", 1,2,3,4,5,6,7,8,9, "Mixer", "Sound");
        for (i = 0; i < 17; i++) {
            (void)fscanf(cfile, "%s %s", buffer, buffer2);
            c = fgetc(cfile);
            while (!(c == CR || c == LF || c == EOF)) {
                strncat(buffer2, &c, 1);
                c = fgetc(cfile);
            }
            
            if (strstr(buffer, "xmax") != NULL)
                std.xmax = atoi(buffer2);
            else if (strstr(buffer, "ymax") != NULL)
                std.ymax = atoi(buffer2);
            else if (strstr(buffer, "zmin") != NULL)
                std.zmin = atoi(buffer2);
            else if (strstr(buffer, "sound") != NULL)
                std.actsound = atoi(buffer2);
            else if (strstr(buffer, "sndfile") != NULL)
                strcpy(std.soundname, buffer2);
            else if (strstr(buffer, "triggerx") != NULL)
                std.triggerx = atoi(buffer2);
            else if (strstr(buffer, "triggery") != NULL)
                std.triggery = atoi(buffer2);
            else if (strstr(buffer, "action") != NULL)
                strcpy(actions[atoi(&buffer[6])-1], buffer2);
            else if (strstr(buffer, "mixer") != NULL)
                strcpy(std.alsamixer, buffer2);
            else
                printf("Unmapped option in line %d. Check cfg!\n", i+1); 
        }
        printf("%3d %3d %5s %5s %5s %5s %5s %5s %5s %5s %5s %s %s\n",
                    std.xmax, std.ymax, std.a1, std.a2, 
                    std.a3, std.a4, std.a5, std.a6, 
                    std.a7, std.a8, std.a9,
                    std.alsamixer, std.soundname);
        fflush(stdout);
        
        return std;
    }
}
