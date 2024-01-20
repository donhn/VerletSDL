#ifndef PHYSICS_OBJECT_H_
#define PHYSICS_OBJECT_H_

#include <Eigen/Dense>

using Eigen::Vector2d;
class PhysicsObject {
 public:
  PhysicsObject(Vector2d position_init, double radius)
      : position_current(position_init),
        position_old(position_init),
        radius(radius),
        diameter(radius * 2) {}

  Vector2d GetRenderPosition() {
    return position_current - Vector2d(radius, radius);
  };
  void UpdatePosition(double dt) {
    Vector2d velocity = position_current - position_old;
    position_old = position_current;
    position_current = position_current + velocity + acceleration * dt * dt;
    acceleration.setZero();
  }

  void SetVelocity(Vector2d velocity, double dt) {
    position_old = position_current - (velocity * dt);
  }

  void AddAcceleration(Vector2d acc) { acceleration += acc; }

  std::string GetPropertiesString() {
    return "Current Position: (" + std::to_string(position_current[0]) + "," +
           std::to_string(position_current[1]) + ")\n" +
           "Previous Position: (" + std::to_string(position_old[0]) + "," +
           std::to_string(position_old[1]) + ")";
  }

  Vector2d position_current{0.0, 0.0};
  Vector2d position_old{0.0, 0.0};
  Vector2d acceleration{0.0, 0.0};
  double radius{0};
  double diameter{0};
  struct Color {
    int r;
    int g;
    int b;
  } color{rand() % 255, rand() % 255, rand() % 255};
};
#endif  // !PHYSICS_OBJECT_H_
