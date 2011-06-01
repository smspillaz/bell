#include "bell.h"
#include <core/atoms.h>

COMPIZ_PLUGIN_20090315 (bell, BellPluginVTable);

void
AudibleBell::handleEvent (XEvent *event)
{
    if (event->type == screen->xkbEvent ())
    {
        XkbAnyEvent *xkb_any_event = (XkbAnyEvent *) event;

        if (xkb_any_event->xkb_type == XkbBellNotify)
        {
            int ret;
            const char *filename = optionGetFilename().c_str();

            ret = ca_context_play (context, 0,
                                  CA_PROP_EVENT_ID, "bell",
                                  CA_PROP_MEDIA_FILENAME, filename,
                                  CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
                                  NULL);
        }
    }
    
    screen->handleEvent (event);
}

void
AudibleBell::filenameChange(CompOption *option,
			                Options    num)
{
    int ret;
    const char *filename = optionGetFilename().c_str();

    ret = ca_context_cache (context,
                           CA_PROP_EVENT_ID, "bell",
                           CA_PROP_MEDIA_FILENAME, filename,
                           CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
                           NULL);
    
    /* play sound to give feedback?
    ret = ca_context_play (c, 0,
                           CA_PROP_EVENT_ID, "bell",
                           CA_PROP_MEDIA_FILENAME, filename,
                           CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
                           NULL);
    */
}


AudibleBell::AudibleBell (CompScreen *screen) :
    PluginClassHandler <AudibleBell, CompScreen> (screen),
    screen (screen)
{
    int ret;
    
    ret = ca_context_create (&context);

    ret = ca_context_change_props (context,
                                   CA_PROP_APPLICATION_NAME, "Compiz bell",
                                   CA_PROP_APPLICATION_ID, "org.freedesktop.compiz.Bell",
                                   CA_PROP_WINDOW_X11_SCREEN, screen->displayString(),
                                   NULL);


    ret = ca_context_open (context);
    
    ScreenInterface::setHandler (screen); // Sets the screen function hook handler
    
    optionSetFilenameNotify (boost::bind (&AudibleBell::
						filenameChange, this, _1, _2));
}

AudibleBell::~AudibleBell ()
{
    ca_context_destroy (context);
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
