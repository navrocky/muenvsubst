#include <iostream>
#include <iterator>
#include <sstream>

#include <args-parser/all.hpp>
#include <functional>
#include <inja/inja.hpp>
#include <mstch/mstch.hpp>

using namespace std;
using namespace std::placeholders;
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

template <typename Map>
Map getAllEnvs(char** envp)
{
    Map m;
    for (char** env = envp; *env != 0; env++) {
        auto kv = parseKeyValue(*env);
        m[kv.key] = kv.value;
    }
    return m;
}

static const char* MSTCH_ENGINE = "mstch";
static const char* INJA_ENGINE = "inja";

string renderWithMstch(const string& tmpl, char** envp)
{
    auto envs = getAllEnvs<mstch::map>(envp);
    return mstch::render(tmpl, envs);
}

vector<string> stringSplit(const string& s, const string& delimiter)
{
    vector<string> res;
    if (delimiter.empty()) {
        res.push_back(s);
        return res;
    }
    string::size_type prevPos = 0, pos = 0;
    while ((pos = s.find(delimiter, pos)) != string::npos) {
        res.push_back(s.substr(prevPos, pos - prevPos));
        prevPos = ++pos;
    }
    res.push_back(s.substr(prevPos, pos - prevPos));
    return res;
}

inja::json split(const inja::Arguments& args)
{
    if (args.size() != 2)
        throw runtime_error("Expected 2 parameters: split(s: string, delimiter: string)");
    auto s = args[0]->get<string>();
    auto delimiter = args[1]->get<string>();
    return stringSplit(s, delimiter);
}

string renderWithInja(const string& tmpl, char** envp)
{
    auto envs = getAllEnvs<inja::json>(envp);
    inja::Environment env;
    env.add_callback("split", bind(split, _1));
    return env.render(tmpl, envs);
}

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
