#include "bell.h"
#include <core/atoms.h>

COMPIZ_PLUGIN_20090315 (bell, BellPluginVTable);

void
AudibleBell::handleEvent (XEvent *event)
{
    if (event->type == screen->xkbEvent ())
    {
		XkbAnyEvent *xkbEvent = (XkbAnyEvent *) event;

		if (xkbEvent->xkb_type == XkbBellNotify)
		{
			/* XkbBellNotifyEvent *xkbBellEvent = (XkbBellNotifyEvent *) xkbEvent; */

			if (optionGetAudibleBell ())
			{
				/* libcanberra magic here */
			}
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
