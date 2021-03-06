// File:        test_list.cpp
// Description: ---
// Notes:       ---
// Author:      leoxiang <leoxiang727@qq.com>
// Revision:    2012-06-15 by leoxiang

#include "lsf/basic/unit_test.hpp"
#include "lsf/container/list.hpp"
#include "node.hpp"

using namespace std;
using namespace lsf::container;

#define SHM_KEY 0x082157ff
#define QUEUE_SIZE 1024

LSF_TEST_CASE(bind_to_new_mem) {
    if (SharedMem::IsShmExist(SHM_KEY)) LSF_ASSERT(SharedMem::Delete(SHM_KEY));
    LSF_ASSERT(SharedMem::Create(SHM_KEY, List<TestNode>::CalcByteSize(QUEUE_SIZE)));

    List<TestNode, SharedMem> list;

    LSF_ASSERT(list.BindAndInitStorage(SharedMem(SHM_KEY)));
    LSF_ASSERT(list.IsBindStorage());
    LSF_ASSERT(list.max_size() == QUEUE_SIZE);
    LSF_ASSERT(list.size() == 0);
    LSF_ASSERT(list.empty());
    LSF_ASSERT(!list.full());
    LSF_ASSERT(list.size() == 0);

    LSF_ASSERT(list.PushBack(TestNode()));
    LSF_ASSERT(list.PushBack(TestNode()));
    LSF_ASSERT(list.PushBack(TestNode()));
    LSF_ASSERT(list.size() == 3);
}

LSF_TEST_CASE(recovery_from_exist_mem) {
    List<TestNode, SharedMem> list;

    LSF_ASSERT(list.BindAndRecoverStorage(SharedMem(SHM_KEY)));
    LSF_ASSERT(list.IsBindStorage());

    LSF_ASSERT(list.max_size() == QUEUE_SIZE);
    LSF_ASSERT(list.size() == 3);

    LSF_ASSERT(SharedMem::Delete(SHM_KEY));
}

LSF_TEST_CASE(list_common_funcs) {
    List<TestNode, SharedMem> list;

    if (SharedMem::IsShmExist(SHM_KEY)) LSF_ASSERT(SharedMem::Delete(SHM_KEY));
    LSF_ASSERT(SharedMem::Create(SHM_KEY, List<TestNode, SharedMem>::CalcByteSize(5)));
    LSF_ASSERT(list.BindAndInitStorage(SharedMem(SHM_KEY)));
    LSF_ASSERT(list.max_size() == 5);
    LSF_ASSERT(list.IsBindStorage());

    // common func
    LSF_ASSERT(list.empty());
    LSF_ASSERT(list.PushBack(TestNode(1, 1)));
    LSF_ASSERT(list.PushBack(TestNode(2, 2)));
    LSF_ASSERT(list.PushBack(TestNode(3, 3)));
    LSF_ASSERT(list.PushBack(TestNode(4, 4)));
    LSF_ASSERT(list.PushBack(TestNode(5, 5)));
    LSF_ASSERT(list.max_size() == list.size());
    LSF_ASSERT(!list.PushBack(TestNode(6, 6)));
    LSF_ASSERT(list.full());
    LSF_ASSERT(list.size() == 5);

    LSF_ASSERT(*list.GetFront() == TestNode(1, 1));
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(*list.GetFront() == TestNode(2, 2));
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(*list.GetFront() == TestNode(5, 5));
    LSF_ASSERT(list.size() == 1);
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(list.empty());

    LSF_ASSERT(SharedMem::Delete(SHM_KEY));
}

