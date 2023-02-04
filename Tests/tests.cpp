#include <gtest/gtest.h>

#include "GraphicsTests.h"
#include "LibTests.h"

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest();

	return RUN_ALL_TESTS();
}

