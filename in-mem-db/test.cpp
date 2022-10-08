#include "table.hpp"

int main(int, char**) {
    // types of data type : Bool, integer, string and float
    // can be in any order

    table<int, const char*, bool> tbl1({"Id", "Name", "IsIndividual"});
    // optional
    // tbl1.insertHeader("number", "Name", "visited");
    tbl1.insert(1, "Mr.K", true);
    tbl1.insert(2, "CppIndia", false);
    tbl1.insert(5, "Bjarne", true);
    tbl1.println();   // should println 1 Mr.K 1
    tbl1.println(2);  // bjarne
    // optional to print all the data inside object
    std::cout<< "\nTable 1:\n" << tbl1 << '\n';

    table<const char*, int, double> tbl2;
    // optional
    // tbl2.insertHeader( "Name", "ID", amount );
    tbl2.insert("Mr.K", 2157, 234.56);
    tbl2.insert("Scott Meyers", 86436, 6899.99);
    tbl2.insert("Sean Parent", 98765, 4678.78);
    tbl2.println();   // should print Mr.K 2157 234.56
    tbl2.println(1);  // Scott Meyers 86436 6899.99

    // optional to print all the data inside object
    std::cout<< "\nTable 2:\n " << tbl2 << '\n';

    try {
        table<const char*, int, double> tbl3({"Name", "Id", "Amount", "Extra", "More Extra"});
    } catch(const std::invalid_argument& e) {
        std::cout << "std::invalid_argument exception for tbl3: " << e.what() << '\n';
    }

    try {
        table<const char*, int, double> tbl4({"Name", "Id"});
    } catch(const std::invalid_argument& e) {
        std::cout << "std::invalid_argument exception for tbl4: " << e.what() << '\n';
    }

    table<const char*, double, long> tbl5(
        {"Name", "Height", "Distance"}, 
        {
            {"Bjarne Stroustrup", 1.88, 6600},
            {"Herb Sutter", 1.75, 6200},
            {"Richard Smith", 1.90, 7000},
            {"Andrei Alexadrescu", 1.80, 5600},
        });
    std::cout<< "\nTable 5:\n" << tbl5 << '\n';
    tbl5.insert("Ankur Satle", 1.75, 0);
    std::cout<< "\nTable 5:\n" << tbl5 << '\n';
    //tbl5.insert(1.40, "Superman", 2.2); //Mismatched parameters do not compile, only correct ones do
}
