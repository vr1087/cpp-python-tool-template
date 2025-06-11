/**
 * @file aligncount.cpp
 * @brief Definition of ::aligncount::Aligncount methods.
 */

#include <aligncount/aligncount.h>
#include <fmt/format.h>

#include <cstdlib>  // std::exit
#include <iostream>
#include <string>

// ─── HTSlib ───────────────────────────────────────────────────────────
extern "C" {
#include <htslib/sam.h>
}

using namespace aligncount;

/* -------------------------------------------------------------------------- */
/* Constructor                                                                */
/* -------------------------------------------------------------------------- */
/// @cond doxygen_skip
Aligncount::Aligncount(std::string file_path) : m_alignment_file(std::move(file_path)) {}
/// @endcond

/* -------------------------------------------------------------------------- */
/* count_records                                                              */
/* -------------------------------------------------------------------------- */
/**
 * @details
 * 1.  Opens the file in read-only mode (`sam_open`).\n
 * 2.  Reads the header (`sam_hdr_read`).\n
 * 3.  Iterates over every record (`sam_read1`) and increments a counter.\n
 * 4.  Cleans up HTSlib structures before returning.
 *
 * Any I/O failure prints a diagnostic to `stderr` and exits the program with
 * `EXIT_FAILURE`, because this placeholder tool is not intended for use as a
 * reusable library.
 */
uint64_t Aligncount::count_records() const {
  const char* fname = m_alignment_file.c_str();

  // 1) Open the SAM/BAM/CRAM file
  htsFile* fp = sam_open(fname, "r");
  if (!fp) {
    std::cerr << fmt::format("cannot open \"{}\"", m_alignment_file) << '\n';
    std::exit(EXIT_FAILURE);
  }

  // 2) Read the header
  bam_hdr_t* hdr = sam_hdr_read(fp);
  if (!hdr) {
    std::cerr << fmt::format("failed to read header in \"{}\"", m_alignment_file) << '\n';
    sam_close(fp);
    std::exit(EXIT_FAILURE);
  }

  // 3) Prepare alignment record and iterate
  bam1_t* rec = bam_init1();
  uint64_t count = 0;
  while (sam_read1(fp, hdr, rec) >= 0) {
    ++count;
  }

  // 4) Clean up
  bam_destroy1(rec);
  bam_hdr_destroy(hdr);
  sam_close(fp);

  return count;
}
