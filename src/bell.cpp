#include "bell.h"
#include <core/atoms.h>

#include <canberra.h>

COMPIZ_PLUGIN_20090315 (bell, BellPluginVTable);

void
AudibleBell::handleEvent (XEvent *event)
{
    if (event->type == screen->xkbEvent ())
    {
        XkbAnyEvent *xkbEvent = (XkbAnyEvent *) event;

        if (xkbEvent->xkb_type == XkbBellNotify)
        {
            if (optionGetAudibleBell ())
            {
                ca_context *c;
                int ret;

                ret = ca_context_create (&c);

                ret = ca_context_change_props (c,
                                               CA_PROP_APPLICATION_NAME, "Compiz bell plugin",
                                               CA_PROP_APPLICATION_ID, "org.freedesktop.compiz.Bell",
                                               CA_PROP_WINDOW_X11_SCREEN, screen->displayString(),
                                               NULL);


                ret = ca_context_open (c);

                ret = ca_context_play (c, 0,
                                       CA_PROP_EVENT_ID, "bell",
                                       CA_PROP_MEDIA_FILENAME, "/usr/share/sounds/ubuntu/stereo/bell.ogg",
                                       CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
                                       NULL);
                
                ca_context_destroy (c);
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
