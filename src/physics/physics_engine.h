#ifndef PHYSICS_ENGINE_H_
#define PHYSICS_ENGINE_H_

#include <Eigen/Dense>

#include "physics/physics_object.h"

class PhysicsEngine {
 public:
  PhysicsEngine(int screen_w, int screen_h, int container_radius,
                int simulation_rate, int substeps);
  void Update();
  void CreatePhysicsObject(Vector2d position, double radius);
  void CreatePhysicsObject(Vector2d position, Vector2d velocity, double radius);
  void ApplyGravity();
  void UpdatePositions(double dt);
  void ApplyConstraints();
  void SolveCollisions();
  uint64_t GetSimTime();
  const std::vector<PhysicsObject>& GetObjects();

 private:
  int screen_w_;
  int screen_h_;
  int container_radius_;
  std::vector<PhysicsObject> objects_;
  Vector2d gravity_{0, 1000};
  int substeps_;
  double dt_;
  double sub_dt_;
  uint64_t sim_time_{0};
};

#endif  // !PHYSICS_ENGINE_H_
