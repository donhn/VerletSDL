#include <SDL_events.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_timer.h>

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cstdint>
#include <string>

#include "engine/engine.h"
#include "physics/physics_engine.h"

constexpr int kScreenWidth = 1920;    // Px
constexpr int kScreenHeight = 1080;   // Px
constexpr int kTargetFrameRate = 60;  // Hz

constexpr int kMaxObjs = 2000;
constexpr int kObjSpawnRate = 40;  // Hz
constexpr int kObjInitVel = 200;
constexpr int kObjMinSize = 5;
constexpr int kObjMaxSize = 10;
constexpr double kRotationDeg = 5.7;

constexpr int kSubsteps = 8;
constexpr int kSimulationRate = 60;  // Hz
constexpr int kContainerRadius = 530;

// Updating text is expensive. Only update the text occasionally.
constexpr int kStatisticsRefreshRate = 2;  // Hz

int main(int argc, char *argv[]) {
  SDL2Engine engine = SDL2Engine(kScreenWidth, kScreenHeight);
  PhysicsEngine physics =
      PhysicsEngine(kScreenWidth, kScreenHeight, kContainerRadius,
                    kSimulationRate, kSubsteps);

  engine.Init("Verlet Simulation");
  engine.IMGLoadTexture("circle", "assets\\images\\circle.png");
  engine.LoadFont("assets\\fonts\\JetBrainsMonoNerdFont-Regular.ttf", 200);

  // Calculate sim parameters.
  const int sim_interval = 1000 / kSimulationRate;
  const int frame_interval = 1000 / kTargetFrameRate;
  const int spawn_interval = 1000 / kObjSpawnRate;
  const int stats_interval = 1000 / kStatisticsRefreshRate;

  // Calculate container parameters.
  // Draw container in the middle of the screen.
  const int container_x = (kScreenWidth / 2 - kContainerRadius);
  const int container_y = (kScreenHeight / 2 - kContainerRadius);

  // Calculate object spawnining parameters.
  const Vector2d spawn_location{kScreenWidth / 2, kScreenHeight / 2};
  const Eigen::Rotation2D<double> velocity_rot(kRotationDeg * M_PI / 180.0);
  Vector2d velocity_v{0.0, kObjInitVel};

  // Create statistics container and data
  SDL_Rect stat_rect = {10, 10, 255, 65};
  int last_render_time = 0;
  int obj_count = 0;

  // Create timers.
  uint64_t next_render = SDL_GetTicks64();
  uint64_t next_stat_update = SDL_GetTicks64();
  uint64_t next_physics = SDL_GetTicks64();
  uint64_t next_create = SDL_GetTicks64();

  SDL_Event event;
  bool simulation_running = true;
  while (simulation_running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          simulation_running = false;
          break;
      }
    }

    // Update physics
    if (SDL_GetTicks64() > next_physics) {
      physics.Update();
      next_physics = SDL_GetTicks64() + sim_interval;
    }

    // Spawn objects
    if (obj_count < kMaxObjs && SDL_GetTicks64() > next_create) {
      obj_count++;
      // Random radius between 5-10.
      double radius = kObjMinSize + (rand() % (kObjMaxSize - kObjMinSize));
      physics.CreatePhysicsObject(spawn_location, velocity_v, radius);
      velocity_v = velocity_rot * velocity_v;
      next_create = SDL_GetTicks64() + spawn_interval;
    }

    // Render frame
    if (SDL_GetTicks64() > next_render) {
      uint64_t render_start_time = SDL_GetTicks64();

      // Draw Statistics
      if (render_start_time > next_stat_update) {
        // clang-format off
        std::string stat_txt =
            "Simulation Time: " + std::to_string(physics.GetSimTime()) + " (ms)\n" +
            "Objects: " + std::to_string(obj_count) + "\n"
            "Render Time: " + std::to_string(last_render_time) + " (ms)";
        // clang-format on
        engine.UpdateText("stats", stat_txt, stat_rect, {0, 0, 0});
        next_stat_update = SDL_GetTicks64() + stats_interval;
      }
      engine.DrawText("stats", stat_rect);

      // Draw containing circle.
      engine.SetTextureColor("circle", 0, 0, 0);
      engine.DrawTexture("circle", container_x, container_y,
                         kContainerRadius * 2, kContainerRadius * 2);

      // Draw physics objects.
      engine.SetTextureColor("circle", 255, 255, 255);
      for (auto obj : physics.GetObjects()) {
        Vector2d render_position = obj.GetRenderPosition();
        engine.DrawTexture("circle", render_position[0], render_position[1],
                           obj.diameter, obj.diameter);
        engine.SetTextureColor("circle", obj.color.r, obj.color.g, obj.color.b);
      }

      engine.Render();

      uint64_t render_end_time = SDL_GetTicks64();
      last_render_time = render_end_time - render_start_time;
      next_render = render_end_time + frame_interval;
    }
  }

  return 0;
}