LSF_TEST_CASE(test_push_and_pop_from_front_and_back) {
    List<TestNode, SharedMem> list;

    if (SharedMem::IsShmExist(SHM_KEY)) LSF_ASSERT(SharedMem::Delete(SHM_KEY));
    LSF_ASSERT(SharedMem::Create(SHM_KEY, List<TestNode, SharedMem>::CalcByteSize(10)));
    LSF_ASSERT(list.BindAndInitStorage(SharedMem(SHM_KEY)));
    LSF_ASSERT(list.max_size() == 10);

    LSF_ASSERT(list.PushFront(TestNode(1, 1)));
    LSF_ASSERT(list.GetFront() == list.GetBack());
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(list.PushFront(TestNode(1, 1)));
    LSF_ASSERT(list.PopBack());

    LSF_ASSERT(list.PushFront(TestNode(1, 1)));
    LSF_ASSERT(list.PushFront(TestNode(2, 2)));
    LSF_ASSERT(list.PushFront(TestNode(3, 3)));
    LSF_ASSERT(list.PushFront(TestNode(4, 4)));
    LSF_ASSERT(list.PushFront(TestNode(5, 5)));
    LSF_ASSERT(list.PushFront(TestNode(6, 6)));
    LSF_ASSERT(*(list.GetFront()) == TestNode(6, 6));
    LSF_ASSERT(*(list.GetBack()) == TestNode(1, 1));
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(*(list.GetFront()) == TestNode(5, 5));
    LSF_ASSERT(list.PopBack());
    LSF_ASSERT(*(list.GetBack()) == TestNode(2, 2));
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(list.PopBack());
    LSF_ASSERT(list.PopBack());
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(list.empty());

    LSF_ASSERT(SharedMem::Delete(SHM_KEY));
}

LSF_TEST_CASE(test_iterator) {
    List<TestNode, SharedMem> list;
    if (SharedMem::IsShmExist(SHM_KEY)) LSF_ASSERT(SharedMem::Delete(SHM_KEY));
    LSF_ASSERT(SharedMem::Create(SHM_KEY, List<TestNode, SharedMem>::CalcByteSize(10)));
    LSF_ASSERT(list.BindAndInitStorage(SharedMem(SHM_KEY)));
    LSF_ASSERT(list.max_size() == 10);

    LSF_ASSERT(list.PushBack(TestNode(1, 1)));
    LSF_ASSERT(list.PushBack(TestNode(2, 2)));
    LSF_ASSERT(list.PushBack(TestNode(3, 3)));
    LSF_ASSERT(list.PushBack(TestNode(4, 4)));
    LSF_ASSERT(list.PushBack(TestNode(5, 5)));
    LSF_ASSERT(list.PushBack(TestNode(6, 6)));
    LSF_ASSERT(list.PushBack(TestNode(7, 7)));
    LSF_ASSERT(list.PushBack(TestNode(8, 8)));
    LSF_ASSERT(list.PushBack(TestNode(9, 9)));
    LSF_ASSERT(list.PushBack(TestNode(0, 0)));
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(list.PopFront());
    LSF_ASSERT(list.PushBack(TestNode(1, 1)));
    LSF_ASSERT(list.PushBack(TestNode(2, 2)));
    LSF_ASSERT(list.PushBack(TestNode(3, 3)));

    // test iterator
    List<TestNode, SharedMem>::iterator iter_begin = list.begin();
    List<TestNode, SharedMem>::iterator iter = iter_begin;
    List<TestNode, SharedMem>::iterator iter_end = list.end();
    LSF_ASSERT(iter_begin != iter_end);
    LSF_ASSERT(iter == iter_begin);
    LSF_ASSERT(*(iter++) == TestNode(4, 4));
    LSF_ASSERT(*(iter++) == TestNode(5, 5));
    LSF_ASSERT(*(iter++) == TestNode(6, 6));
    LSF_ASSERT(*(iter++) == TestNode(7, 7));
    LSF_ASSERT(*(iter++) == TestNode(8, 8));
    LSF_ASSERT(*(iter++) == TestNode(9, 9));
    LSF_ASSERT(*(iter++) == TestNode(0, 0));
    LSF_ASSERT(*(iter++) == TestNode(1, 1));
    LSF_ASSERT(*(iter++) == TestNode(2, 2));
    LSF_ASSERT(*(iter++) == TestNode(3, 3));
    LSF_ASSERT(list.size() == 10);
    LSF_ASSERT(iter == iter_end);

    LSF_ASSERT(iter == iter_end);
    LSF_ASSERT(*(--iter) == TestNode(3, 3));
    LSF_ASSERT(*(--iter) == TestNode(2, 2));
    LSF_ASSERT(*(--iter) == TestNode(1, 1));
    LSF_ASSERT(*(--iter) == TestNode(0, 0));
    LSF_ASSERT(*(--iter) == TestNode(9, 9));
    LSF_ASSERT(*(--iter) == TestNode(8, 8));
    LSF_ASSERT(*(--iter) == TestNode(7, 7));
    LSF_ASSERT(*(--iter) == TestNode(6, 6));
    LSF_ASSERT(*(--iter) == TestNode(5, 5));
    LSF_ASSERT(*(--iter) == TestNode(4, 4));
    LSF_ASSERT(list.size() == 10);
    LSF_ASSERT(iter == iter_begin);

    // test reverse iterator
    List<TestNode, SharedMem>::reverse_iterator iter_rbegin = list.rbegin();
    List<TestNode, SharedMem>::reverse_iterator iter_rend = list.rend();
    List<TestNode, SharedMem>::reverse_iterator riter = iter_rbegin;
    LSF_ASSERT(iter_rbegin != iter_rend);
    LSF_ASSERT(riter == iter_rbegin);
    LSF_ASSERT(*(riter++) == TestNode(3, 3));
    LSF_ASSERT(*(riter++) == TestNode(2, 2));
    LSF_ASSERT(*(riter++) == TestNode(1, 1));
    LSF_ASSERT(*(riter++) == TestNode(0, 0));
    LSF_ASSERT(*(riter++) == TestNode(9, 9));
    LSF_ASSERT(*(riter++) == TestNode(8, 8));
    LSF_ASSERT(*(riter++) == TestNode(7, 7));
    LSF_ASSERT(*(riter++) == TestNode(6, 6));
    LSF_ASSERT(*(riter++) == TestNode(5, 5));
    LSF_ASSERT(*(riter++) == TestNode(4, 4));
    LSF_ASSERT(list.size() == 10);
    LSF_ASSERT(riter == iter_rend);

    // test generic algorithm
    for (iter = list.begin(); iter != list.end();) {
        LSF_ASSERT(list.Erase(iter++));
    }
    LSF_ASSERT(list.empty());

    LSF_ASSERT(SharedMem::Delete(SHM_KEY));
}

