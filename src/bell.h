#include <core/core.h>
#include <core/pluginclasshandler.h>
#include <core/atoms.h>

#include <canberra.h>

#include "bell_options.h"

class AudibleBell :
    public PluginClassHandler<AudibleBell, CompScreen>,
    public ScreenInterface,
    public BellOptions
{
    public:

        AudibleBell (CompScreen *screen);
        ~AudibleBell ();

	void
	handleEvent (XEvent *event);

    private:

        ca_context *mCanberraContext;
};

class BellPluginVTable :
    public CompPlugin::VTableForScreen<AudibleBell>
{
    public:
        bool init ();
};

