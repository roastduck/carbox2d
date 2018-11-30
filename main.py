import sys
import numpy as np

sys.stderr.write("Python script is running...\n");

# Not that this chromosome is normalized, which is different from the website
# See algorithm.cpp for details
chromos = np.array([
    1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0,
    0.01, 0.5, 0.5, 0.49, 0.5, 0.5, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
], np.float32) # Must be float32 to output

def simulate(chromos):
    assert len(chromos) == 40
    assert chromos.dtype == np.float32
    # Don't use `chromos.tofile`, which requires the file handler to be seekable
    sys.stdout.buffer.write(chromos.tobytes())
    sys.stdout.flush()

    result = np.fromstring(sys.stdin.buffer.read(2 * 4), dtype=np.float32)
    return result[0], result[1]

while True:
    score, time = simulate(chromos)
    sys.stderr.write("score = %s, time = %s\n"%(score, time))