LSF_TEST_CASE(test_insert_and_erase_and_find) {
    List<TestNode, SharedMem> list;
    if (SharedMem::IsShmExist(SHM_KEY)) LSF_ASSERT(SharedMem::Delete(SHM_KEY));
    LSF_ASSERT(SharedMem::Create(SHM_KEY, List<TestNode, SharedMem>::CalcByteSize(10)));
    LSF_ASSERT(list.BindAndInitStorage(SharedMem(SHM_KEY)));
    LSF_ASSERT(list.max_size() == 10);

    LSF_ASSERT(list.PushBack(TestNode(1, 1)));
    LSF_ASSERT(list.PushBack(TestNode(2, 2)));
    LSF_ASSERT(list.PushBack(TestNode(3, 3)));
    LSF_ASSERT(list.PushBack(TestNode(4, 4)));
    LSF_ASSERT(list.PushBack(TestNode(5, 5)));  // 1 2 3 4 5

    // test insert
    LSF_ASSERT(list.Insert(list.begin(), TestNode(9, 9)));  // 9 1 2 3 4 5
    LSF_ASSERT(*list.begin() == TestNode(9, 9));
    LSF_ASSERT(*++list.begin() == TestNode(1, 1));

    LSF_ASSERT(list.Insert(++list.begin(), TestNode(8, 8)));  // 9 8 1 2 3 4 5
    LSF_ASSERT(*list.begin() == TestNode(9, 9));
    LSF_ASSERT(*++list.begin() == TestNode(8, 8));

    // test find
    LSF_ASSERT(list.Find(TestNode(9, 9)) == list.begin());
    LSF_ASSERT(list.Find(TestNode(8, 8)) == list.begin() + 1);
    LSF_ASSERT(list.Find(TestNode(1, 1)) == list.begin() + 2);
    LSF_ASSERT(list.Find(TestNode(2, 2)) == list.begin() + 3);
    LSF_ASSERT(list.Find(TestNode(3, 3)) == list.begin() + 4);
    LSF_ASSERT(list.Find(TestNode(4, 4)) == list.begin() + 5);
    LSF_ASSERT(list.Find(TestNode(5, 5)) == list.begin() + 6);

    // test erase
    LSF_ASSERT(list.Erase(++list.begin()));
    LSF_ASSERT(*list.begin() == TestNode(9, 9));
    LSF_ASSERT(*++list.begin() == TestNode(1, 1));

    LSF_ASSERT(list.Erase(list.begin()));
    LSF_ASSERT(*list.begin() == TestNode(1, 1));

    LSF_ASSERT(list.Erase(list.begin(), list.end()));

    LSF_ASSERT(list.empty());
}

int main(int argc, char **argv) {
    LSF_TEST_ALL(argc, argv);
    SharedMem::Delete(SHM_KEY);
}

// vim:ts=4:sw=4:et:ft=cpp:
