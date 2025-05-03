#include <iostream>
#include <string>
#include <intrin.h>
#include "ipp.h"

using namespace std;

string serialEncode(const string&);
string parallelEncode(const string&);
string generate_sample(const string&, int);
void write_report(const string&, const string&, const string&, Ipp64u, Ipp64u);

int main(int argc, char** argv) {
   Ipp64u serialStart, serialEnd, serialTime;
   Ipp64u parallelStart, parallelEnd, parallelTime;

   string sample = generate_sample(argv[1], stoi(argv[2]));

   serialStart = ippGetCpuClocks();
   string s_result = serialEncode(sample);
   serialEnd = ippGetCpuClocks();
   serialTime = serialEnd - serialStart;

   parallelStart = ippGetCpuClocks();
   string p_result = parallelEncode(sample);
   parallelEnd = ippGetCpuClocks();
   parallelTime = parallelEnd - parallelStart;

   write_report(sample, s_result, p_result, serialTime, parallelTime);

   return 0;
}

string generate_sample(const string& base, int repeat) {
    string sample = "";
    for (int i = 0; i < repeat; i++) {
        sample += base;
    }
    return sample;
}

void write_report(const string& sample, const string& s_result, const string& p_result, Ipp64u s_time, Ipp64u p_time) {
    cout << "Serial Run time = " << (Ipp32s)s_time << " cycles" << endl;
    cout << "Serial, lentgh after compression: " << s_result.size() << endl << endl;
    cout << "Parallel Run time = " << (Ipp32s)p_time << " cycles" << endl;
    cout << "Parallel, lentgh after compression: " << p_result.size() << endl << endl;
    cout << "Compressing Ratio: " << double(sample.size()) / s_result.size() << endl;
    cout << "Speedup = " << static_cast<float>(s_time) / static_cast<float>(p_time) << endl;
}

string serialEncode(const string& base_string) {
    int base_string_size = base_string.size();
    string result = "";
    if (base_string_size == 0) {
        return result;
    }

    int run_length = 1;
    char run_value = base_string[0];
    for (int i = 1; i < base_string_size; i++) {
        if (run_value == base_string[i]) {
            run_length++;
        }
        else {
            result += run_value;
            result += to_string(run_length);
            run_length = 1;
            run_value = base_string[i];
        }
    }

    result += run_value;
    result += to_string(run_length);

    return result;
}

string parallelEncode(const string& base_string) {
    int base_string_size = base_string.size();
    string result = "";
    if (base_string_size == 0) {
        return result;
    }

    int i;
    int run_length = 1;
    char prev_loop_last_value = base_string[0];
    for (i = 1; i + 15 <= base_string_size; i += 15) {
        __m128i curr_chunk = _mm_load_si128(reinterpret_cast<const __m128i*>(&base_string[i]));
        __m128i next_chunk = _mm_srli_si128(curr_chunk, 1);
        __m128i cmp_mask = _mm_cmpeq_epi8(curr_chunk, next_chunk);

        int mask = _mm_movemask_epi8(cmp_mask);

        char run_value = base_string[i];
        if (run_value == prev_loop_last_value) {
            run_length++;
        }
        else {
            result += prev_loop_last_value;
            result += to_string(run_length);
            run_length = 1;
        }

        for (int j = 0; j < 14; j++) {
            if ((mask & (1 << j)) != 0) {
                run_length++;
            }
            else {
                result += run_value;
                result += to_string(run_length);
                run_length = 1;
                run_value = base_string[i + j + 1];
            }
        }

        prev_loop_last_value = base_string[i + 14];
    }

    char run_value = prev_loop_last_value;
    for (; i < base_string_size; i++) {
        if (run_value == base_string[i]) {
            run_length++;
        }
        else {
            result += run_value;
            result += to_string(run_length);
            run_length = 1;
            run_value = base_string[i];
        }
    }

    result += run_value;
    result += to_string(run_length);

    return result;
}
