#include <args-parser/all.hpp>

#include <lib/inja_renderer.h>
#include <lib/tools.h>

using namespace std;
using namespace std::placeholders;
using namespace Args;

const char* APP_VERSION = "1.4.0";

int main(int argc, char** argv, char** envp)
{
    bool showVersion = false;
    CmdLine cmd(argc, argv);
    cmd.addHelp(true, argv[0], "Substitutes environment variables using Inja templating engine, as envsubst does.");
    cmd.addArgWithFlagAndName('v', "version", false, false, "Output version information and exit");
    cmd.addArgWithFlagAndName('i', "in", true, false, "Input file");
    cmd.addArgWithFlagAndName('o', "out", true, false, "Output file");
    try {
        cmd.parse();

        if (cmd.isDefined("-v")) {
            cout << APP_VERSION << endl;
            return 0;
        }

        string tmpl;
        if (cmd.isDefined("-i")) {
            tmpl = readFile(cmd.value("-i"));
        } else {
            tmpl = readAllInput();
        }
        auto out = renderWithInja(tmpl, envp);
        if (cmd.isDefined("-o")) {
            writeFile(cmd.value("-o"), out);
        } else {
            cout << out;
        }
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
