#include "table.hpp"

int main(int argc, char** argv) {
    // types of data type : Bool, integer, string and float
    // can be in any order

    table<int, const char*, bool> db1;
    // optional
    // db1.insertHeader("number", "Name", "visited");
    db1.insert(1, "Mr.K", true);
    db1.insert(2, "CppIndia", false);
    db1.insert(5, "Bjarne", true);
    db1.println();   // should println 1 Mr.K 1
    db1.println(2);  // bjarne
    // optional to print all the data inside object
    // std::cout<< db1 ;

    table<const char*, int, double> db2;
    // optional
    // db2.insertHeader( "Name", "ID", amount );
    db2.insert("Mr.K", 2157, 234.56);
    db2.insert("Scott Meyers", 86436, 6899.99);
    db2.insert("Sean Parent", 98765, 4678.78);
    db2.println();   // should print Mr.K 2157 234.56
    db2.println(1);  // Scott Meyers 86436 6899.99

    // optional to print all the data inside object
    // std::cout<< db2 ;
}
