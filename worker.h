#ifndef WORKER_H_
#define WORKER_H_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>

class Record {
  int used_bits;

  public:
  unsigned char sample_size_bits;
  unsigned int reference;
  char *samples;

  // Non-copyable
  Record(const Record&) = delete;
  Record& operator=(const Record&) = delete;

  // No default constructor
  Record() = delete;

  Record(int sample_size_bits, int n)
    : used_bits(0),
      sample_size_bits(sample_size_bits),
      reference(0),
      samples(new char[sample_size_bits*n/8]) {}
  ~Record() { if (samples) delete samples; }

  // Move constructor
  Record(Record&& other)
    : used_bits(other.used_bits),
      sample_size_bits(other.sample_size_bits),
      reference(other.reference),
      samples(other.samples) { other.samples = nullptr; }

  // Move assignment
  Record& operator=(Record&& other) {
    this->used_bits = other.used_bits;
    this->reference = other.reference;
    this->sample_size_bits = other.sample_size_bits;
    this->samples = other.samples;
    other.samples = nullptr;
    return *this;
  }

  void push_sample(uint32_t s);
};

class BlockingQueue {
  std::mutex *mtx;
  std::condition_variable *cv;
  std::queue<Record> q;
  int max_size;
  bool closed;

  public:
  explicit BlockingQueue(int size);
  ~BlockingQueue();

  // Non-copyable
  BlockingQueue(const BlockingQueue&) = delete;
  BlockingQueue& operator=(const BlockingQueue&) = delete;

  // Move-constructable
  BlockingQueue(BlockingQueue&& other);
  BlockingQueue& operator=(BlockingQueue&& other);

  void push(Record&& x);
  Record pop();
  void close();
  bool isClosed();
};

struct WorkerContext {
  std::condition_variable& cv;
  std::mutex& lock;
  std::ifstream& input;
  int N;
};

void do_work(
  int thread_id,
  std::vector<BlockingQueue>& output_queue,
  WorkerContext& ctx);

#endif // WORKER_H_
