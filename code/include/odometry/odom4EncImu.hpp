#pragma once
#include "kappa/api.hpp"

class Odom4EncImu : public kappa::ComputationalInput<std::array<double,6>> {
public:
  Odom4EncImu(std::unique_ptr<okapi::Filter> ivelFilter,
              std::unique_ptr<okapi::Filter> istfVelFilter,
              std::unique_ptr<okapi::Filter> iangVelFilter,
              std::shared_ptr<kappa::AbstractInput<std::array<double,5>>> iinput); // left, back, right, front, imu (positive dir front;left;ccw)

  virtual const std::array<double,6> &step();

  virtual const std::array<double,6> &get();

protected:
  std::shared_ptr<kappa::AbstractInput<std::array<double,5>>> input;
  std::unique_ptr<okapi::Filter> velFilter;
  std::unique_ptr<okapi::Filter> stfVelFilter;
  std::unique_ptr<okapi::Filter> angVelFilter;

  std::array<double,6> pose{0,0,0,0,0,0};

  std::array<double,6> lastIn{0,0,0,0,0,0};
};
