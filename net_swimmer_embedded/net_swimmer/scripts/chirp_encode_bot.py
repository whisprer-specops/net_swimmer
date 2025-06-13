import os
import time
import zlib
import json
import wave
import numpy as np
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
from pydub import AudioSegment

# === CONFIG ===
KEY = b"ThisIsASecretKey_"           # 16-byte AES key
IV = b"InitializationVe"             # 16-byte IV
MUSIC_FILE = "music.wav"
OUTPUT_FILE = "chirp_with_payload.wav"
DATA_DIR = "data_outbox"
LOOP_INTERVAL = 60  # seconds

# === UTIL ===
def encrypt_and_compress(json_data):
    raw = json.dumps(json_data).encode()
    compressed = zlib.compress(raw)
    cipher = AES.new(KEY, AES.MODE_CBC, IV)
    encrypted = cipher.encrypt(pad(compressed, AES.block_size))
    return encrypted

def encode_lsb(audio_data, payload):
    samples = np.frombuffer(audio_data, dtype=np.int16)
    payload_bits = ''.join(f"{byte:08b}" for byte in payload)
    if len(payload_bits) > len(samples):
        raise ValueError("Payload too large for host audio.")
    for i, bit in enumerate(payload_bits):
        samples[i] = (samples[i] & ~1) | int(bit)
    return samples.tobytes()

def embed_chirp_into_music(json_data, music_path, output_path):
    encrypted = encrypt_and_compress(json_data)

    music = AudioSegment.from_wav(music_path).set_channels(1)
    raw = music.raw_data
    encoded = encode_lsb(raw, encrypted)

    with wave.open(output_path, "wb") as out_wav:
        out_wav.setnchannels(1)
        out_wav.setsampwidth(music.sample_width)
        out_wav.setframerate(music.frame_rate)
        out_wav.writeframes(encoded)

# === LOOPING BOT ===
def run_chirp_bot():
    print("[*] Chirp encoder bot starting...")
    os.makedirs(DATA_DIR, exist_ok=True)

    while True:
        files = sorted([f for f in os.listdir(DATA_DIR) if f.endswith(".json")])
        if not files:
            print("[*] No JSON payloads. Sleeping.")
            time.sleep(LOOP_INTERVAL)
            continue

        for fname in files:
            try:
                with open(os.path.join(DATA_DIR, fname), "r") as f:
                    json_data = json.load(f)

                outname = f"chirped_{fname.replace('.json','')}.wav"
                outpath = os.path.join(DATA_DIR, outname)

                embed_chirp_into_music(json_data, MUSIC_FILE, outpath)
                print(f"[+] Encoded {fname} → {outname}")

                os.remove(os.path.join(DATA_DIR, fname))  # mark as done

            except Exception as e:
                print(f"[!] Failed to encode {fname}: {e}")
        time.sleep(LOOP_INTERVAL)

# === ENTRY POINT ===
if __name__ == "__main__":
    run_chirp_bot()
