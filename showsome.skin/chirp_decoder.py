import wave
import numpy as np
import zlib
import json
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

# CONFIG
KEY = b"ThisIsASecretKey_"  # 16-byte key
IV = b"InitializationVe"    # 16-byte IV

# Load chirp
with wave.open("chirp_with_payload.wav", "rb") as wav_in:
    params = wav_in.getparams()
    audio = wav_in.readframes(params.nframes)

samples = np.frombuffer(audio, dtype=np.int16)

# LSB decode
bitstream = ''.join([str(s & 1) for s in samples])
bytes_out = bytearray(int(bitstream[i:i+8], 2) for i in range(0, len(bitstream), 8) if len(bitstream[i:i+8]) == 8)

# Decrypt + decompress
cipher = AES.new(KEY, AES.MODE_CBC, IV)
decrypted = cipher.decrypt(bytes(bytes_out))
decompressed = zlib.decompress(unpad(decrypted, AES.block_size))

# Output
data = json.loads(decompressed.decode())
print(json.dumps(data, indent=4))
