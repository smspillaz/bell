#include <core/core.h>
#include <core/pluginclasshandler.h>

class AudibleBell :
    public ScreenInterface,
	public PluginClassHandler<AudibleBell, CompScreen>
{
    public:
		AudibleBell (CompScreen *screen);
		~AudibleBell ();
		
		CompScreen      *screen;

		void
		handleEvent (XEvent *event);
};

class BellPluginVTable :
	public CompPlugin::VTableForScreen<AudibleBell>
{
    public:
		bool init ();
};

