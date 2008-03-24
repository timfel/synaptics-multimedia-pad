#include "common.h"

#ifdef GSTREAMER
#include <gst/gst.h>
#include "gstreamer.h"

/*
 * Global objects are usually a bad thing. For the purpose of this
 * example, we will use them, however.
 */

GstElement *decoder;

static gboolean
bus_call (GstBus     *bus,
	  GstMessage *msg,
	  gpointer    data)
{
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_EOS:
      #ifdef DEBUG
        g_print ("End-of-stream\n");
      #endif
      g_main_loop_quit (loop);
      break;
    case GST_MESSAGE_ERROR: {
      gchar *debug;
      GError *err;

      gst_message_parse_error (msg, &err, &debug);
      g_free (debug);

      g_print ("Error: %s\n", err->message);
      g_error_free (err);

      g_main_loop_quit (loop);
      break;
    }
    default:
      break;
  }

  return TRUE;
}

static void
new_pad (GstElement *element,
	 GstPad     *pad,
	 gpointer    data)
{
  GstPad *sinkpad;
  /* We can now link this pad with the audio decoder */
  #ifdef DEBUG
    g_print ("Dynamic pad created, linking parser/decoder\n");
  #endif

  sinkpad = gst_element_get_pad (decoder, "sink");
  gst_pad_link (pad, sinkpad);

  gst_object_unref (sinkpad);
}

int
gstreamer_play (char* filename)
{
  GMainLoop *loop;
  GstBus *bus;
  GstElement *pipeline, *source, *parser, *conv, *sink;

  char* decoderstr;
  char* demuxstr;

  /* initialize GStreamer */
  gst_init (NULL, NULL);
  loop = g_main_loop_new (NULL, FALSE);
  #ifdef DEBUG
    printf("Trying to play %s.\n", filename);fflush(stdout);
  #endif

  if (strstr(filename, ".ogg") != NULL) {
	decoderstr = (char*)malloc(sizeof(char)*10);
	strcpy(decoderstr, "vorbisdec");
	demuxstr = (char*)malloc(sizeof(char)*9);
	strcpy(demuxstr, "oggdemux");
  } else if (strstr(filename, ".mp3") != NULL) {
	decoderstr = (char*)malloc(sizeof(char)*10);
	strcpy(decoderstr, "ffdec_mp3");
	demuxstr = (char*)malloc(sizeof(char)*12);
	strcpy(demuxstr, "ffdemux_mp3");
  } else if (strstr(filename, ".wma") != NULL) {
	decoderstr = (char*)malloc(sizeof(char)*12);
	strcpy(decoderstr, "ffdec_wmav2");
	demuxstr = (char*)malloc(sizeof(char)*14);
	strcpy(demuxstr, "ffdemux_asf");
  } else if (strstr(filename, ".wav") != NULL) {
	demuxstr = (char*)malloc(sizeof(char)*9);
	strcpy(demuxstr, "wavparse");
  } else {
	return -1;
  }

  /* create elements */
  pipeline = gst_pipeline_new ("audio-player");
  source = gst_element_factory_make ("filesrc", "file-source");
  parser = gst_element_factory_make (demuxstr, "parser");
  decoder = gst_element_factory_make (decoderstr, "decoder");
  conv = gst_element_factory_make ("audioconvert", "converter");
  sink = gst_element_factory_make ("alsasink", "alsa-output");
  if (!pipeline || !source || !parser || !conv || !sink) {
    #ifdef DEBUG
      g_print ("One element could not be created\n");
      if (!parser)
	g_print ("Check your demux\n");
    #endif
    return -1;
  }
  if (strstr(filename, ".wav") == NULL) {
      if (!decoder)
	g_print ("Check your decoder\n");
      return -1;
  }


  /* set filename property on the file source. Also add a message
   * handler. */
  g_object_set (G_OBJECT (source), "location", filename, NULL);

  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  gst_bus_add_watch (bus, bus_call, loop);
  gst_object_unref (bus);

  /* put all elements in a bin */
  if (strstr(filename, ".wav") == NULL)
	  gst_bin_add_many (GST_BIN (pipeline),
		    source, parser, decoder, conv, sink, NULL);
  else
	  #ifdef DEBUG
		g_print("Playing *.wav");
	  #endif
	  gst_bin_add_many (GST_BIN (pipeline),
		    source, parser, conv, sink, NULL);

  /* link together - note that we cannot link the parser and
   * decoder yet, becuse the parser uses dynamic pads. For that,
   * we set a pad-added signal handler. */
  gst_element_link (source, parser);
  gst_element_link_many (decoder, conv, sink, NULL);
  g_signal_connect (parser, "pad-added", G_CALLBACK (new_pad), NULL);

  /* Now set to playing and iterate. */
  #ifdef DEBUG
    g_print ("Setting to PLAYING\n");
  #endif
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  #ifdef DEBUG
    g_print ("Running\n");
  #endif
  g_main_loop_run (loop);

  /* clean up nicely */
  #ifdef DEBUG
    g_print ("Returned, stopping playback\n");
  #endif
  gst_element_set_state (pipeline, GST_STATE_NULL);
  #ifdef DEBUG
    g_print ("Deleting pipeline\n");
  #endif
  gst_object_unref (GST_OBJECT (pipeline));

  return 0;
}
#endif
