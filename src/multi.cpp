#include "integral_solver.hpp"

static const size_t CHILDREN_CNT = 4;

static const size_t TASKS_CNT = 16;
static const size_t TASK_FRAC = FRAC / TASKS_CNT;


static const std::function<double(double)> functions[] = {
  xsin2x,
  exsinex,
  expx2,
  x2sinx2,
  x2ln1_plus_x,
  x3
};

static constexpr double answers[] = {
  2.4674,
  0.949004,
  3267.24,
  1.70073,
  12.3138,
  24.3523
};

void ChildRoutine() {
  ManagedSharedMemory managed_shared_memory(find_only, "managed_shared_memory");

  BlockingQueue<IntegralTask, TASKS_CNT>* task_queue = managed_shared_memory.Find<BlockingQueue<IntegralTask, TASKS_CNT>>("task_queue");
  BlockingQueue<double, TASKS_CNT>* results_queue = managed_shared_memory.Find<BlockingQueue<double, TASKS_CNT>>("results_queue");

  while (true) {
    IntegralTask task = task_queue->Take();
    if (task.Last()) {
      break;
    }
    results_queue->Put(task.Complete());
  }
}

double MultiSolution(std::function<double(double)> f) {
  double result = 0;

  pid_t creator_id = getpid();

  ManagedSharedMemory managed_shared_memory(create_only, "managed_shared_memory", SHARED_SIZE);

  BlockingQueue<IntegralTask, TASKS_CNT>* task_queue = managed_shared_memory.Construct<BlockingQueue<IntegralTask, TASKS_CNT>>("task_queue");
  Destructor<BlockingQueue<IntegralTask, TASKS_CNT>> task_destr(task_queue, creator_id);

  BlockingQueue<double, TASKS_CNT>* results_queue = managed_shared_memory.Construct<BlockingQueue<double, TASKS_CNT>>("results_queue");
  Destructor<BlockingQueue<double, TASKS_CNT>> results_destr(results_queue, creator_id);

  MakeChildren(CHILDREN_CNT);

  if (getpid() != creator_id) {
    ChildRoutine();
  } else {

    double lhs_lim = 0;
    double rhs_lim = M_PI;

    for (size_t i = 0; i < TASKS_CNT; ++i) {
      task_queue->Put(IntegralTask{f, GetIndexed(lhs_lim, rhs_lim, i, TASKS_CNT), GetIndexed(lhs_lim, rhs_lim, i + 1, TASKS_CNT), TASK_FRAC});
    }

    for (size_t i = 0; i < TASKS_CNT; ++i) {
      result += results_queue->Take();
    }

    for (size_t i = 0; i < TASKS_CNT; ++i) {
      task_queue->Put(IntegralTask{});
    }
  }

  WaitChildren(creator_id);
  return result;
}

int main(int, char** argv) {
  int i = atoi(argv[1]);
  pid_t parent_id = getpid();
  double res = 0;
  res = MultiSolution(functions[i]);
  if (getpid() == parent_id) {
    assert(fabs(res - answers[i]) < eps);
  }
  return 0;
}
