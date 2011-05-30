#include "bell.h"

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
                int error;

                fprintf (stderr, "got xkbbellnotify\n");

                if ((error = ca_context_play (mCanberraContext, 0,
                                      CA_PROP_EVENT_ID, "bell",
                                      CA_PROP_MEDIA_FILENAME, "/usr/share/sounds/ubuntu/stereo/bell.ogg",
                                      CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
                                      NULL)) < 0)
                {
                    compLogMessage ("bell", CompLogLevelWarn, "couldn't play sound - %s",
                                    ca_strerror (error));
                }
            }
        }
    }
    
    screen->handleEvent (event);
}


AudibleBell::AudibleBell (CompScreen *screen) :
    PluginClassHandler <AudibleBell, CompScreen> (screen),
    mCanberraContext (NULL)
{
    int error;

    ScreenInterface::setHandler (screen); // Sets the screen function hook handler

    if ((error = ca_context_create (&mCanberraContext)) < 0)
    {
        compLogMessage ("bell", CompLogLevelWarn, "couldn't initialize canberra - %s",
                        ca_strerror (error));
        setFailed ();
    }
    else
    {
        if ((error = ca_context_change_props (mCanberraContext,
                                      CA_PROP_APPLICATION_NAME,
                                      "Compiz bell plugin",
                                      CA_PROP_APPLICATION_ID,
                                      "org.freedesktop.compiz.Bell",
                                      CA_PROP_WINDOW_X11_SCREEN,
                                      screen->displayString (),
                                      NULL)) < 0)
        {
            compLogMessage ("bell", CompLogLevelWarn, "couldn't register bell handler - %s",
                            ca_strerror (error));
            setFailed ();
        }
        else
        {
            if ((error = ca_context_open (mCanberraContext)) < 0)
            {
                compLogMessage ("bell", CompLogLevelWarn, "couldn't open canberra context - %s",
                                ca_strerror (error));
                setFailed ();
            }
        }
    }
}

AudibleBell::~AudibleBell ()
{
    ca_context_destroy (mCanberraContext);
}

bool
BellPluginVTable::init ()
{
    if (!CompPlugin::checkPluginABI ("core", CORE_ABIVERSION))
         return false;

    return true;
}
