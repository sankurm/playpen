#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <tuple>
#include <numeric>
#include <string>
#include <iostream>

template <typename... Ts>
class table
{
    public:
        table() = default;
        table(std::initializer_list<std::string> column_names);
        table(std::initializer_list<std::string> column_names, std::initializer_list<std::tuple<Ts...>> data);

        void insert(Ts... ts);

        void println(const int index = 0, std::ostream& out = std::cout);

        const auto& get_data() const { return data; }
        const auto& get_column_names() const { return column_names; }
    private:
        std::vector<std::string> column_names;
        std::vector<std::tuple<Ts...>> data;

        //friend std::ostream& operator<<(std::ostream& out, const table<>&);
};

template<typename... Ts>
table<Ts...>::table(std::initializer_list<std::string> column_names)
        : column_names(column_names) {
    if (column_names.size() != sizeof...(Ts)) {
        throw std::invalid_argument(std::string{"No. of column_names ("} + std::to_string(column_names.size())
            + ") does not match the no. of columns (" + std::to_string(sizeof...(Ts)) + ")");
    }
}

template<typename... Ts>
table<Ts...>::table(std::initializer_list<std::string> column_names, std::initializer_list<std::tuple<Ts...>> data)
        : column_names(column_names), data(data) {
    if (column_names.size() != sizeof...(Ts)) {
        throw std::invalid_argument(std::string{"No. of column_names ("} + std::to_string(column_names.size())
            + ") does not match the no. of columns (" + std::to_string(sizeof...(Ts)) + ")");
    }
}

template<typename... Ts>
void table<Ts...>::insert(Ts... ts) {
    data.emplace_back(std::forward<Ts>(ts)...);
}

template<typename... Ts>
void table<Ts...>::println(const int index, std::ostream& out) {
    out << "At index " << index << ": " << data[index];
}

template<typename... Ts>
std::ostream& operator<<(std::ostream& out, const std::tuple<Ts...>& tup) {
    std::apply([&out](const Ts&... elems){
        out << '{';
        std::size_t n{0};
        ((out << elems << (++n != sizeof...(Ts)? ", ": "")), ...);
        out << "}\n";
    }, tup);
    return out;
    //for (int i = 0; i < std::tuple_size_v(tup); ++i) {
        //out << std::get<i>(tup) << ", ";
    //}
    //return out << ... << tup;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    for (const auto& elem : v) {
        out << elem << ' ';
    }
    return out;
}

template<typename... Ts>
std::ostream &operator<<(std::ostream &out, const table<Ts...>& tbl) {
    if (const auto& cols = tbl.get_column_names(); !cols.empty()) {
        out << ' ' << tbl.get_column_names() << '\n';
        std::size_t no_chars = std::accumulate(begin(cols), end(cols), 0z, [](int sum, const std::string& col) { return sum += col.length(); });
        out << ' ' << std::string(no_chars + cols.size() - 1, '=') << "\n ";
    }
    return out << tbl.get_data();
}


#endif