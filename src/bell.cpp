#include "bell.h"
#include <core/atoms.h>

#include <canberra.h>

COMPIZ_PLUGIN_20090315 (bell, BellPluginVTable);

void
AudibleBell::handleEvent (XEvent *event)
{
    if (event->type == screen->xkbEvent ())
    {
        XkbAnyEvent *xkb_any_event = (XkbAnyEvent *) event;

        if (xkb_any_event->xkb_type == XkbBellNotify)
        {
            ca_context *c;
            int ret;
            const char *filename = optionGetFilename().c_str();

            ret = ca_context_create (&c);

            ret = ca_context_change_props (c,
                                           CA_PROP_APPLICATION_NAME, "Compiz bell",
                                           CA_PROP_APPLICATION_ID, "org.freedesktop.compiz.Bell",
                                           CA_PROP_WINDOW_X11_SCREEN, screen->displayString(),
                                           NULL);


            ret = ca_context_open (c);

            ret = ca_context_play (c, 0,
                                  CA_PROP_EVENT_ID, "bell",
                                  CA_PROP_MEDIA_FILENAME, filename,
                                  CA_PROP_CANBERRA_CACHE_CONTROL, "never", // fixme: should be "permanent
                                  NULL);
                
            ca_context_destroy (c);
            
            if (ret != CA_SUCCESS && ret != CA_ERROR_DISABLED)
            {
                int xkb_base_error_type, xkb_opcode;
                XkbBellNotifyEvent *xkb_bell_event = (XkbBellNotifyEvent*) xkb_any_event;
                Display *display = screen->dpy();
                
                if (XkbQueryExtension (display, &xkb_opcode, 
			                            &xkb_any_event->xkb_type, 
			                            &xkb_base_error_type, 
			                            NULL,
			                            NULL))
                {
                      
                     XkbSelectEvents (display,
		                              XkbUseCoreKbd,
		                              XkbBellNotifyMask,
		                              XkbBellNotifyMask);
                     XkbChangeEnabledControls (display,
				                               XkbUseCoreKbd,
				                               XkbAudibleBellMask,
                                               0);
                     XkbForceDeviceBell (display, 
                                         xkb_bell_event->device, 
                                         xkb_bell_event->bell_class, 
                                         xkb_bell_event->bell_id, 
                                         xkb_bell_event->percent);
                }
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
