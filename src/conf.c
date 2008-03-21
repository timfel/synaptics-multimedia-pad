#include "conf.h"


Config 
conf_read(Config* config)
{
	Config std;
	std = *config;
	
	std.xmax = 980;
	std.ymax = 750;
	std.zmin = 30;
	std.triggerx = 960;
	std.triggery = 80;
	std.actsound = 1;
	std.soundon[0] = '\0';
	std.soundoff[0] = '\0';
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
		#ifdef DEBUG
        printf("%4s %4s %5d %5d %5d %5d %5d %5d %5d %5d %5d %5s %2s %10s %10s\n",
                "MaxX", "MaxY", 1,2,3,4,5,6,7,8,9, "Mixer", "Snd", "SoundOn", "SoundOff");
    	#endif
        for (i = 0; i < 18; i++) {
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
            else if (strstr(buffer, "sndon") != NULL)
                strcpy(std.soundon, buffer2);
            else if (strstr(buffer, "sndoff") != NULL)
                strcpy(std.soundoff, buffer2);
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
        
        #ifndef ALSA
			std.actsound = 0;
		#endif
        
        #ifdef DEBUG
        printf("%3d %3d %5s %5s %5s %5s %5s %5s %5s %5s %5s %s %2d %s %s\n",
                    std.xmax, std.ymax, std.a1, std.a2, 
                    std.a3, std.a4, std.a5, std.a6, 
                    std.a7, std.a8, std.a9,
                    std.alsamixer, std.actsound, std.soundon, std.soundoff);
        #endif
        fflush(stdout);
        
        return std;
    }
}
