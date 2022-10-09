#include <algorithm>
#include <any>
#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

struct Index {
    std::unordered_multimap<std::string, unsigned long long> index_map;

    template<typename T>
    void populate(T data, unsigned long long ind){
        std::stringstream str;
        str << data;
        index_map.insert({str.str(), ind});
    }

    template<typename T>
    std::vector<unsigned long long> getIndexItem(T data){
        std::stringstream str;
        str << data;
        auto a = index_map.equal_range(str.str());
        std::vector<unsigned long long> ret;
        for(auto it = a.first; it != a.second; ++it){
            ret.push_back(it->second);
        }
        return ret;
    }
};

template<typename... Ts>
class IndexManager {
public:
    explicit IndexManager(std::shared_ptr<std::vector<std::tuple<Ts...>>>);

    template<unsigned I>
    void registerIndex();

    void addToIndex(unsigned long long row);

    template<unsigned I, typename T>
    void searchIndex(const T& data);

private:
    std::shared_ptr<std::vector<std::tuple<Ts...>>> data_ptr;
    std::unordered_map<unsigned int, Index> indices;
    static std::unordered_map<uintptr_t, std::shared_ptr<IndexManager>> managers;

    //Iterating through a tuple and populating index
    template<int I, typename... T>
    void iterateAndPopulate(const std::tuple<T...>& tup, unsigned long long row);
};

template <typename... Ts>
class Table {
public:
    Table(): data_ptr(std::make_shared<std::vector<std::tuple<Ts...>>>()), idx_mgr(data_ptr) {
    }

    //Inserts a new row into the table
    template<typename... Types>
    void insertRow(Types&&... vals);

    //Prints a row of the table to the output stream specified
    void printRow(unsigned rowId = 0, std::ostream& out = std::cout) const;

    //Creates an index for a specific field
    //making searches for values in that column
    //faster
    template <unsigned I>
    void createIndex();

    //Searches for an element in the column specified by
    //I. The column must have been indexed before this search.
    template<unsigned I, typename T>
    void searchIndex(const T& data);

//friends
    template<typename... T>
    friend std::ostream& operator<<(std::ostream&, const Table<T...>&);
    friend class IndexManager<Ts...>;

private:
    std::shared_ptr<std::vector<std::tuple<Ts...>>> data_ptr;
    IndexManager<Ts...> idx_mgr;
    std::vector<std::string> headers {sizeof...(Ts)};
};

template<typename... Ts>
std::unique_ptr<Table<Ts...>> createTable(){
    auto ptr = std::make_unique<Table<Ts...>>();
    return ptr;
}

template <typename P0, typename P1>
struct are_pack_elements_convertible;

template <typename... T0s, typename... T1s>
struct are_pack_elements_convertible<Table<T0s...>, Table<T1s...>>
        : std::bool_constant<(sizeof...(T0s) == sizeof...(T1s))
                             && (std::is_convertible_v<T1s, T0s> && ...)>
{
};

template <typename... Ts, std::size_t... Is> 
void print(const std::tuple<Ts...> &tup, std::index_sequence<Is...>, std::ostream& out){
        ((out << std::get<Is>(tup) << " "), ...);
}

template<typename... Ts>
void Table<Ts...>::printRow(const unsigned rowId, std::ostream& out) const{
    if(!(rowId >= data_ptr->size())){
        const auto& tup = (*data_ptr)[rowId];
        print(tup, std::index_sequence_for<Ts...>(), out);
        out << "\n";
    }
}

template<typename... Ts>
std::ostream& operator<<(std::ostream& out, Table<Ts...> const& db){
    for (const auto& tup : (*db.data_ptr)) {
        print(tup, std::index_sequence_for<Ts...>(), out);
    }
    return out;
}

template<typename... Ts>
template<typename... Types>
void Table<Ts...>::insertRow(Types&&... vals){
    static_assert(are_pack_elements_convertible<Table<Ts...>, Table<Types...>>::value, "DB Error: You can only insert values of the types that were used when creating the table");
    data_ptr->push_back(std::make_tuple(std::forward<Types>(vals)...));
    idx_mgr.addToIndex(data_ptr->size()-1);
}

