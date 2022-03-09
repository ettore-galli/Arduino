#define NREADINGS 10

volatile float readings[NREADINGS];
volatile int readingsIdx = 0;

void setNewReading(int reading)
{
    readings[readingsIdx] = reading;
    readingsIdx = (readingsIdx + 1) % NREADINGS;
}

float getReadingsAverage()
{
    float sum = 0.0;
    for (int i = 0; i < NREADINGS; i++) {
        sum += readings[i];
    }
    return sum / NREADINGS;
}