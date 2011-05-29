#include <core/core.h>
#include <core/pluginclasshandler.h>

#include "bell_options.h"

class AudibleBell :
	public PluginClassHandler<AudibleBell, CompScreen>,
	public BellOptions
{
    public:
		AudibleBell (CompScreen *s);
		~AudibleBell ();

		void handleEvent (XEvent *event);
};

class BellPluginVTable :
	public CompPlugin::VTableForScreen<AudibleBell>
{
    public:
		bool init ();
};

