#include <gtest/gtest.h>

#include "GraphicsTests.h"
#include "LibTests.h"

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
#else
int main(int argc, char* argv[])
#endif
{
	::testing::InitGoogleTest();

	return RUN_ALL_TESTS();
}

