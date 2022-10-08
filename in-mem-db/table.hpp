#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <tuple>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <string>
#include <iostream>

namespace db
{
    template <typename... Ts>
    class [[nodiscard]] table
    {
        public:
            table() = default;
            table(std::initializer_list<std::string> column_names);
            table(std::initializer_list<std::string> column_names, std::initializer_list<std::tuple<Ts...>> data);

            void insert(Ts... ts);

            void println(const int index = 0, std::ostream& out = std::cout);

            const auto& get_data() const;
            const auto& get_column_names() const;
        private:
            std::vector<std::string> column_names;
            std::vector<std::tuple<Ts...>> data;

            //friend std::ostream& operator<<(std::ostream& out, const table<>&);
    };

    /**
     * @brief Construct a new table object with column names
     * 
     * @param column_names Names of columns in the table
     * 
     * @exception std::illegal_argument Thrown when the no of column_names does not match the no of data elements
     */
    template<typename... Ts>
    table<Ts...>::table(std::initializer_list<std::string> column_names)
            : column_names(column_names) {
        if (column_names.size() != sizeof...(Ts)) {
            throw std::invalid_argument(std::string{"No. of column_names ("} + std::to_string(column_names.size())
                + ") does not match the no. of columns (" + std::to_string(sizeof...(Ts)) + ")");
        }
    }

    /**
     * @brief Construct a new table object with data and column names
     * 
     * @param column_names Names of columns in the table
     * @param data Entries in the table
     * 
     * @exception std::illegal_argument Thrown when the no of column_names does not match the no of data elements
     */
    template<typename... Ts>
    table<Ts...>::table(std::initializer_list<std::string> column_names, std::initializer_list<std::tuple<Ts...>> data)
            : column_names(column_names), data(data) {
        if (column_names.size() != sizeof...(Ts)) {
            throw std::invalid_argument(std::string{"No. of column_names ("} + std::to_string(column_names.size())
                + ") does not match the no. of columns (" + std::to_string(sizeof...(Ts)) + ")");
        }
    }

    /**
     * @brief Inserts a new row in the table with the passed values
     * 
     * @param ts The values of each column
     */
    template<typename... Ts>
    void table<Ts...>::insert(Ts... ts) {
        data.emplace_back(std::forward<Ts>(ts)...);
    }

    /**
     * @brief Prints the row at the given index to an std::ostream
     * 
     * @param index Index of the table entry
     * @param out std::ostream to write the entry to
     */
    template<typename... Ts>
    void table<Ts...>::println(const int index, std::ostream& out) {
        out << "At index " << index << ": " << data[index] << '\n';
    }

    /**
     * @brief Get the table data. This encapsulation breaking function only exists as I could not get the friend operator<< working :(
     * 
     * @return const std::vector<std::tuple<Ts...>>& Returns read-only internal data
     */
    template<typename... Ts>
    [[nodiscard]] const auto& table<Ts...>::get_data() const { return data; }

    /**
     * @brief Get the column names. This encapsulation breaking function only exists as I couldn't get the friend operator to access private members
     * 
     * @return const std::vector<std::string>& Returns read-only column names vector
     */
    template<typename... Ts>
    [[nodiscard]] const auto& table<Ts...>::get_column_names() const { return column_names; }

    template<typename... Ts>
    std::ostream &operator<<(std::ostream &out, const table<Ts...>& tbl) {
        if (const auto& cols = tbl.get_column_names(); !cols.empty()) {
            out << " | ";
            //std::copy(begin(cols), end(cols), std::ostream_iterator<std::string>(out, " | "));
            std::ranges::copy(cols, std::ostream_iterator<std::string>(out, " | "));
            std::size_t no_chars = std::accumulate(begin(cols), end(cols), 0z,
                [](int sum, const std::string& col) { return sum += col.length(); });
            out << "\n " << std::string(no_chars + 3 * cols.size() + 1, '=') << "\n";
        }
        return out << tbl.get_data();
    }
}

template<typename... Ts>
std::ostream& operator<<(std::ostream& out, const std::tuple<Ts...>& tup) {
    std::apply([&out](const Ts&... elems){
        ((out << " | " << elems), ...);
        out << " |";
    }, tup);
    return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    for (const auto& elem : v) {
        out << elem << "\n";
    }
    return out;
}

#endif