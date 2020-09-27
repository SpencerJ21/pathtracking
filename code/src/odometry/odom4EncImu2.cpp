#include "odometry/odom4EncImu2.hpp"

Odom4EncImu2::Odom4EncImu2(
              std::unique_ptr<okapi::Filter> ivelFilter,
              std::unique_ptr<okapi::Filter> istfVelFilter,
              std::unique_ptr<okapi::Filter> iangVelFilter,
              std::shared_ptr<kappa::AbstractInput<std::array<double,5>>> iinput):
              input(iinput), velFilter(std::move(ivelFilter)),
              stfVelFilter(std::move(istfVelFilter)),
              angVelFilter(std::move(iangVelFilter))
  {
   lastIn[5] = pros::millis();
  }

const std::array<double,6> &Odom4EncImu2::step() {
  const std::array<double,5> &in = input->get();

  double dL     = in[0] - lastIn[0];
  double dB     = in[1] - lastIn[1];
  double dR     = in[2] - lastIn[2];
  double dF     = in[3] - lastIn[3];
  double dTheta = (in[4] - lastIn[4]) * M_PI / 180;
  double dT = pros::millis() - lastIn[5];

  if(dT == 0) return pose;

  lastIn[5] = pros::millis();
  lastIn[0] = in[0];
  lastIn[1] = in[1];
  lastIn[2] = in[2];
  lastIn[3] = in[3];
  lastIn[4] = in[4];

  if(dTheta == 0){
    double lsin = sin(pose[2]);
    double lcos = cos(pose[2]);

    double dV  = (dL + dR) / 2.0;
    double dSV = (dB + dF) / 2.0;

    pose[0] += lcos * dV -
               lsin * dSV;
    pose[1] += lcos * dSV +
               lsin * dV;

    pose[3] = velFilter->   filter(dV  / dT);
    pose[4] = stfVelFilter->filter(dSV / dT);
    pose[5] = angVelFilter->filter(0);
  }else{
    double lsin = sin(pose[2] + 0.5 * dTheta);
    double lcos = cos(pose[2] + 0.5 * dTheta);
    double cOffset = 2 * sin(dTheta / 2);

    double A_r = (dL + dR) / (2 * dTheta);
    double S_r = (dB + dF) / (2 * dTheta);

    pose[0] += cOffset * (lcos * A_r - lsin * S_r);
    pose[1] += cOffset * (lcos * S_r + lsin * A_r);
    pose[2] += dTheta;
    pose[3] = velFilter->   filter(A_r * dTheta / dT);
    pose[4] = stfVelFilter->filter(S_r * dTheta / dT);
    pose[5] = angVelFilter->filter(      dTheta / dT);
  }

  return pose;
}

const std::array<double,6> &Odom4EncImu2::get() {
  return pose;
}