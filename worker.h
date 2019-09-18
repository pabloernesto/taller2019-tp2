#ifndef WORKER_H_
#define WORKER_H_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>

struct Record {
  unsigned int reference;
  unsigned char sample_size_bits;
  char *samples;

  // Non-copyable
  Record(const Record&) = delete;
  Record& operator=(const Record&) = delete;

  // No default constructor
  Record() = delete;

  Record(int sample_size_bits, int n)
    : reference(0),
      sample_size_bits(sample_size_bits),
      samples(new char[sample_size_bits*n/8]) {}
  ~Record() { if (samples) delete samples; }

  // Move constructor
  Record(Record&& other)
    : reference(other.reference),
      sample_size_bits(other.sample_size_bits),
      samples(other.samples) { other.samples = nullptr; }

  // Move assignment
  Record& operator=(Record&& other) {
    this->reference = other.reference;
    this->sample_size_bits = other.sample_size_bits;
    this->samples = other.samples;
    other.samples = nullptr;
    return *this;
  }
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
