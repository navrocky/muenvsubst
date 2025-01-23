#include <sstream>

#include <args-parser/all.hpp>
#include <functional>

#include "inja_renderer.h"
#include "mstch_renderer.h"
#include "tools.h"

using namespace std;
using namespace std::placeholders;
using namespace Args;

static const char* MSTCH_ENGINE = "mstch";
static const char* INJA_ENGINE = "inja";

int main(int argc, char** argv, char** envp)
{
    CmdLine cmd(argc, argv);
    cmd.addHelp(
        true, argv[0], "Substitutes environment variables using one of the templating engines, as envsubst does.");
    cmd.addArgWithFlagAndName('V', "version", false, false, "Output version information and exit");
    cmd.addArgWithFlagAndName('e', "engine", true, false,
        (stringstream() << "Use template engine. Supported engines: " << MSTCH_ENGINE << ", " << INJA_ENGINE
                        << ". Default is: " << MSTCH_ENGINE)
            .str());
    try {
        cmd.parse();

        if (cmd.isDefined("-V")) {
            cout << "1.1.0" << endl;
            return 0;
        }

        string engine = MSTCH_ENGINE;
        if (cmd.isDefined("-e")) {
            engine = cmd.value("-e");
        }

        function<string(const string&, char**)> render;

        if (engine == MSTCH_ENGINE) {
            render = bind(renderWithMstch, _1, _2);
        } else if (engine == INJA_ENGINE) {
            render = bind(renderWithInja, _1, _2);
        } else {
            throw runtime_error((stringstream() << "Unsupported engine: " << engine).str());
        }

        auto tmpl = readAllInput();
        cout << render(tmpl, envp);

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
