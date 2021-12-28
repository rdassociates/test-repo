// Simple_Threaded_Scheduler.cpp : Defines the entry point for the application.
//

#include "Simple_TaskScheduler.h"

//bool gStart = false;
class Task {
	int value;
	mutex mtxLog;
public:
	Task(int x) :value(x) {}
	void out(int a) {
		cout << a;
	}
	void complete() {
		mtxLog.lock();		//Because cout is not Thread safe
		cout << "Task Complete:"; //<< value << endl;
		mtxLog.unlock();
	}
};

class Worker {
	Task* task;
	bool ready;
	condition_variable cv;
	mutex mtx;
	unique_lock<mutex> ulock;
public:
	Worker(){ 
		ready = false; 
		unique_lock<mutex>(m);
		task = nullptr;
	}
	typedef void(Worker::* funPtr)();
	void AssignTask(Task* t) {
		unique_lock<mutex> ulock(mtx);
		bool r = ready;
		cv.wait(ulock, [r]
			{	//Wait until this code block return true
				return !r;
			}
		);
		ready = true;
		this->task = t; 
		ulock.unlock();
		cv.notify_all();
	}

	void run() {
		while (1) {
			unique_lock<mutex> ulock(mtx);
			bool r = ready;
			cv.wait(ulock, [r] 
				{ 
					//Wait until this code block return true
					return r; 
				}
			);

			ready = false;
			//task->complete();
			cout << "Task Complete";
			ulock.unlock();
		}
	}
};
queue<Worker*> queue_worker;
queue<Task*> queue_task;
mutex mtxQueueTask;
mutex mtxQueueWorker;
int main()
{
	for (int i = 0; i < 2; ++i) {
		//1.Create worker class objects. ready = false, unique_lock(mutex), task=nullptr
		Worker* w = new Worker;

		//2. Push all workers in queue_worker
		queue_worker.push(w);

		//3. Create threads, all will execute Worker::run()
		thread* t = new thread(&Worker::run, w);	//run() declaration
	}
		
	for (int i = 0; i < 50 ; ++i) {

		//4. Create Task Object
		Task* t = new Task(i);

		//5. Lock worker_queue mutex, if queue has worker present
		mtxQueueWorker.lock();

		//6. if worker queue is not empty, Get 1st worker &
		//assign task to it
		if (!queue_worker.empty()) {
			Worker* front = queue_worker.front();
			front->AssignTask(t);							//Set task for worker
			queue_worker.pop();
			mtxQueueWorker.unlock();
		}
		else {		//Queue empty
			mtxQueueWorker.unlock();
			mtxQueueTask.lock();
			queue_task.push(t);
			mtxQueueTask.unlock();
		}
	}
	return 0;
}
