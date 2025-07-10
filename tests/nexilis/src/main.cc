#include <gtest/gtest.h>
#include <nexilis/ci.hh>

int main(int argc, char** argv)
{
    std::cout << "Running tests in " << (nexilis::isRunningInCI() ? "ci" : "local") << " environment." << std::endl;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
