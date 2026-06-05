# Projectile Motion Simulator

A real-time projectile motion visualizer written in C++ using raylib. Takes an initial velocity and launch angle from the terminal and animates the trajectory in a 1920x1080 window with live stats.

---

## Dependencies

- g++ with C++11 or later
- [raylib](https://www.raylib.com/)

## Build

```
g++ main.cpp -o projectile -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Usage

```
./projectile
```

The program prompts for initial velocity and launch angle in the terminal before opening the window.

```
Enter initial velocity (+ve): 35
Enter angle (+ve degrees): 60
```

---

## Details

Input velocity is passed through a sigmoid function to map it into a screen-appropriate range, keeping the parabola visible regardless of the magnitude entered. The stats displayed during the animation — range, peak height, and time of flight — are computed from the original unmapped values and reflect real physics.

The trajectory is computed via linear state-vector propagation using homogeneous coordinates. The kinematic equations are expressed as a 5x5 matrix acting on the state vector `[x, y, vx, vy, 1]` at each timestep, stepped across 500 points over the total time of flight.

---

## Display

| Element        | Description                                  |
|----------------|----------------------------------------------|
| Red circle     | Projectile                                   |
| Time readout   | Elapsed time mapped to real time of flight   |
| Peak height    | Shown at the halfway point of the trajectory |
| Range          | Shown on landing                             |

---

## License

MIT
