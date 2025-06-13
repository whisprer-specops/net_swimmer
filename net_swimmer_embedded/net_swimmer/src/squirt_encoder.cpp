
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <filesystem>
#include <cstdint>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Output params
const int SAMPLE_RATE = 44100;
const double DURATION = 0.1; // 100ms per tone

// Frequencies assigned to base64 characters
std::map<char, double> char_freq_map;

void init_freq_map() {
    std::string base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    double base_freq = 1000.0; // Start at 1kHz
    double step = 30.0;        // Step per char
    for (size_t i = 0; i < base64.size(); ++i) {
        char_freq_map[base64[i]] = base_freq + i * step;
    }
}

std::vector<int16_t> generate_chirp(double freq_start, double freq_end) {
    std::vector<int16_t> buffer;
    int num_samples = int(SAMPLE_RATE * DURATION);
    for (int i = 0; i < num_samples; ++i) {
        double t = static_cast<double>(i) / SAMPLE_RATE;
        double freq = freq_start + (freq_end - freq_start) * (t / DURATION);
        double sample = sin(2.0 * M_PI * freq * t);
        buffer.push_back(static_cast<int16_t>(sample * 32767));
    }
    return buffer;
}

std::vector<int16_t> encode_string_as_tone(const std::string& input) {
    std::vector<int16_t> waveform;
    for (char c : input) {
        double freq = char_freq_map[c];
        auto chirp = generate_chirp(freq, freq + 100.0); // Short upsweep
        waveform.insert(waveform.end(), chirp.begin(), chirp.end());
    }
    return waveform;
}

void write_wav(const std::string& filename, const std::vector<int16_t>& samples) {
    std::ofstream out(filename, std::ios::binary);

    int byte_rate = SAMPLE_RATE * 2;
    int block_align = 2;
    int data_chunk_size = samples.size() * 2;
    int file_size = 36 + data_chunk_size;

    // RIFF header
    out.write("RIFF", 4);
    out.write(reinterpret_cast<const char*>(&file_size), 4);
    out.write("WAVE", 4);

    // fmt chunk
    out.write("fmt ", 4);
    int fmt_size = 16;
    short audio_format = 1; // PCM
    short num_channels = 1;
    short bits_per_sample = 16;
    out.write(reinterpret_cast<const char*>(&fmt_size), 4);
    out.write(reinterpret_cast<const char*>(&audio_format), 2);
    out.write(reinterpret_cast<const char*>(&num_channels), 2);
    out.write(reinterpret_cast<const char*>(&SAMPLE_RATE), 4);
    out.write(reinterpret_cast<const char*>(&byte_rate), 4);
    out.write(reinterpret_cast<const char*>(&block_align), 2);
    out.write(reinterpret_cast<const char*>(&bits_per_sample), 2);

    // data chunk
    out.write("data", 4);
    out.write(reinterpret_cast<const char*>(&data_chunk_size), 4);
    out.write(reinterpret_cast<const char*>(samples.data()), data_chunk_size);
}

std::string read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::string base64_encode(const std::string& data) {
    static const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    int val = 0, valb = -6;
    for (unsigned char c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) result.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (result.size() % 4) result.push_back('=');
    return result;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: squirt_encoder <input_blob.json> <output.wav>\n";
        return 1;
    }

    init_freq_map();

    std::string blob = read_file(argv[1]);
    std::string b64 = base64_encode(blob);

    auto waveform = encode_string_as_tone(b64);
    write_wav(argv[2], waveform);

    std::cout << "[+] Encoded " << b64.size() << " base64 chars into tone stream\n";
    return 0;
}
