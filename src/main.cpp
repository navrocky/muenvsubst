#include <iostream>
#include <iterator>

#include <args-parser/all.hpp>
#include <mstch/mstch.hpp>

using namespace std;
using namespace Args;

string readAllInput()
{
    cin >> noskipws;
    istream_iterator<char> it(cin);
    istream_iterator<char> end;
    return string(it, end);
}

struct KeyValue {
    string key;
    string value;
};

KeyValue parseKeyValue(const string& s)
{
    auto i = s.find("=");
    if (i == string::npos)
        return { .key = s };
    return {
        .key = s.substr(0, i),
        .value = s.substr(i + 1),
    };
}

mstch::map getAllEnvs(char** envp)
{
    mstch::map m;
    for (char** env = envp; *env != 0; env++) {
        auto kv = parseKeyValue(*env);
        m[kv.key] = kv.value;
    }
    return m;
}

int main(int argc, char** argv, char** envp)
{
    CmdLine cmd(argc, argv);
    cmd.addHelp(true, argv[0], "Substitutes environment variables using Mustache template syntax.");
    cmd.addArgWithFlagAndName('V', "version", false, false, "Output version information and exit");
    try {
        cmd.parse();

        if (cmd.isDefined("-V")) {
            cout << "1.0.2" << endl;
            return 0;
        }

        auto tmpl = readAllInput();
        auto envs = getAllEnvs(envp);
        cout << mstch::render(tmpl, envs);
    } catch (const HelpHasBeenPrintedException&) {
        return 0;
    } catch (const BaseException& x) {
        cerr << x.desc() << "\n";
        return 1;
    }
    return 0;
}
