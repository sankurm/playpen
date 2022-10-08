#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <tuple>
#include <string>
#include <iostream>

template <typename... Ts>
class table {
    public:
        table() = default;
        table(std::initializer_list<std::string> column_names) : column_names(column_names) {}

        void insert(Ts... ts);

        void println(const int rowID = 0);
    private:
        std::vector<std::string> column_names;
        std::vector<std::tuple<Ts...>> data;
};

template<typename... Ts>
void table<Ts...>::insert(Ts... ts) {
    //data.push_back(std::make_tuple<Ts...>(std::forward<Ts>(ts)...));
    data.emplace_back(ts...);
}

template<typename... Ts>
void table<Ts...>::println(const int index) {
    std::cout << data[index] << '\n';
}

template<typename... Ts>
std::ostream& operator<<(std::ostream& out, const std::tuple<Ts...>& tup) {
    std::apply([&out](const Ts&... elems){
        out << '{';
        std::size_t n{0};
        ((out << elems << (++n != sizeof...(Ts)? ", ": "")), ...);
        out << '}';
    }, tup);
    return out;
    //for (int i = 0; i < std::tuple_size_v(tup); ++i) {
        //out << std::get<i>(tup) << ", ";
    //}
    //return out << ... << tup;
}

// optional
// std::ostream &operator<<(std::ostream &out, const kDB &ob)
//{
//	return out;
// }


#endif