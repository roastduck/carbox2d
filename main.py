import os
import sys
import random
import pickle
import itertools
import tensorflow as tf
import numpy as np

sys.stderr.write("Python script is running...\n");

CHROMOS_SIZE = 40
LEARNING_RATE = 1e-4
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

def shuffle(sample, label):
    assert len(sample) == len(label)
    arr = list(zip(sample, label))
    random.shuffle(arr)
    return zip(*arr)

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
saver = tf.train.Saver(max_to_keep=1)
if os.environ['CARBOX2D_RESTORE'] == 'NO':
    sys.stderr.write("Creating clean variables")
    sess.run(tf.global_variables_initializer())
    allChromos = []
    allScores = []
else:
    sys.stderr.write("Restoring checkpoint")
    saver.restore(sess, tf.train.latest_checkpoint('./checkpoint'))
    with open('./checkpoint/extra.data', 'rb') as f:
        allChromos, allScores = pickle.load(f)

for generation in itertools.count():
    # Perceive
    scores = []
    for c in chromos:
        score, time = simulate(c)
        sys.stderr.write("> score = %f, time = %f\n"%(score, time))
        scores.append(score)
    scores = np.array(scores, dtype=np.float32)
    sys.stderr.write("Generation #%d: avg score = %f\n"%(generation, np.mean(scores)))
    sys.stderr.write("Generation #%d: max score = %f\n"%(generation, np.max(scores)))
    allChromos, allScores = shuffle(list(allChromos) + list(chromos), list(allScores) + list(scores / 1000))
    avgLoss = 0.
    for i in range(len(allChromos) // BATCH_SIZE):
        chromosIn = allChromos[i * BATCH_SIZE : (i + 1) * BATCH_SIZE]
        scoresIn = allScores[i * BATCH_SIZE : (i + 1) * BATCH_SIZE]
        curLoss, _ = sess.run((loss, opt), {trainIn: chromosIn, label: scoresIn})
        avgLoss += curLoss
    avgLoss /= len(allChromos) // BATCH_SIZE
    sys.stderr.write("Generation #%d: loss = %f\n"%(generation, np.max(avgLoss)))
    with open('./checkpoint/extra.data', 'wb') as f:
        pickle.dump((allChromos, allScores), f)
    saver.save(sess, './checkpoint/params')

    # Action
    median = np.median(scores)
    delta = sess.run(improve, {infIn: chromos})
    for i in range(BATCH_SIZE):
        if scores[i] >= median:
            chromos[i] = np.minimum(np.maximum(chromos[i] + delta[i], 0.01), 0.99)
        else:
            chromos[i] = randomChromos()

