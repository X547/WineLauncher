#include <stdio.h>
#include <Application.h>
#include <Entry.h>
#include <Path.h>
#include <OS.h>
#include <image.h>


class Application: public BApplication
{
public:
	Application(): BApplication("application/x-vnd.Test-WineLauncher")
	{
	}

	void RefsReceived(BMessage* message)
	{
		entry_ref ref;
		for (int32 index = 0; message->FindRef("refs", index, &ref) >= B_OK; index++) {
			BEntry entry(&ref, true);
			if (entry.InitCheck() < B_OK) continue;
			BPath path(&entry);
			const char *argv[] = {
				"/Haiku/data/packages/wine/tools/winewrapper",
				path.Path(),
				NULL
			};
			BPath parentPath;
			if (path.GetParent(&parentPath) >= B_OK) {
				chdir(parentPath.Path());
			}
			thread_id thread = load_image(B_COUNT_OF(argv) - 1, argv, (const char**)environ);
			resume_thread(thread);
		}
		
		PostMessage(B_QUIT_REQUESTED);
	}

};


int main()
{
	Application app;
	app.Run();
	return 0;
}
