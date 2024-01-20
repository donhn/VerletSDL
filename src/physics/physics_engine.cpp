#include "physics/physics_engine.h"

#include <SDL_timer.h>

PhysicsEngine::PhysicsEngine(int screen_w, int screen_h, int container_radius,
                             int simulation_rate, int substeps)
    : screen_w_(screen_w),
      screen_h_(screen_h),
      container_radius_(container_radius),
      substeps_(substeps) {
  dt_ = 1.0 / simulation_rate;
  sub_dt_ = dt_ / substeps_;
}

void PhysicsEngine::CreatePhysicsObject(Vector2d position, double radius) {
  objects_.push_back(PhysicsObject(position, radius));
}

void PhysicsEngine::CreatePhysicsObject(Vector2d position, Vector2d velocity,
                                        double radius) {
  PhysicsObject obj = PhysicsObject(position, radius);
  obj.SetVelocity(velocity, dt_);
  objects_.push_back(obj);
}

void PhysicsEngine::Update() {
  uint64_t sim_start = SDL_GetTicks64();
  for (int steps = 0; steps < substeps_; steps++) {
    ApplyGravity();
    ApplyConstraints();
    SolveCollisions();
    UpdatePositions(sub_dt_);
  }
  sim_time_ = SDL_GetTicks64() - sim_start;
}

void PhysicsEngine::SolveCollisions() {
  int num_objects = objects_.size();
  for (int i = 0; i < num_objects; i++) {
    auto& obj1 = objects_[i];
    for (int j = i + 1; j < num_objects; j++) {
      auto& obj2 = objects_[j];
      Vector2d collision_v = obj1.position_current - obj2.position_current;
      if (collision_v.norm() < obj1.radius + obj2.radius) {
        Vector2d collision_n = collision_v.normalized();
        double collision_d = (obj1.radius + obj2.radius) - collision_v.norm();
        obj1.position_current += (collision_n * collision_d) / 2;
        obj2.position_current -= (collision_n * collision_d) / 2;
      }
    }
  }
}

void PhysicsEngine::ApplyConstraints() {
  const Vector2d center{screen_w_ / 2, screen_h_ / 2};
  const double radius = container_radius_;
  for (auto& obj : objects_) {
    // Calculate distance from center of container.
    const Vector2d dist = obj.position_current - center;
    // If object is further than radius, put it back inside.
    if (dist.norm() > radius - obj.radius) {
      Vector2d norm = dist.normalized();
      obj.position_current =
          center + (dist.normalized() * (radius - obj.radius));
    }
  }
}

void PhysicsEngine::UpdatePositions(double dt) {
  for (auto& obj : objects_) {
    obj.UpdatePosition(dt);
  }
}

void PhysicsEngine::ApplyGravity() {
  for (auto& obj : objects_) {
    obj.AddAcceleration(gravity_);
  }
}

const std::vector<PhysicsObject>& PhysicsEngine::GetObjects() {
  return objects_;
}

uint64_t PhysicsEngine::GetSimTime() { return sim_time_; }
