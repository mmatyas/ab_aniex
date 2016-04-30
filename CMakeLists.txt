cmake_minimum_required(VERSION 2.8)
project(ABAniExtract)

include(TestBigEndian)
test_big_endian(IS_BIG_ENDIAN)
if (IS_BIG_ENDIAN)
	add_definition(-DBYTEORDER_BIG_ENDIAN)
endif()

add_subdirectory(src)
