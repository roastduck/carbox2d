import sys
import random
import itertools
import tensorflow as tf
import numpy as np

sys.stderr.write("Python script is running...\n");

CHROMOS_SIZE = 40
LEARNING_RATE = 5e-4
BATCH_SIZE = 32

def simulate(chromos):
    assert len(chromos) == CHROMOS_SIZE
    assert chromos.dtype == np.float32
    # Don't use `chromos.tofile`, which requires the file handler to be seekable
    sys.stdout.buffer.write(chromos.tobytes())
    sys.stdout.flush()

    result = np.fromstring(sys.stdin.buffer.read(2 * 4), dtype=np.float32)
    return result[0], result[1]

def randomChromos():
    # Not that this chromosome is normalized, which is different from the website
    # See algorithm.cpp for details
    # We restrict the range to [0.01, 0.99] to avoid some chromosome that lead to assertion faults
    return np.array([random.uniform(0.01, 0.99) for i in range(CHROMOS_SIZE)], np.float32)

def net(chrIn, reuse):
    x = tf.layers.dense(chrIn, 128, kernel_initializer=tf.initializers.random_normal(0, 0.01), name="FC1", reuse=reuse)
    x = tf.nn.relu(x)
    x = tf.layers.dense(x, 128, kernel_initializer=tf.initializers.random_normal(0, 0.01), name="FC2", reuse=reuse)
    x = tf.nn.relu(x)
    x = tf.layers.dense(x, 1, kernel_initializer=tf.initializers.random_normal(0, 0.01), name="FC3", reuse=reuse)
    x = tf.reshape(x, (-1,))
    return x

trainIn = tf.placeholder(tf.float32, (None, CHROMOS_SIZE), "trainIn")
trainOut = net(trainIn, reuse=False)
label = tf.placeholder(tf.float32, (None,), "label")
diff = trainOut - label
loss = tf.reduce_mean(diff * diff)
opt = tf.train.AdamOptimizer(LEARNING_RATE).minimize(loss)

infIn = tf.placeholder(tf.float32, (None, CHROMOS_SIZE), "infIn")
infOut = net(infIn, reuse=True)
dIn = tf.gradients(infOut, (infIn,))[0]
improve = tf.nn.l2_normalize(dIn) * 0.01

chromos = [randomChromos() for i in range(BATCH_SIZE)]

sess = tf.Session()
sess.run(tf.global_variables_initializer())

for generation in itertools.count():
    # Perceive
    scores = []
    for c in chromos:
        score, time = simulate(c)
        sys.stderr.write("> score = %f, time = %f\n"%(score, time))
        scores.append(score)
    curLoss, _ = sess.run((loss, opt), {trainIn: chromos, label: scores})
    sys.stderr.write("Generation #%d: avg score = %f\n"%(generation, np.mean(scores)))
    sys.stderr.write("Generation #%d: max score = %f\n"%(generation, np.max(scores)))
    sys.stderr.write("Generation #%d: loss = %f\n"%(generation, np.max(curLoss)))

    # Action
    median = np.median(scores)
    delta = sess.run(improve, {infIn: chromos})
    for i in range(BATCH_SIZE):
        if scores[i] >= median:
            chromos[i] = np.minimum(np.maximum(chromos[i] + delta[i], 0.01), 0.99)
        else:
            chromos[i] = randomChromos()

