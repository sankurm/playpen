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

        private:
            std::vector<std::string> column_names;
            std::vector<std::tuple<Ts...>> data;

            template<typename... Tees>
            friend std::ostream& operator<<(std::ostream& out, const table<Tees...>&);
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

    template<typename... Ts>
    std::ostream& operator<<(std::ostream& out, const std::tuple<Ts...>& tup) {
        out << " | ";
        std::apply([&out](const Ts&... elems) { ((out << elems << " | "), ...); }, tup);
        return out;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
        for (const auto& elem : v) {
            out << elem << "\n";
        }
        return out;
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

    template<typename... Ts>
    std::ostream &operator<<(std::ostream &out, const table<Ts...>& tbl) {
        if (const auto& cols = tbl.column_names; !cols.empty()) {
            out << " | ";
            std::size_t no_underline_chars = 2;   //"| " already printed above, ignoring initial space
            //Print columns while summing up the chars written
            std::ranges::transform(cols, std::ostream_iterator<std::string>(out, " | "),
                [&no_underline_chars](const std::string& name) { no_underline_chars += name.length() + 3; return name; });
            no_underline_chars -= 1;  //Deduct 1 - dont underline the trailing space
            out << "\n " << std::string(no_underline_chars, '=') << "\n";
        }
        return out << tbl.data;
    }
}

#endif