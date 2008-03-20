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
    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *hw_params;
    FILE* rFile;
    long lSize;
    char Buffer[1024*1024] = {'\0'};
 
    if ((err = snd_pcm_open (&playback_handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
			fprintf (stderr, "cannot open audio device (%s)\n", 
				 snd_strerror (err));
			exit (1);
		}
		   
		if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
			fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
				 
		if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
			fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
			fprintf (stderr, "cannot set access type (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
			fprintf (stderr, "cannot set sample format (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, 11025, 0)) < 0) {
			fprintf (stderr, "cannot set sample rate (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, 1)) < 0) {
			fprintf (stderr, "cannot set channel count (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
			fprintf (stderr, "cannot set parameters (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		snd_pcm_hw_params_free (hw_params);
	
		if ((err = snd_pcm_prepare (playback_handle)) < 0) {
			fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
         
    rFile = fopen("/home/tim/ready.wav", "r");
    (void)fseek (rFile , 0 , SEEK_END);
    lSize = ftell(rFile);
    rewind(rFile);
    setbuf(rFile, Buffer);
	(void)fread(Buffer,1,(size_t)lSize,rFile);
	(void)fclose(rFile);
         
    (void)snd_pcm_writei(playback_handle, Buffer, sizeof(Buffer));
    (void)snd_pcm_close(playback_handle);
	return 0;
}
