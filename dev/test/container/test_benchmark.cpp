// File:        test_benchmark.cpp
// Description: ---
// Notes:       ---
// Author:      leoxiang <leoxiang@tencent.com>
// Revision:    2012-11-07 by leoxiang

#include <set>
#include <fstream>
#include "lsf/basic/unit_test.hpp"
#include "lsf/basic/type_cast.hpp"
#include "lsf/util/random.hpp"
#include "lsf/container/set.hpp"
#include "lsf/container/heap_mem.hpp"
#include "node.hpp"

using namespace std;
using namespace lsf::basic;
using namespace lsf::container;

static string conf_path;

#define TREE_SIZE 800000

LSF_TEST_CASE(test_stl_rbtree)
{
    time_t st_time = time(NULL);
    set<uint32_t> sets;

    // insert
    ifstream ifs(conf_path.c_str());
    LSF_ASSERT(ifs.is_open());
    for (string line; !ifs.eof(); getline(ifs, line)) {
        if (line.empty()) continue;
        LSF_ASSERT_ERR_ONLY(sets.insert(TypeCast<uint32_t>(line)).second);
    }
    LSF_ASSERT(sets.size() == TREE_SIZE);
    
    // erase
    ifs.close();
    ifs.open(conf_path.c_str());
    LSF_ASSERT(ifs.is_open());
    for (string line; !ifs.eof(); getline(ifs, line)) {
        if (line.empty()) continue;
        LSF_ASSERT_EXIT_ERR_ONLY(sets.erase(TypeCast<uint32_t>(line)));
    }
    LSF_ASSERT(sets.size() == 0);

    // calc time
    cout << "total time is: " << time(NULL) - st_time << endl;
}

LSF_TEST_CASE(test_lsf_rbtree)
{
    time_t st_time = time(NULL);
    Set<uint32_t, HeapMem> sets;
    LSF_ASSERT(sets.BindAndInitStorage(HeapMem(sets.CalcByteSize(TREE_SIZE))));

    // insert
    ifstream ifs(conf_path.c_str());
    LSF_ASSERT(ifs.is_open());
    for (string line; !ifs.eof(); getline(ifs, line)) {
        if (line.empty()) continue;
        LSF_ASSERT_ERR_ONLY(sets.Insert(TypeCast<uint32_t>(line)));
    }
    LSF_ASSERT(sets.Size() == TREE_SIZE);
    
    // erase
    ifs.close();
    ifs.open(conf_path.c_str());
    LSF_ASSERT(ifs.is_open());
    for (string line; !ifs.eof(); getline(ifs, line)) {
        if (line.empty()) continue;
        LSF_ASSERT_EXIT_ERR_ONLY(sets.Erase(TypeCast<uint32_t>(line)));
    }
    LSF_ASSERT(sets.Size() == 0);

    // calc time
    cout << "total time is: " << time(NULL) - st_time << endl;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " [conf_path]" << endl;
        exit(0);
    }
    else {
        conf_path = argv[1];
    }

	LSF_TEST_ALL();
}

// vim:ts=4:sw=4:et:ft=cpp:
