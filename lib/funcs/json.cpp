#include "json.h"

using namespace std;

namespace InjaFunctions {

inja::json fromJson(const inja::Arguments& args)
{
    if (args.size() != 1)
        throw runtime_error("Expected 1 parameters: fromJson(json: string): json");
    auto jsonString = args[0]->get<string>();
    try {
        auto json = inja::json::parse(jsonString);
        return json;
    } catch (const exception& e) {
        throw runtime_error((stringstream() << "Cannot parse JSON: " << e.what()).str());
    }
}

inja::json toJson(const inja::Arguments& args)
{
    if (args.size() < 1 || args.size() > 2)
        throw runtime_error("Expected 1 or 2 parameters: toJson(value: any, indent: int?): string");
    auto value = args[0];
    int indent = -1;
    if (args.size() == 2) {
        indent = args[1]->get<int>();
    }
    return value->dump(indent);
}

}
