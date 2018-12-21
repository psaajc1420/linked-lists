/*
    Jacob Cadena
    11/22/18
    list_unitest.cpp    
    
    Unit tests for my implementation of a list
*/

#include <limits.h>
#include <iostream>
#include "linked_list.h"
#include "gtest/gtest.h"

namespace {
 
class LinkedListTest : public testing::Test {
 protected:
  LinkedListTest() {
    list_ = new LinkedList<double>();
    list_->PushBack(43.342);
    list_->PushBack(534.342);
    list_->PushBack(7.324);
  }

  ~LinkedListTest() override { delete list_; }

  LinkedList<double>* list_;
};
 
class PushBackTest : public LinkedListTest { 
 protected:
  PushBackTest() {
    list_->PushBack(445.23);
    list_->PushBack(12.45);
    list_->PushBack(35.56);
  }

};
 
class PushFrontTest : public LinkedListTest { 
 protected:
  PushFrontTest() {
    list_->PushFront(445.23);
    list_->PushFront(12.45);
    list_->PushFront(35.56);
  }
};

class SizeTest : public LinkedListTest { };
class LinkedListIteratorTest : public LinkedListTest { };
class AssignmentOperatorTest : public LinkedListTest { };
class CopyConstructorTest : public LinkedListTest { };
class IsEmptyTest : public LinkedListTest { };
class ClearTest : public LinkedListTest { };
class RemoveTest : public LinkedListTest { };

class SortTest : public testing::Test {
 protected:
  SortTest() {
    list_ = new LinkedList<double>();
    list_->PushBack(4354.43);
    list_->PushBack(445.23);
    list_->PushBack(7.45);
    list_->PushBack(1253.23);
    list_->PushBack(63.56);
    list_->PushBack(342423.45);
    list_->PushBack(3.56);
  }

  ~SortTest() override { delete list_; }

  LinkedList<double>* list_;

  const double kSortedNums[7] = { 3.56, 7.45, 63.56, 445.23, 1253.23, 
                                  4354.43, 342423.45};
  const double kSortedNumsReverse[7] = { 342423.45, 4354.43, 1253.23, 445.23, 
                                         63.56, 7.45, 3.56};
  const double kListSize = 7;

  // comparison, decreasing order
  static bool Reverse(const double& first, const double& second) {
    return first >= second;
  }
};


// Tests the push back method
TEST_F(PushBackTest, CheckSizeFunction) {
  EXPECT_EQ(6, list_->Size());
}

TEST_F(PushBackTest, TestValues) {
  EXPECT_EQ(43.342, list_->GetHead()->data);
}

TEST_F(PushBackTest, TailPointsToNull) {
  EXPECT_EQ(nullptr, list_->GetTail()->next);
}

// Tests the push front method
TEST_F(PushFrontTest, CheckSizeFunction) {
  EXPECT_EQ(6, list_->Size());
}

TEST_F(PushFrontTest, TestValues) {
  EXPECT_EQ(35.56, list_->GetHead()->data);
  EXPECT_EQ(12.45, list_->GetHead()->next->data);
  EXPECT_EQ(445.23, list_->GetHead()->next->next->data);
}

TEST_F(PushFrontTest, TailPointsToNull) {
  EXPECT_EQ(nullptr, list_->GetTail()->next);
}

// Tests the size function
TEST_F(SizeTest, CheckSizeIsFunctional) {
  EXPECT_EQ(3, list_->Size());
  list_->PushBack(2);
  list_->PushBack(56);
  EXPECT_EQ(5, list_->Size());
}

// tests the LinkedListIterator
TEST_F(LinkedListIteratorTest, CheckForEquality) {

  double nums[3] = { 43.342, 534.342, 7.324 };

  std::size_t i = 0;
  for (LinkedListIterator<double> it = list_->Begin(); it != list_->End(); ++it) {
    EXPECT_EQ(nums[i], *it);
    i++;
  }
}

TEST_F(LinkedListIteratorTest, CheckTypedefIterator) {

  double nums[3] = { 43.342, 534.342, 7.324 };

  std::size_t i = 0;
  for (LinkedList<double>::iterator it = list_->Begin(); it != list_->End(); ++it) {
    EXPECT_EQ(nums[i], *it);
    i++;
  }
}

// Tests the assignment operator
TEST_F(AssignmentOperatorTest, CheckForEquality) {

  double nums[3] = { 43.342, 534.342, 7.324 };
  
  LinkedList<double> list;
  list = *list_;

  std::size_t i = 0;
  for (LinkedListIterator<double> it = list.Begin(); it != list.End(); ++it) {
    EXPECT_EQ(nums[i], *it);
    i++;
  }
}

// Tests the copy constructor
TEST_F(CopyConstructorTest, CheckForEquality) {

  double nums[3] = { 43.342, 534.342, 7.324 };
  
  LinkedList<double> list(*list_);

  std::size_t i = 0;
  for (LinkedListIterator<double> it = list.Begin(); it != list.End(); ++it) {
    EXPECT_EQ(nums[i], *it);
    i++;
  }
}

// Check the clear method
TEST_F(ClearTest, CheckSizeIsZero) {
  list_->Clear();
  EXPECT_EQ(0, list_->Size());
}

TEST_F(ClearTest, CheckHeadIsNull) {
  list_->Clear();
  EXPECT_EQ(nullptr, list_->GetHead());
}

TEST_F(ClearTest, CheckTailIsNull) {
  list_->Clear();
  EXPECT_EQ(nullptr, list_->GetTail());
}

// Tests the isEmpty method
TEST_F(IsEmptyTest, CheckForFunctionality) {
  list_->Clear();
  EXPECT_TRUE(list_->IsEmpty());
  list_->PushBack(10);
  EXPECT_FALSE(list_->IsEmpty());
}


TEST_F(SortTest, CheckUnSorted) {
  LinkedList<double>::iterator it = list_->Begin();
  for(int i = 0; i < kListSize; i++) {
    EXPECT_FALSE(kSortedNums[i] == *it);
    ++it;
  }
}

TEST_F(SortTest, CheckSorted) {
  list_->Sort();

  LinkedList<double>::iterator it = list_->Begin();
  for(int i = 0; i < kListSize; i++) {
    EXPECT_TRUE(kSortedNums[i] == *it);
    ++it;
  }
}

TEST_F(SortTest, CheckComparisonSort) {

  list_->Sort(Reverse);

  LinkedList<double>::iterator it = list_->Begin();
  for(int i = 0; i < kListSize; i++) {
    EXPECT_TRUE(kSortedNumsReverse[i] == *it);
    ++it;
  }
}

TEST_F(RemoveTest, CheckSize) {
  list_->Remove(534.342);
  EXPECT_EQ(2, list_->Size());
}

TEST_F(RemoveTest, CheckInBetweenRemove) {

  double num_remove = 534.342;
  list_->Remove(num_remove);

  LinkedList<double>::iterator it;
  for(it = list_->Begin(); it != list_->End(); ++it) {
    EXPECT_TRUE(*it != num_remove);
  }
}

TEST_F(RemoveTest, CheckListWithOne) {

  double num_remove = 43.342;
  list_->Remove(num_remove);
  num_remove = 7.324;
  list_->Remove(num_remove);
  EXPECT_EQ(1, list_->Size());
}

TEST_F(RemoveTest, CheckLastCase) {

  double num_remove = 7.324;
  list_->Remove(num_remove);

  LinkedList<double>::iterator it;
  for(it = list_->Begin(); it != list_->End(); ++it) {
    EXPECT_TRUE(*it != num_remove);
  }
  EXPECT_EQ(2, list_->Size());
}


} // namespace
