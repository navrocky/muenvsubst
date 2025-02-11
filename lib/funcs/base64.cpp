#include "base64.h"

#include <sstream>

using namespace std;

namespace InjaFunctions {

namespace {

// Implementation of base64 taken from:
//
//   http://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp/
//

static unsigned int posOfChar(const unsigned char chr)
{
    if (chr >= 'A' && chr <= 'Z')
        return chr - 'A';
    else if (chr >= 'a' && chr <= 'z')
        return chr - 'a' + ('Z' - 'A') + 1;
    else if (chr >= '0' && chr <= '9')
        return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
    else if (chr == '+' || chr == '-')
        return 62;
    else if (chr == '/' || chr == '_')
        return 63;
    else
        throw std::runtime_error("Input is not valid base64-encoded data");
}

static const char* base64Chars[2] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                      "abcdefghijklmnopqrstuvwxyz"
                                      "0123456789"
                                      "+/",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "-_" };

std::string base64Encode(unsigned char const* bytesToEncode, size_t inLen, bool url)
{
    size_t lenEncoded = (inLen + 2) / 3 * 4;
    unsigned char trailingChar = url ? '.' : '=';
    const char* base64Chars_ = base64Chars[url];
    std::string ret;
    ret.reserve(lenEncoded);
    unsigned int pos = 0;
    while (pos < inLen) {
        ret.push_back(base64Chars_[(bytesToEncode[pos + 0] & 0xfc) >> 2]);
        if (pos + 1 < inLen) {
            ret.push_back(
                base64Chars_[((bytesToEncode[pos + 0] & 0x03) << 4) + ((bytesToEncode[pos + 1] & 0xf0) >> 4)]);
            if (pos + 2 < inLen) {
                ret.push_back(
                    base64Chars_[((bytesToEncode[pos + 1] & 0x0f) << 2) + ((bytesToEncode[pos + 2] & 0xc0) >> 6)]);
                ret.push_back(base64Chars_[bytesToEncode[pos + 2] & 0x3f]);
            } else {
                ret.push_back(base64Chars_[(bytesToEncode[pos + 1] & 0x0f) << 2]);
                ret.push_back(trailingChar);
            }
        } else {
            ret.push_back(base64Chars_[(bytesToEncode[pos + 0] & 0x03) << 4]);
            ret.push_back(trailingChar);
            ret.push_back(trailingChar);
        }
        pos += 3;
    }
    return ret;
}

static std::string base64Decode(const string& encodedString, bool removeLinebreaks)
{
    if (encodedString.empty())
        return std::string();
    if (removeLinebreaks) {
        std::string copy(encodedString);
        copy.erase(std::remove(copy.begin(), copy.end(), '\n'), copy.end());
        return base64Decode(copy, false);
    }
    size_t lengthOfString = encodedString.length();
    size_t pos = 0;
    size_t approxLengthOfDecodedString = lengthOfString / 4 * 3;
    std::string ret;
    ret.reserve(approxLengthOfDecodedString);
    while (pos < lengthOfString) {
        size_t posOfChar1 = posOfChar(encodedString.at(pos + 1));
        ret.push_back(static_cast<std::string::value_type>(
            ((posOfChar(encodedString.at(pos + 0))) << 2) + ((posOfChar1 & 0x30) >> 4)));

        if ((pos + 2 < lengthOfString) && encodedString.at(pos + 2) != '=' && encodedString.at(pos + 2) != '.') {
            unsigned int posOfChar2 = posOfChar(encodedString.at(pos + 2));
            ret.push_back(
                static_cast<std::string::value_type>(((posOfChar1 & 0x0f) << 4) + ((posOfChar2 & 0x3c) >> 2)));

            if ((pos + 3 < lengthOfString) && encodedString.at(pos + 3) != '=' && encodedString.at(pos + 3) != '.') {
                ret.push_back(static_cast<std::string::value_type>(
                    ((posOfChar2 & 0x03) << 6) + posOfChar(encodedString.at(pos + 3))));
            }
        }
        pos += 4;
    }
    return ret;
}

}

inja::json fromBase64(const inja::Arguments& args)
{
    if (args.size() != 1)
        throw runtime_error("Expected 1 parameters: fromBase64(base64: string): string");
    auto in = args[0]->get<string>();
    try {
        return base64Decode(in, true);
    } catch (const std::exception& e) {
        throw runtime_error((stringstream() << "Cannot decode base64: " << e.what()).str());
    }
}

inja::json toBase64(const inja::Arguments& args)
{
    if (args.size() != 1)
        throw runtime_error("Expected 1 parameters: toBase64(text: string): string");
    auto in = args[0]->get<string>();
    return base64Encode((unsigned char*)in.data(), in.size(), false);
}

}
