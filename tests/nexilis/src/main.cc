#include <gtest/gtest.h>
#include <nexilis/environment.hh>

int main(int argc, char** argv)
{
    auto env = nexilis::envToString(nexilis::detectRuntimeType());
    std::cout << "Running tests in " << env << " environment." << std::endl;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
