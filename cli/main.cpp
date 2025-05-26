#include <args-parser/all.hpp>

#include <lib/inja_renderer.h>
#include <lib/template_loader.h>
#include <lib/tools.h>

using namespace std;
using namespace std::placeholders;
using namespace Args;

const char* APP_VERSION = "1.5.0";

int main(int argc, char** argv, char** envp)
{
    CmdLine cmd(argc, argv);
    cmd.addHelp(true, argv[0], "Substitutes environment variables using Inja templating engine, as envsubst does.");
    cmd.addArgWithFlagAndName('v', "version", false, false, "Output version information and exit");
    cmd.addArgWithFlagAndName('i', "in", true, false, "Input file");
    cmd.addMultiArg('d', "include-dir", true, false,
        "The directory where the included files are searched for. This argument can be specified more than once.");
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

        vector<string> includeDirs;
        if (cmd.isDefined("-d")) {
            auto dirs = cmd.values("-d");
            for (const auto& d : dirs) {
                includeDirs.push_back(d);
            }
        }

        FileTemplateLoader templateLoader({
            .includeDirs = includeDirs,
        });
        auto out = renderWithInja(tmpl,
            {
                .envs = getAllEnvs<nlohmann::json>(envp),
                .templateLoader = &templateLoader,
            });
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
