// GateServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "boost/lexical_cast.hpp"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>


int main()
{
    float weight = 4.5;
    std::cout << boost::lexical_cast<std::string>(weight) << std::endl;
    std::cout << "Hello World!\n";

    Json::Value root;
    root["id"] = 1000;
    root["data"] = "hello world json";
    std::string request = root.toStyledString();
    std::cout << "request data is: " << request << std::endl;

    Json::Value root2;
    Json::Reader reader;
    reader.parse(request, root2);
    std::cout << "id is: " << root["id"] << ", data is: " << root["data"] << std::endl;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
