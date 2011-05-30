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
			XkbBellNotifyEvent *xkbBellEvent = (XkbBellNotifyEvent *) xkbEvent;

			/* if (optionGetAudibleBell ())
			{
			*/
			    ca_context *c;
                ca_proplist *p;
                int res;

                ca_proplist_create (&p);
                ca_proplist_sets (p, CA_PROP_EVENT_ID, "bell-window-system");
                ca_proplist_sets (p, CA_PROP_EVENT_DESCRIPTION, "Bell event");
                ca_proplist_sets (p, CA_PROP_CANBERRA_CACHE_CONTROL, "permanent");
                
                ca_context_open (c);

                /* First, we try to play a real sound ... */
                res = ca_context_play_full (c, 1, p, NULL, NULL);

                ca_proplist_destroy (p);
                ca_context_destroy (c);

			/* } */
		}
    }
}


AudibleBell::AudibleBell (CompScreen *screen) :
	PluginClassHandler <AudibleBell, CompScreen> (screen)
{

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
