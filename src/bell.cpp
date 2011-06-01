#include "bell.h"

COMPIZ_PLUGIN_20090315 (bell, BellPluginVTable);

void
AudibleBell::handleEvent (XEvent *event)
{
    if (event->type == screen->xkbEvent ())
    {
        XkbAnyEvent *xkb_any_event = (XkbAnyEvent *) event;

        if (xkb_any_event->xkb_type == XkbBellNotify)
        {
	    int error;

	    if ((error = ca_context_play (mCanberraContext, 0,
		                          CA_PROP_EVENT_ID, "bell",
		                          CA_PROP_MEDIA_FILENAME, mFilename.c_str (),
		                          CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
		                          NULL)) < 0)
	    {
	        compLogMessage ("bell", CompLogLevelWarn, "couldn't play sound %s - %s",
		                mFilename.c_str (), ca_strerror (error));
	    }
        }
    }
    
    screen->handleEvent (event);
}

void
AudibleBell::filenameChange(CompOption *option,
			    Options    num)
{
    int		error;

    mFilename = optionGetFilename();

    if ((error = ca_context_change_props  (mCanberraContext,
					   CA_PROP_APPLICATION_NAME, "Compiz bell",
					   CA_PROP_APPLICATION_ID, "org.freedesktop.compiz.Bell",
					   CA_PROP_WINDOW_X11_SCREEN, screen->displayString(),
					   NULL)) < 0)
    {
        compLogMessage ("bell", CompLogLevelWarn, "couldn't change context properties - %s",
                        ca_strerror (error));
    }

    if ((error = ca_context_cache (mCanberraContext,
                                   CA_PROP_EVENT_ID, "bell",
                                   CA_PROP_MEDIA_FILENAME, mFilename.c_str (),
                                   CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
                                   NULL)) < 0)
    {
        compLogMessage ("bell", CompLogLevelWarn, "couldn't change context cache - %s",
                        ca_strerror (error));
    }
}


AudibleBell::AudibleBell (CompScreen *screen) :
    PluginClassHandler <AudibleBell, CompScreen> (screen),
    mCanberraContext (NULL),
    mFilename (optionGetFilename ())
{
    int 					   error;
    boost::function <void (CompOption *, Options)> fileNameChangedCallback;

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

    fileNameChangedCallback =
	boost::bind (&AudibleBell::filenameChange, this, _1, _2);

    optionSetFilenameNotify (fileNameChangedCallback);
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
