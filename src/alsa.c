 /*
  *  This extra small demo sends a random samples to your speakers.
  */
 
#include "alsa.h"
 
int make_noise(const char* filename)
{
	static char *device = "default";                        /* playback device */
	snd_output_t *output = NULL;
    int err;
    unsigned int i;
    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
    FILE* rFile;
    long lSize;
    char Buffer[1024*1024] = {'\0'};
 
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
                                  2,
                                  0)) < 0) /* 5sec */
    {   
     	printf("Playback open error: %s\n", snd_strerror(err));
     	return(err);
    }
         
    rFile = fopen("/home/tim/ready.wav", "r");
    (void)fseek (rFile , 0 , SEEK_END);
    lSize = ftell(rFile);
    rewind(rFile);
    setbuf(rFile, Buffer);
	(void)fread(Buffer,1,(size_t)lSize,rFile);

    (void)snd_pcm_writei(handle, Buffer, sizeof(Buffer));
    (void)snd_pcm_close(handle);
    (void)fclose(rFile);
	return 0;
}

void set_volume(int percentage) 
{
	
}
