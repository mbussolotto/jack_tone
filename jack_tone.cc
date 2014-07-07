// ----------------------------------------------------------------------------
//
//  Copyright (C) 2003-2011 Michele Bussolotto <michelebussolotto@gmail.com>
//    
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// ----------------------------------------------------------------------------


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include <jack/jack.h>
#include <signal.h>
#include <errno.h>

#include "jack_tone.h"



static jack_client_t  *jack_handle;
static jack_port_t    *jack_play;
static const char     *jack_name;
static bool            active = false;
static double p = 0;



int jack_callback (jack_nframes_t nframes, void *arg)
{
    float *op;

    if (active)
    {

        op = (float *)(jack_port_get_buffer (jack_play, nframes));
        process (nframes, op);
    }
    return 0;
}


static char *options = (char *)"hf:a:o:";


static void help (void)
{
    fprintf (stderr, "\njack_tone %s\n", VERSION);
    fprintf (stderr, "Generate sine wave with an offset value.\n");
    fprintf (stderr, "Usage: jack_tone <options>\n");
    fprintf (stderr, "  -f  sine frequency in Hz [1000]\n");
    fprintf (stderr, "  -a  sine amplitude [1]\n");
    fprintf (stderr, "  -o  offset [0]\n");
    exit (1);
}


static void procoptions (int ac, char *av [], const char *where)
{
    int k;
    
    optind = 1;
    opterr = 0;

    while ((k = getopt (ac, av, options)) != -1)
    {
        if (optarg && (*optarg == '-'))
        {
            fprintf (stderr, "\n%s\n", where);
	    fprintf (stderr, "  Missing argument for '-%c' option.\n", k); 
            fprintf (stderr, "  Use '-h' to see all options.\n");
            exit (1);
        }
	switch (k)
	{
        case 'h' :
            help ();
            exit (0);
 	case 'f' :
	    frequency = atof(optarg);
            break;
 	case 'a' :
	    amp = atof(optarg);
            break;
 	case 'o' :
	    offset = atof(optarg);
            break;
        default:
            abort ();
 	}
    }
}


int main (int ac, char *av [])
{
    jack_status_t  s;
    frequency = 1000;
    offset = 0;
    amp = 0.99;
    p = frequency;
    procoptions (ac, av, "On command line:");

    if( (amp + offset) > 1){
        fprintf (stderr, "Offset plus amplitude must be <= 0.99\n");
	exit(0);
    }
    jack_handle = jack_client_open ("jack_tone", JackNoStartServer, &s);
    if (jack_handle == 0)
    {
        fprintf (stderr, "Can't connect to Jack, is the server running ?\n");
        return 1;
    }

    jack_set_process_callback (jack_handle, jack_callback, 0);
    jack_on_shutdown(jack_handle, jack_shutdown, 0);
    if (jack_activate (jack_handle))
    {
        fprintf(stderr, "Can't activate Jack");
        return 1;
    }

    jack_name = jack_get_client_name (jack_handle);
    jack_play = jack_port_register (jack_handle, "out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    totalLen = jack_get_buffer_size (jack_handle);
    fsamp = jack_get_sample_rate(jack_handle);

    active = true;
    while (1)
    {
	usleep (250000);
    }

    return 0;
}

int process (size_t len, float *op)
{
    float   a,s;
    float sinValue;

    while (len--)
    {
	//printf("%d\n",len);
	a = 2 * (float) M_PI * p; 
	p += (double)((double)frequency/(double)fsamp);
	sinValue =  - sinf (a);
	s = offset + amp*sinValue;
	if(sinValue < 0.000001 && sinValue > (-0.000001) ){
	  p = frequency;
	}
	*op++ = s;
    }
    return 0;
}

static void jack_shutdown(void *arg)
{
  printf("The Jack Server was shut down!\n");
  exit (1);
}
