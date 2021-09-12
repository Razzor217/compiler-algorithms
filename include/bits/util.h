#include <algorithm>
#include <vector>
#include <queue>

namespace compiler
{
    namespace util
    {
        template <typename T>
        bool contains(std::vector<T> const& v, T const& value)
        {
            return std::find(v.begin(), v.end(), value) != v.end();
        }

        template <typename T>
        bool contains(std::queue<T> const& v, T const& value)
        {
            return std::find(v.begin(), v.end(), value) != v.end();
        }
    } // namespace util
} // namespace compiler