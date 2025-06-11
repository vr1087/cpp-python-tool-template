#include <aligncount/aligncount.h>
#include <aligncount/version.h>
#include <doctest/doctest.h>

#include <filesystem>
#include <fstream>

TEST_CASE("Aligncount counts two records in a SAM file") {
  using namespace aligncount;

  const std::string sam_path = "sample.sam";

  // ── 1. create the file ────────────────────────────────────────────
  {
    std::ofstream sam(sam_path);
    REQUIRE(sam);  // abort test if file couldn’t be opened

    sam << "@HD\tVN:1.6\tSO:coordinate\n"
        << "@SQ\tSN:chr1\tLN:1000\n"
        << "read1\t0\tchr1\t1\t60\t10M\t*\t0\t0\tACTGACTGAC\t*\n"
        << "read2\t0\tchr1\t20\t60\t10M\t*\t0\t0\tACTGACTGAC\t*\n";
  }

  // ── 2. run Aligncount ────────────────────────────────────────────
  Aligncount ac(sam_path);
  const auto n_records = ac.count_records();

  CHECK(n_records == 2);

  // ── 3. clean-up ──────────────────────────────────────────────────
  std::filesystem::remove(sam_path);
}

TEST_CASE("Aligncount version") {
  static_assert(std::string_view(ALIGNCOUNT_VERSION) == std::string_view("1.0"));
  CHECK(std::string(ALIGNCOUNT_VERSION) == std::string("1.0"));
}