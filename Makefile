# ----------------------------------------------------------------------------
#
#  Copyright (C) 2003-2011 Michele Bussolotto <michelebussolotto@gmail.com>
#    
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# ----------------------------------------------------------------------------


PREFIX = /usr/local
SUFFIX := $(shell uname -m | sed -e 's/^unknown/$//' -e 's/^i.86/$//' -e 's/^x86_64/$/64/')
LIBDIR = lib$(SUFFIX)
DISTDIR = jack_tone-$(VERSION)
VERSION = 0.1.0
LDFLAGS += -L$(PREFIX)/$(LIBDIR)
CPPFLAGS += -O2 -Wall -MMD -MP -DVERSION=\"$(VERSION)\"
CPPFLAGS += -march=native

all:	jack_tone


JACK_TONE_O =	jack_tone.o
jack_tone : LDLIBS += -ljack -lasound -lrt
jack_tone:	$(JACK_TONE_O)
	g++ $(LDFLAGS) -o $@ $(JACK_TONE_O) $(LDLIBS)


install:	jack_tone
	install -m 755 jack_tone $(PREFIX)/bin


clean:
	/bin/rm -f *~ *.o *.a *.d *.so jack_tone

