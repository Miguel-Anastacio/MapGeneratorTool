#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>
namespace Core
{
    namespace Utils
    {
        template <typename K, typename T>
        std::vector<T> mapKeysToVector(const std::unordered_map<K, T> &map)
        {
            std::vector<T> keys;
            keys.reserve(map.size());
            for (const auto val : map)
            {
                keys.emplace_back(val.first);
            }

            return keys;
        }

        template <typename K, typename V>
        bool getKeyFromValue(const std::unordered_map<K, V> &map, const V &value, K &key)
        {
            for (const auto val : map)
            {
                if (val.second == value)
                {
                    key = val.first;
                    return true;
                }
            }
            return false;
        }

        static std::string WStringToString(const std::wstring &wstr)
        {
            std::string str;
            size_t size;
            str.resize(wstr.length());
            wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
            return str;
        }
    } // namespace Utils

} // namespace MapGeneratorTool
