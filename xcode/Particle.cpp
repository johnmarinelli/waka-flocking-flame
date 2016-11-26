#include "Particle.hpp"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"

Particle::Particle(const glm::vec3& loc) :
  mLocation(loc),
  mVelocity(ci::randVec3() * 0.01f),
  mRadius(ci::randFloat(0.05f))
{
}

glm::vec3 Particle::cohesion(const std::vector<Particle>& neighbors) const {
  glm::vec3 sum{0};
  
  
  for (const auto& n : neighbors) {
    sum += n.mLocation;
  }

  if (neighbors.size() > 0) {
    return steerTo(sum / float(neighbors.size()));
  }
  else {
    return sum;
  }
}

static const float MAX_STEER_SPEED = 1.f;
static const float MAX_FORCE = 1.f;
glm::vec3 Particle::steerTo(const Particle &other) const {
  // vector pointing from location to target
  auto v1 = other.mLocation - mLocation;
  glm::vec3 steer{0};
  
  // magnitude of v1
  auto d = glm::length(v1);
  if (d > 0.f) {
    v1 = glm::normalize(v1);
    
    steer = v1 - mVelocity;
    steer = limitVec3(steer, MAX_FORCE);
  }

  return steer;
}

static const float MAX_ALIGN_FORCE = 1.f;
glm::vec3 Particle::align(const std::vector<Particle>& neighbors) const {
  glm::vec3 mean{0};
  
  for (const auto& n : neighbors) {
    mean += n.mVelocity;
  }
  
  if (neighbors.size() > 0) {
    mean /= float(neighbors.size());
  }
  
  mean = limitVec3(mean, MAX_ALIGN_FORCE);
  
  return mean;
}

static const float DESIRED_SEPARATION = 2.f;
glm::vec3 Particle::separate(const std::vector<Particle>& neighbors) const {
  
  glm::vec3 mean{0};
  
  for (const auto& n : neighbors) {
    auto dis = glm::distance(mLocation, n.mLocation);
    
    if (dis > 0.f && dis < DESIRED_SEPARATION) {
      auto diff = mLocation - n.mLocation;
      auto ndiff = glm::normalize(diff);
      auto dndiff = ndiff / dis;
      
      mean += dndiff;
    }
    
  }
  
  if (neighbors.size() > 0) {
    mean /= float(neighbors.size());
  }
  
  return mean;
}

static const float SEPARATION_WEIGHT = 1.f;
static const float ALIGNMENT_WEIGHT = 1.f;
static const float COHESION_WEIGHT = 1.f;
static const float MAX_SPEED = 0.25f;

glm::vec3 Particle::flock(const std::vector<Particle>& neighbors) {
  auto sep = separate(neighbors) * SEPARATION_WEIGHT;
  auto al = align(neighbors) * ALIGNMENT_WEIGHT;
  auto coh = cohesion(neighbors) * COHESION_WEIGHT;
  
  return sep + al + coh;
}

void Particle::update(const std::vector<Particle>& neighbors) {
  auto accel = flock(neighbors);
  mVelocity += accel;
  mVelocity = limitVec3(mVelocity, MAX_SPEED);

  mLocation = limitVec3(mLocation + mVelocity, 10.f);
}

void Particle::draw() {
  cinder::gl::drawSphere(mLocation, mRadius);
}
