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



namespace {
	class FooTest : public ::testing::Test {

	protected:
		smart_priority* d;
		ClassificationResult clas_res{ "name" };
		protected:
		
		    ClassificationItem item1{"cup"};
		    ClassificationItem item2{ "hat" };
		    ClassificationItem item3{ "dog" };
		    ClassificationItem item4{ "person" };
		
		    std::vector<ClassificationResult> test_vector;
		    std::vector<ClassificationItem> test_items;
		      
		FooTest() {
		}

		virtual ~FooTest() {
		}

		virtual void SetUp() {
			d = new smart_priority();
			item1.distance = 0.6;
			item2.distance = 0.7;
			item3.distance = 0.5;
			test_items.push_back(item1);
			test_items.push_back(item2);
			test_items.push_back(item3);
			test_items.push_back(item4);
			clas_res.model_name = "test_model";
			clas_res.objects = test_items;
			test_vector.push_back(clas_res);

		}

		virtual void TearDown() {
			delete d;
		}

	};

}
TEST_F(FooTest, TestFoo) {
	
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