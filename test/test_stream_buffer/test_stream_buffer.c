#include "test.h"

void test_stream_buffer()
{
	UNIT_Notify();
	TEST_Add(test_stream_buffer_1);
#ifndef __CSMC__
	TEST_Add(test_stream_buffer_2);
	TEST_Add(test_stream_buffer_3);
#endif
}
