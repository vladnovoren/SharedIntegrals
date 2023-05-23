#include <gmock/gmock.h>
#include <fstream>
#include <gtest/gtest.h>

static constexpr double answers[] = {
  2.4674,
  0.949004,
  3267.24,
  1.70073,
  12.3138,
  24.3523
};

TEST(SharedIntegrals, Results) {
  for (int i = 0; i < 6; ++i) {
    std::string cmd = "./multi " + std::to_string(i);
    std::system(cmd.c_str());
  }

  std::ifstream out;
  out.open("out.txt");
  for (int i = 0; i < 6; ++i) {
    double res;
    out >> res;
    ASSERT_TRUE(fabs(res - answers[i]) < 0.01);
  }
  out.close();
  std::remove("out.txt");
}
