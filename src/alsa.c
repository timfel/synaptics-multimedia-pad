#include "alsa.h"
 
#ifdef ALSA
int alsa_make_noise(const char* filename)
{
	static char *device = "default";                        /* playback device */
	snd_output_t *output = NULL;
    int err;
    unsigned int i;
    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
    FILE* rFile;
    long lSize;
    char* Buffer;
 
    if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) 
    {
    	printf("Playback open error: %s\n", snd_strerror(err));
    	return(err);
    }
    
    if ((err = snd_pcm_set_params(handle,
                                  SND_PCM_FORMAT_U8,
                                  SND_PCM_ACCESS_RW_INTERLEAVED,
                                  1,
                                  11025,
                                  1,
                                  0)) < 0) /* 5sec */
    {   
     	printf("Playback open error: %s\n", snd_strerror(err));
     	return(err);
    }
         
    rFile = fopen(filename, "r");
    #ifdef DEBUG
    	printf("Playing sound: %s\n", filename);fflush(stdout);
    #endif
    (void)fseek (rFile , 0 , SEEK_END);
    lSize = ftell(rFile);
    rewind(rFile);
    Buffer = (char*)malloc(sizeof(char)*lSize);
    if (Buffer != NULL) {
    	setbuf(rFile, Buffer);
		(void)fread(Buffer,1,(size_t)lSize,rFile);

    	(void)snd_pcm_writei(handle, Buffer, (size_t)lSize);
    } else {
    	#ifdef DEBUG
    		printf("Requested allocation size: %d \n", lSize);
    	#endif
    	printf("Not enough memory, file is too large!\n");
    	fflush(stdout);	
    }
    	
    (void)snd_pcm_close(handle);
    (void)fclose(rFile);
	return 0;
}

void alsa_set_volume(int percentage) 
{
	
}
#endif
