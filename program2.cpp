// Bailey Kiehl
// Programming Assignment 2


#include <iostream>
#include <cstdlib>
#include <queue>


using namespace std;

const int NUM_PROCESSES = 10000;
const double AVERAGE_SERVICE_TIME = 0.04;

enum EventType { ARRIVAL, DEPARTURE };

struct Event {
    double time;
    EventType type;
    int processId;
    double arrivalTime;

    Event(double t, EventType ty, int id, double at = 0.0) : time(t), type(ty), processId(id), arrivalTime(at) {}

    bool operator<(const Event& other) const {
        return time > other.time;
    }
};






double random_uniform() {
    return (double)rand() / (RAND_MAX + 1.0);
}

double random_exponential(double lambda) {
    double U = random_uniform();
    return -log(U) / lambda;
}

double exp_approx(double x) {
    return pow(1 - x / (x + 1), x);
}

int random_poisson(double lambda) {
    double L = exp_approx(-lambda);
    double p = 1.0;
    int k = 0;


    do {

        k++;

        p *= random_uniform();
    } while (p > L);

    return k - 1;
}




void simulate(double lambda) {
    priority_queue<Event> eventQueue;

    double currentTime = 0.0;
    int numProcessesCompleted = 0;
    double totalTurnaroundTime = 0.0;
    double totalCPUTime = 0.0;
    double totalReadyQueueTime = 0.0;

    int readyQueueLength = 0;
    int processId = 0;

    
    double firstArrivalTime = random_exponential(lambda);

    eventQueue.push(Event(firstArrivalTime, ARRIVAL, processId++, firstArrivalTime));

    while (numProcessesCompleted < NUM_PROCESSES) {
        Event currentEvent = eventQueue.top();


        eventQueue.pop();
        currentTime = currentEvent.time;

        if (currentEvent.type == ARRIVAL) {
            
            double serviceTime = random_exponential(1.0 / AVERAGE_SERVICE_TIME);
            double departureTime = currentTime + serviceTime;
            eventQueue.push(Event(departureTime, DEPARTURE, currentEvent.processId, currentEvent.arrivalTime));

           
            double interArrivalTime = random_exponential(lambda);
            eventQueue.push(Event(currentTime + interArrivalTime, ARRIVAL, processId++, currentTime + interArrivalTime));

            readyQueueLength++;
            totalReadyQueueTime += readyQueueLength * (departureTime - currentTime);
        } else {
            
            numProcessesCompleted++;
            double turnaroundTime = currentTime - currentEvent.arrivalTime; // Corrected turnaround time calculation
            totalTurnaroundTime += turnaroundTime;
            totalCPUTime += currentEvent.time - currentEvent.arrivalTime;

            readyQueueLength--;
        }
    }

    double averageTurnaroundTime = totalTurnaroundTime / NUM_PROCESSES;
    double throughput = NUM_PROCESSES / currentTime;
    double cpuUtilization = totalCPUTime / currentTime;
    double averageReadyQueueLength = totalReadyQueueTime / currentTime;

    cout << "Average arrival rate (lambda): " << lambda << endl;
    cout << "Average turnaround time: " << averageTurnaroundTime << endl;
    cout << "Throughput: " << throughput << endl;
    cout << "CPU utilization: " << cpuUtilization << endl;
    cout << "Average ready queue length: " << averageReadyQueueLength << endl;
    cout << endl;
}





int main() {

    for (double lambda = 10.0; lambda <= 30.0; lambda += 1.0) {
        simulate(lambda);
    }

    return 0;
}