#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <zlib.h>
#include "chirp_encoder.h"

std::vector<unsigned char> compress_gzip(const std::string& data) {
    std::vector<unsigned char> compressed(data.size() * 1.1 + 12); // max compressed size heuristic
    z_stream zs{};
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;

    if (deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
        throw std::runtime_error("deflateInit2 failed");

    zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(data.data()));
    zs.avail_in = data.size();
    zs.next_out = compressed.data();
    zs.avail_out = compressed.size();

    if (deflate(&zs, Z_FINISH) != Z_STREAM_END)
        throw std::runtime_error("deflate failed");

    compressed.resize(zs.total_out);
    deflateEnd(&zs);
    return compressed;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: json_to_chirp <input.json> <output.wav>\n";
        return 1;
    }

    // Load JSON
    std::ifstream in(argv[1]);
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string json_data = buffer.str();

    // Compress
    std::vector<unsigned char> compressed = compress_gzip(json_data);

    // Encode into chirps
    std::vector<short> chirp_samples = encode_data_to_chirps(compressed);

    // Write .wav
    write_wav(argv[2], chirp_samples);

    std::cout << "[+] Wrote encoded chirp to " << argv[2] << "\n";
    return 0;
}