template<typename... Ts>
std::unordered_map<uintptr_t, std::shared_ptr<IndexManager<Ts...>>> IndexManager<Ts...>::managers;

template<typename... Ts>
IndexManager<Ts...>::IndexManager(std::shared_ptr<std::vector<std::tuple<Ts...>>> data_ptr): data_ptr(data_ptr) {}

template<typename... Ts>
template<unsigned I>
void IndexManager<Ts...>::registerIndex(){
    using T = std::tuple<Ts...>;
    using type = std::tuple_element<I, T>;

    if (indices.find(I) ==  indices.end()){
        indices.insert({I, Index{}});

        for(decltype(data_ptr->size()) i = 0; i < data_ptr->size(); ++i){
            auto tup = data_ptr->at(i);
            auto elem = std::get<I>(tup);
            indices[I].populate(elem, i);
        }
    }
}

template<typename... Ts>
template<int I, typename... T>
void IndexManager<Ts...>::iterateAndPopulate(const std::tuple<T...>& tup, unsigned long long row){
    if (indices.find(I) != indices.end()){
        auto elem = std::get<I>(tup);
        indices[I].populate(elem, row);
    }
    if constexpr(I < (sizeof...(T) - 1)){
        iterateAndPopulate<I+1>(tup, row);
    }
}

template<typename... Ts>
void IndexManager<Ts...>::addToIndex(unsigned long long row){
    auto tup = data_ptr->at(row);
    iterateAndPopulate<0>(tup, row);
}

template<typename... Ts>
template<unsigned I, typename T>
void IndexManager<Ts...>::searchIndex(const T& data){
    if (indices.find(I) != indices.end()){
        auto vec = indices[I].getIndexItem(data);
        if(!vec.empty()){
            std::cout << "Search for '" << data << "', resulted in " << vec.size() << " result" << ((vec.size()>1)? "s.\n": ".\n");
            for(auto i : vec){
                print((*data_ptr)[i], std::index_sequence_for<Ts...>(), std::cout);
                std::cout << '\n';
            }
        } else {
            std::cout << "Search for " << data << " on index field " << I << " returned 0 results";
        }
    } else {
        std::cout << "Search for '" << data << "' on index field " << I << " 'returned 0 results' because field '" << I << "' has not been indexed.\n";
    }
}

template<typename... Ts>
template<unsigned I>
void Table<Ts...>::createIndex(){
    static_assert(I < sizeof...(Ts), "DB Error: You can create an index only on existing fields.");
    //if(!idx_mgr){
        //idx_mgr = IndexManager<Ts...>::getIndexManager(this);
    //}
    idx_mgr.template registerIndex<I>();
}

template<typename... Ts>
template<unsigned int I, typename T>
void Table<Ts...>::searchIndex(const T& data){
    static_assert(I<sizeof...(Ts), "DB Error: The index to be searched should be a valid index");
    using Type = typename std::tuple_element<I, std::tuple<Ts...>>::type;
    static_assert(std::is_convertible_v<T, Type>, "DB Error: The searched for item's type does not match"
                                                  " the type of the field on which this index was created");
    idx_mgr.template searchIndex<I>(data);
}

int main(){
    auto ptr = createTable<std::string,float, char, int>();
    ptr->createIndex<0>();
    ptr->insertRow("hello",2.0f, 'a', 32.0);
    ptr->insertRow("hello", 18, 74, 1);
    ptr->insertRow("beta", 12, 'z', 2);
    ptr->insertRow("gamma",5.1f, 'x', 3);
    ptr->insertRow("theta",20, 'y', 4);
    ptr->createIndex<2>();
    std::cout << "-----All Rows-----\n";
    std::cout << *ptr;
    std::cout << "------------------\n";
    ptr->createIndex<2>();
    ptr->searchIndex<2>('J');
    ptr->createIndex<1>();
    ptr->searchIndex<1>(5.1);
    ptr->searchIndex<0>("hello");
    std::cout << "----Print Individually----\n";
    ptr->printRow();
    ptr->printRow(1);
    ptr->printRow(2);
    ptr->printRow(3);
    ptr->printRow(4);
    std::cout << "--------------------------\n";
}