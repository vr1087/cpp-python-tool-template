/**
 * @file main.cpp
 * @brief Command-line front-end for the Aligncount placeholder tool.
 *
 * The executable parses options with **cxxopts**, validates that the input file
 * exists, runs ::aligncount::Aligncount, prints the number of records, and
 * returns `0` on success.
 */

#include <aligncount/aligncount.h>
#include <aligncount/version.h>

#include <cxxopts.hpp>
#include <filesystem>
#include <iostream>
#include <string>

/**
 * @brief Entry point of the standalone Aligncount tool.
 *
 * Supported options:
 * * `-a / --alignments <file>` – path to SAM/BAM/CRAM file (required).
 * * `-h / --help`              – show usage.
 * * `-v / --version`           – print version string.
 *
 * @return `EXIT_SUCCESS` (`0`) on success, non-zero on error.
 */
auto main(int argc, char** argv) -> int {
  cxxopts::Options options(*argv, "A program to count alignment records");

  std::string alignments;

  // clang-format off
  options.add_options()
    ("h,help",    "Show help")
    ("v,version", "Print the current version number")
    ("a,alignments",
        "Path to SAM/BAM/CRAM file.",
        cxxopts::value(alignments)->default_value(""))
  ;
  // clang-format on

  const auto result = options.parse(argc, argv);

  /* ------------------------ handle --help / --version ---------------------- */
  if (result["help"].as<bool>()) {
    std::cout << options.help() << '\n';
    return 0;
  }
  if (result["version"].as<bool>()) {
    std::cout << "Aligncount, version " << ALIGNCOUNT_VERSION << '\n';
    return 0;
  }

  /* ---------------------------- validate input ----------------------------- */
  if (alignments.empty()) {
    std::cout << "alignments file not specified!\n";
    return 1;
  }
  std::filesystem::path p = alignments;
  if (!std::filesystem::exists(p) || !std::filesystem::is_regular_file(p)) {
    std::cout << "File " << p << " does not exist!\n";
    return 1;
  }

  /* ----------------------- perform the record count ------------------------ */
  aligncount::Aligncount counter(alignments);
  std::cout << counter.count_records() << '\n';

  return 0;
}
