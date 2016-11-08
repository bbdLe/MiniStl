#include"VectorTest.h"
#include"ListTest.h"
#include"SlistTest.h"
#include"DequeTest.h"
#include"QueueTest.h"
#include"StackTest.h"
#include"Priority_queueTest.h"
#include"SetTest.h"
#include"MapTest.h"
#include"MultimapTest.h"
#include"MultisetTest.h"
#include"HashsetTest.h"
#include"HashmultisetTest.h"
#include"HashmapTest.h"
#include"HashmultimapTest.h"
#include"AlgobaseTest.h"
#include"AlgoTest.h"
#include"AlgorithmTest.h"

int main(int argc, char **argv)
{
    VectorTest vector_Test;
    ListTest list_Test;
    SlistTest slist_Test;
    DequeTest deque_Test;
    QueueTest queue_Test;
    StackTest stack_Test;
    Priority_queueTest pqueue_Test;
    SetTest set_test;
    MapTest map_test;
    MultimapTest mmap_test;
    MultisetTest mset_test;
    HashsetTest hset_test;
    HashmultisetTest hmset_test;
    HashmapTest hmap_test;
    HashmultimapTest hmmap_test;
    AlgobaseTest algobase_test;
    AlgoTest algo_test;
    AlgorithmTest algorithm_test;

    vector_Test.test();
    list_Test.test();
    slist_Test.test();
    deque_Test.test();
    queue_Test.test();
    stack_Test.test();
    pqueue_Test.test();
    set_test.test();
    map_test.test();
    mmap_test.test();
    mset_test.test();
    hset_test.test();
    hmset_test.test();
    hmap_test.test();
    hmmap_test.test();
    algobase_test.test();
    algo_test.test();
    algorithm_test.test();
}
