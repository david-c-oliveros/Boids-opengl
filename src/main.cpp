#include "Boids.h"


int main()
{
    Boids boids = Boids();

    if (boids.Construct())
        boids.Start();

    return 0;
}
