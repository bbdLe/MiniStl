objects = main.o

test : $(objects)
	   clang++ $(objects) -std=c++11 -o test

main.o : Test.cpp ListTest.h VectorTest.h SlistTest.h DequeTest.h QueueTest.h StackTest.h Priority_queueTest.h AlgoTest.h \
SetTest.h MapTest.h MultimapTest.h MultisetTest.h HashsetTest.h HashmultisetTest.h HashmapTest.h HashmultimapTest.h AlgobaseTest.h AlgorithmTest.h BasestringTest.h\
../Hashset.h ../Map.h ../Vector.h ../List.h ../Slist.h ../Deque.h ../Queue.h ../Stack.h ../Priority_queue.h ../Algorithm.h ../Basestring.h\
../Set.h ../Multimap.h ../Multiset.h ../Hashmultiset.h ../Hashtable.h ../Hashmap.h ../Hashmultimap.h ../Algobase.h ../Algo.h
		 clang++ -c Test.cpp -std=c++11 -o main.o

.PHONY : clean

clean :
		-rm test $(objects)
