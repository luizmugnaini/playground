#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace day1 {
    constexpr const char* kDigitStrRepr[9] = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
    };

    /**
     * On each line of the input, a calibration value can be computed by combining the first and
     * last digits to form a single two-digit number (the calibration value corresponding to that
     * line). Return the sum of all the calibration values.
     */
    static void part1() {
        const char* inputPath = "aoc23/input/day1_part1.txt";
        FILE*       file      = fopen(inputPath, "rb");
        if (file == nullptr) {
            fprintf(stderr, "Couldn't open file %s.\n", inputPath);
            return;
        }

        long acc              = 0;
        int  currChar         = 0;
        int  lastNum          = 0;
        bool searchFirstDigit = true;
        while ((currChar = fgetc(file)) != EOF) {
            if (currChar >= '1' && currChar <= '9') {
                lastNum = currChar - '0';
                if (searchFirstDigit) {
                    acc += 10 * lastNum;
                    searchFirstDigit = false;
                }
            } else if (currChar == '\n') {
                acc += lastNum;
                searchFirstDigit = true;
            }
        }
        printf("[AOC23 :: Day 1 :: Part 1] %ld\n", acc);
    }

    /**
     * On each line of the input, a calibration value can be computed by combining the first and last
     * numeric ('1', '2', '3', '4', '5', '6', '7', '8', '9'), or string ("one", "two", "three", "four",
     * "five, "six", "seven", "eight", and "nine") representations of a digit to form a single two-digit
     * number (the calibration value corresponding to that line). Return the sum of all the calibration
     * values.
     */
    static void part2() {
        const char* inputPath = "aoc23/input/day1_part2.txt";
        FILE*       file      = fopen(inputPath, "rb");
        if (file == nullptr) {
            fprintf(stderr, "Couldn't open file %s.\n", inputPath);
            return;
        }

        fseek(file, 0, SEEK_END);
        int64_t fileLen = ftell(file);
        fseek(file, 0, SEEK_SET);

        size_t bufLen    = static_cast<size_t>(fileLen);
        char*  buf       = new char[bufLen + 1];
        size_t readCount = fread(buf, 1, bufLen, file);
        if (ferror(file) != 0) {
            fprintf(stderr, "Error while reading file.\n");
        } else {
            buf[readCount] = '\0';
        }
        fclose(file);

        long acc              = 0;
        int  lastNum          = 0;
        bool searchFirstDigit = true;
        for (size_t idx = 0; idx < readCount; idx++) {
            if (buf[idx] == '\n') {
                acc += lastNum;
                searchFirstDigit = true;
            } else if (buf[idx] >= '0' && buf[idx] <= '9') {
                lastNum = buf[idx] - '0';
                if (searchFirstDigit) {
                    acc += 10 * lastNum;
                    searchFirstDigit = false;
                }
            } else {
                for (int i = 0; i < 9; i++) {
                    if (strncmp(buf + idx, kDigitStrRepr[i], strlen(kDigitStrRepr[i])) == 0) {
                        lastNum = i + 1;
                        if (searchFirstDigit) {
                            acc += 10 * lastNum;
                            searchFirstDigit = false;
                        }
                    }
                }
            }
        }
        delete[] buf;
        printf("[AOC23 :: Day 1 :: Part 2] %ld\n", acc);
    }
}  // namespace day1
