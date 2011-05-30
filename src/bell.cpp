#include "bell.h"
#include <core/atoms.h>


#include <canberra.h>
#include <stdlib.h>

COMPIZ_PLUGIN_20090315 (bell, BellPluginVTable);

void
AudibleBell::handleEvent (XEvent *event)
{
    if (event->type == screen->xkbEvent ())
    {
		XkbAnyEvent *xkbEvent = (XkbAnyEvent *) event;

		if (xkbEvent->xkb_type == XkbBellNotify)
		{
			XkbBellNotifyEvent *xkbBellEvent = (XkbBellNotifyEvent *) xkbEvent;

			if (optionGetAudibleBell ())
			{
			    ca_context *c;
    int ret;

    ret = ca_context_create(&c);


    /* Initialize a few meta variables for the following play()
     * calls. They stay valid until they are overwritten with
     * ca_context_change_props() again. */
    ret = ca_context_change_props(c,
                                  CA_PROP_APPLICATION_NAME, "Compiz bell plugin",
                                  CA_PROP_APPLICATION_ID, "org.freedesktop.compiz.Bell",
                                  CA_PROP_WINDOW_X11_SCREEN, getenv("DISPLAY"),
                                  NULL);


    ret = ca_context_open(c);


    /* Now trigger a sound event, the quick version */
    ret = ca_context_play(c, 0,
                          CA_PROP_EVENT_ID, "bell",
                          CA_PROP_MEDIA_FILENAME, "/usr/share/sounds/ubuntu/stereo/bell.ogg",
                          CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
                          NULL);
			}
		}
    }
    
    screen->handleEvent (event);
}


AudibleBell::AudibleBell (CompScreen *screen) :
	PluginClassHandler <AudibleBell, CompScreen> (screen),
	screen (screen)
{
    ScreenInterface::setHandler (screen); // Sets the screen function hook handler
    //screen->handleEventSetEnabled (this, true);
}

AudibleBell::~AudibleBell ()
{

}

bool
BellPluginVTable::init ()
{
    if (!CompPlugin::checkPluginABI ("core", CORE_ABIVERSION))
    {
		return false;
    }
    return true;
}
