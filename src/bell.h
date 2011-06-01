#include <core/core.h>
#include <core/pluginclasshandler.h>

#include <canberra.h>

#include "bell_options.h"

class AudibleBell :
    public ScreenInterface,
	public PluginClassHandler<AudibleBell, CompScreen>,
	public BellOptions
{
    public:
		AudibleBell (CompScreen *screen);
		~AudibleBell ();
		
		CompScreen      *screen;
		ca_context *context;

		void
		handleEvent (XEvent *event);
		
		void
		filenameChange (CompOption *option,
			            Options    num);
};

class BellPluginVTable :
	public CompPlugin::VTableForScreen<AudibleBell>
{
    public:
		bool init ();
};

