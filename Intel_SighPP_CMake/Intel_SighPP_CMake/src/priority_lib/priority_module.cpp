// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include <vector>
#include <iostream>
#include "priority_module.hpp"
int pivot = 0;

void priority_module::build_data(std::vector<ClassificationResult>& data) {

	for (size_t i = 0; i < data.size(); i++)
	{
		all_data.insert(all_data.end(), data[i].objects.begin(), data[i].objects.end());
	}
};

//int priority_module::partition(std::vector<ClassificationItem>& values, int left, int right) {
//	int pivotIndex = left + (right - left) / 2;
//	ClassificationItem pivotItem = values[pivotIndex];
//	int i = left, j = right;
//	ClassificationItem temp;
//	while (i <= j) {
//		while (compare(values[i], pivotItem) == -1) {
//			i++;
//		}
//		while (compare(values[j], pivotItem) == 1) {
//			j--;
//		}
//		if (i <= j) {
//			temp = values[i];
//			values[i] = values[j];
//			values[j] = temp;
//			i++;
//			j--;
//		}
//	}
//	return i;
//}
//
//void priority_module::quicksort(std::vector<ClassificationItem>& data, int left, int right) {
//	//std::cout << "Running quicksort left: " << left << " right: " << right << std::endl; 
//	if (compare(data[left], data[right]) < 0) {
//		int piv = partition(data, left, right);
//		quicksort(data, left, piv-1);
//		quicksort(data, piv, right);
//	}
//}







