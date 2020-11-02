#pragma once
#include "kappa/api.hpp"
#include "tracking/abstractTracker.hpp"

template <std::size_t N>
class LookaheadTracker: public AbstractTracker<N> {
public:
  LookaheadTracker(double ilookaheadDist):
    lookaheadDist(ilookaheadDist),
    lookaheadDistSqr(ilookaheadDist * ilookaheadDist){

    lastWaypoint.fill(0);
  }

  std::array<double,N> getGoalPoint(double robotx, double roboty) {
    return getGoalPoint(robotx, roboty, lookaheadDistSqr);
  }

  std::array<double,N> getGoalPoint(double robotx, double roboty, double effectiveLookaheadSqr);

protected:
  double lookaheadDist;
  double lookaheadDistSqr;

  std::array<double,N> lastWaypoint;
};
