#include <gtest/gtest.h>

#include "GraphicsTests.h"
#include "LibTests.h"

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

