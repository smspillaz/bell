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

	bool bell ();

    private:

        ca_context *mCanberraContext;
	CompString mFilename;
	
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

