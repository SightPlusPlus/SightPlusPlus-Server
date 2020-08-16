#define MAXAPI_USE_MSCRT
#define DllExport   __declspec( dllexport )
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "gtest/gtest.h"
#include "../src/priority_lib/smart_priority.hpp" 
#include "../src/priority_lib/smart_priority.cpp"   
#include "../src/priority_lib/priority_module.cpp"
#include "../src/classification_result.hpp"
#include <Set>


TEST(SquareRootTest, PositiveNos) {
	
}
TEST(SmartCreationTest, TestCreation) {
	std::string name = "sven";
	smart_priority* d = new smart_priority();
	ASSERT_EQ(name, d->get_name());

}

//TEST_F(depth_priorityTest, TestingSortCorrect) {
//    std::string name = d->get_name();
//    ASSERT_EQ(name, name);
//    std::string outs = test_vector.at(0).objects.at(0).to_string();
//    std::cout << outs;
//    //depth_priority dead = *d;
//    //std::vector<ClassificationResult> what;
//    //dead.run(&what);
//    //std::vector<ClassificationItem> data = test->objects;
//    //ASSERT_EQ("dog", data.at(0).name);
//    //ASSERT_EQ("cat", data.at(1).name);
//    //ASSERT_EQ("hat", data.at(2).name);
//    //ASSERT_EQ("person", data.at(3).name);
//
//   
//
//}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); 
}   