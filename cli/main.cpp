#include <args-parser/all.hpp>

#include <lib/inja_renderer.h>
#include <lib/tools.h>

using namespace std;
using namespace std::placeholders;
using namespace Args;

const char* APP_VERSION = "1.3.0";

int main(int argc, char** argv, char** envp)
{
    bool showVersion = false;
    CmdLine cmd(argc, argv);
    cmd.addHelp(
        true, argv[0], "Substitutes environment variables using one of the templating engines, as envsubst does.");
    cmd.addArgWithFlagAndName('V', "version", false, false, "Output version information and exit");
    try {
        cmd.parse();

        if (cmd.isDefined("-V")) {
            cout << APP_VERSION << endl;
            return 0;
        }

        auto tmpl = readAllInput();
        cout << renderWithInja(tmpl, envp);

    } catch (const HelpHasBeenPrintedException&) {
        return 0;
    } catch (const BaseException& x) {
        cerr << x.desc() << "\n";
        return 1;
    } catch (const exception& x) {
        cerr << x.what() << "\n";
        return 1;
    }
    return 0;
}
