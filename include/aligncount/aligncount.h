#pragma once
/**
 * @file aligncount.h
 * @brief Declaration of the ::aligncount::Aligncount class.
 *
 * The class is a tiny façade around HTSlib that opens a SAM/BAM/CRAM file
 *     and returns the number of alignment records it contains.
 *
 * @copyright
 * Copyright © 2025
 * Distributed under the MIT License (see accompanying file LICENSE).
 */

#include <cstdint>  ///< uint64_t
#include <string>

namespace aligncount {

  /**
   * @class Aligncount
   * @brief Lightweight helper to count alignment records in a file.
   *
   * Usage example:
   * @code
   *   aligncount::Aligncount counter("/data/reads.sorted.bam");
   *   std::cout << counter.count_records() << '\n';
   * @endcode
   *
   * The class deliberately owns *only* the path string; all HTSlib resources
   *     are created and destroyed inside the @ref count_records call.
   */
  class Aligncount {
    std::string m_alignment_file;  ///< Path to the SAM/BAM/CRAM file.

  public:
    /**
     * @brief Construct a counter for the given alignment file.
     * @param file_path Absolute or relative path to a SAM/BAM/CRAM file.
     */
    explicit Aligncount(std::string file_path);

    /**
     * @brief Count primary alignment records using HTSlib.
     *
     * The program terminates via `std::exit(EXIT_FAILURE)` if the file cannot
     * be opened or its header cannot be read.
     *
     * @return Number of alignment records (`uint64_t`).
     *
     * @note The result is annotated with `[[nodiscard]]`; ignoring it will
     *       trigger a compiler warning when supported by your tool-chain.
     */
    [[nodiscard]] uint64_t count_records() const;
  };

}  // namespace aligncount
